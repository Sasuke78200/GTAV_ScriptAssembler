#include "main.h"



Decompiler::Decompiler()
{
	int l_iPlateform;
	m_aByteCode = 0;

	
	l_iPlateform = atoi(CommandLine::Instance()->getVal("-p")->c_str());
	switch(l_iPlateform)
	{
	case 0: // pc
		m_pScrHeader = new YscHeader();
		break;
	case 1: // ps3
		// m_pScrHeader = new CscHeader();
		m_pScrHeader = 0; 
		assert(!"Only PC scripts are supported !");
		break;
	default:
		m_pScrHeader = new YscHeader();
		break;
	}
	

}

Decompiler::~Decompiler()
{
	// TODO: Free 'm_aByteCode'
}


bool Decompiler::DecompileFile(char* a_szBinaryPath, char* a_szOutputPath)
{
	m_pBinaryFile = new std::ifstream(a_szBinaryPath, std::ifstream::in |std::ifstream::binary);

	// if we couldn't alloc the std::ifstream, we return false
	if(m_pBinaryFile == 0)
	{
		return false;
	}

	// if the file isn't open, we return false;
	if(m_pBinaryFile->is_open() == false)
	{
		delete m_pBinaryFile;
		m_pBinaryFile = 0;
		return false;
	}
	
	m_pScrHeader->ReadFromFile(m_pBinaryFile);
		
	printf("Decompiling script \"%s\" ...\n", m_pScrHeader->getName().c_str());

	ReadByteCode();

	if(ValidateBinary())
	{
		m_stringCollector.importFromBinary(this->m_pScrHeader, this->m_pBinaryFile);
		m_nativeCollector.importFromBinary(this->m_pScrHeader);
		ConvertToInstructions();
		ProcessInstructions();
		CollectFunctions();
		BuildAST();
		printf("Script decompiled !\n");
	}
	else
	{
		printf("Unable to validate the binary !\n");
	}
	delete m_pBinaryFile;
	m_pBinaryFile = 0;
	return true;
}

void Decompiler::ReadByteCode()
{
	int i;

	m_aByteCode = new unsigned char*[m_pScrHeader->getCodePageCount()];

	for(i = 0; i < m_pScrHeader->getCodePageCount(); i++)
	{
		m_aByteCode[i] = new unsigned char[m_pScrHeader->getCodePageLength(i)];
		this->m_pBinaryFile->seekg(m_pScrHeader->getCodePageOffset(this->m_pBinaryFile, i));
		this->m_pBinaryFile->read((char*)m_aByteCode[i], m_pScrHeader->getCodePageLength(i));
	}
}

void Decompiler::ConvertToInstructions()
{
	int				l_uiByteCodeLength;
	int				l_uiBytecodeAddr;
	unsigned char	l_bOpcode;
	Instruction*	l_pInstruction;

	l_uiByteCodeLength = m_pScrHeader->getByteCodeLength();

	m_Instructions.clear();


	for(l_uiBytecodeAddr = 0; l_uiBytecodeAddr < l_uiByteCodeLength;)
	{
		l_bOpcode = m_aByteCode[l_uiBytecodeAddr / 0x4000][l_uiBytecodeAddr % 0x4000];
	
		if(l_bOpcode == 38 || l_bOpcode == 39)
		{
			InstructioniPush** l_iPushes = new InstructioniPush*[l_bOpcode - 36];
			for(int i = 0; i < l_bOpcode - 36; i++)
			{
				l_iPushes[i] = new InstructioniPush();
				l_iPushes[i]->setAddress(l_uiBytecodeAddr);
				l_iPushes[i]->setValue(m_aByteCode[(l_uiBytecodeAddr + i + 1) / 0x4000][(l_uiBytecodeAddr + i + 1) % 0x4000]);
				this->m_Instructions.push_back(l_iPushes[i]);
			}
			// 38 is ipush byte byte (len 3 bytes)
			// 39 is ipush byte byte byte (len 4 bytes)
			l_uiBytecodeAddr += l_bOpcode - 35;
			continue;
		}

		l_pInstruction = Instruction::allocFromOpcode(l_bOpcode);

		if(l_pInstruction)
		{
			l_pInstruction->setAddress(l_uiBytecodeAddr);
			l_pInstruction->Process(&m_aByteCode[l_uiBytecodeAddr / 0x4000][l_uiBytecodeAddr % 0x4000]);
			this->m_Instructions.push_back(l_pInstruction);
		}
		else
		{
			std::stringstream l_ss;
			l_pInstruction = new InstructionBasic();			
			l_pInstruction->setLength(getOpcodeLen(l_bOpcode));

			l_ss << "unkop_" << (int)l_bOpcode;

			l_pInstruction->setName(l_ss.str());
			l_pInstruction->setAddress(l_uiBytecodeAddr);
			this->m_Instructions.push_back(l_pInstruction);

			//printf("Decompiler::ConvertToInstructions() -> Unk opcode %d\n", l_bOpcode);
		}

		l_uiBytecodeAddr += getOpcodeLenByAddr(l_uiBytecodeAddr);
	}
}

void Decompiler::ProcessInstructions()
{
	std::vector<Instruction*>::iterator it;

	for(it = this->m_Instructions.begin(); it != this->m_Instructions.end(); it++)
	{
		if((*it)->getOpcode() == 99)		// string push
		{
			InstructioniPush* l_piPush;
			InstructionsPush* l_psPush;
			std::vector<Instruction*>::iterator previous_it = it - 1;
			while((*previous_it)->getName() != "ipush") previous_it--; // it should be the good one without even entering the loop once, well that's how R* compiler does (ALWAYS !)

			l_piPush = (InstructioniPush*)*previous_it;

			l_psPush = (InstructionsPush*)*it;
			l_psPush->setStringCollector(&this->m_stringCollector);
			l_psPush->setIndex(l_piPush->getValue());

			*previous_it = new InstructionBasic();
			(*previous_it)->setAddress(l_piPush->getAddress());
			(*previous_it)->setName("DummyInstruction"); // that's a hack, I know

			delete l_piPush;			
			//it = this->m_Instructions.erase(previous_it);
		}
		else if((*it)->getOpcode() >= 85 && (*it)->getOpcode() <= 92)	// any jump
		{
			std::stringstream l_ss;
			InstructionJmp* l_pJmp = (InstructionJmp*)*it;
			l_pJmp->setLabelCollector(&this->m_labelCollector);

			l_ss << "label_" << std::setfill('0') << std::setw(4) << std::hex << l_pJmp->getJumpAddress();
			l_pJmp->setLabel(l_ss.str());
			this->m_labelCollector.AddLabel(l_ss.str(), l_pJmp->getJumpAddress());			
		}
		else if((*it)->getOpcode() == 93) // call
		{
			std::stringstream l_ss;
			InstructionCall* l_pCall = (InstructionCall*)*it;
			l_pCall->setLabelCollector(&this->m_labelCollector);

			l_ss << "label_" << std::setfill('0') << std::setw(4) << std::hex << l_pCall->getCallAddress();
			l_pCall->setLabel(l_ss.str());
			this->m_labelCollector.AddLabel(l_ss.str(), l_pCall->getCallAddress());
		}
		else if((*it)->getOpcode() == 44) // native
		{
			InstructionNative* l_pNative = (InstructionNative*)*it;

			l_pNative->setNativeCollector(&this->m_nativeCollector);
		}
		else if((*it)->getOpcode() == 98) // switch !
		{
			InstructionSwitch* l_pSwitch = (InstructionSwitch*)*it;
			int i;

			l_pSwitch->setLabelCollector(&this->m_labelCollector);

			for(i = 0; i < l_pSwitch->getCaseCount(); i++)
			{
				std::stringstream l_ss;
				l_ss << "label_" << std::setfill('0') << std::setw(4) << std::hex << l_pSwitch->getCaseJmpAddress(i);
				this->m_labelCollector.AddLabel(l_ss.str(), l_pSwitch->getCaseJmpAddress(i));
			}
		}		
	}
}

void Decompiler::CollectFunctions()
{
	std::vector<Instruction*>::iterator it;

	for(it = this->m_Instructions.begin(); it != this->m_Instructions.end(); it++)
	{
		if((*it)->getName() == "enter")
		{
			m_functionCollector.NewFunction();
			//printf("Function at -> 0x%08X\n", (*it)->getAddress());
		}
		m_functionCollector.AddInstruction(*it);
	}
}


bool Decompiler::BuildAST()
{
	int l_iFunctionCount;
	int i;


	l_iFunctionCount = m_functionCollector.getCount();

	// we're only going to decompile the first function
	l_iFunctionCount = 1;

	for(i = 0; i < l_iFunctionCount; i++)
	{
		if(BuildAstForFunction(i) == false)
			return false;
	}
	return true;
}

std::vector<BasicBlock*> 
	Decompiler::generateBasicBlocks(std::vector<Instruction*>* a_pFunction)
{
	std::vector<BasicBlock*>				l_BasicBlocks;
	std::vector<std::pair<int, int>>		l_Addresses;
	std::vector<Instruction*>::iterator		it;
	std::vector<int>						m_labels;
	int										l_iCurrentAddress;
	unsigned int							i;

	for(it = a_pFunction->begin(); it != a_pFunction->end(); it++)
	{
		if((*it)->getOpcode() >= 85 && (*it)->getOpcode() <= 92)
		{
			m_labels.push_back(((InstructionJmp*)*it)->getJumpAddress());
		}		
	}
	std::sort(m_labels.begin(), m_labels.end());

	// function entry point address
	l_iCurrentAddress = (*a_pFunction->begin())->getAddress();	
	it = a_pFunction->begin();
	i = 0;


	while(l_iCurrentAddress < (a_pFunction->back())->getAddress() + getOpcodeLen(a_pFunction->back()->getOpcode()))
	{
		int l_iLabelAddr = -1, l_iJmpAddr = -1;

		while(i < m_labels.size())
		{
			if(m_labels[i] > l_iCurrentAddress)
			{
				l_iLabelAddr = m_labels[i];
				break;
			}
			i++;
		}

		while(it != a_pFunction->end())
		{
			if((*it)->getAddress() >= l_iCurrentAddress)
			{
				if((*it)->getOpcode() >= 85 && (*it)->getOpcode() <= 92 || (*it)->getOpcode() == 46) // jmp or ret
				{
					 l_iJmpAddr = (*it)->getAddress();
					 break;
				}
			}
			it++;
		}

		if(l_iLabelAddr < l_iJmpAddr && l_iLabelAddr != -1)
		{
			l_Addresses.push_back(std::pair<int, int>(l_iCurrentAddress, l_iLabelAddr));
			//printf("Block start %08X\n", l_iCurrentAddress);
			//printf("Block end %08X\n", l_iLabelAddr);
			l_iCurrentAddress = l_iLabelAddr;
		}
		else if(l_iJmpAddr != -1)
		{
			l_Addresses.push_back(std::pair<int, int>(l_iCurrentAddress, l_iJmpAddr + getOpcodeLenByAddr(l_iJmpAddr)));
			//printf("Block start %08X\n", l_iCurrentAddress);
			//printf("Block end %08X\n", l_iJmpAddr + getOpcodeLenByAddr(l_iJmpAddr));
			l_iCurrentAddress = l_iJmpAddr + getOpcodeLenByAddr(l_iJmpAddr);
		}
	}


	for(i = 0; i < l_Addresses.size(); i++)
	{
		BasicBlock* l_pBlock = new BasicBlock();

		for(it = a_pFunction->begin(); it != a_pFunction->end(); it++)
		{
			if((*it)->getAddress() >= l_Addresses[i].second) break;
			if((*it)->getAddress() >= l_Addresses[i].first)
			{
				l_pBlock->addInstruction(*it);
			}
		}
		l_BasicBlocks.push_back(l_pBlock);
	}

	std::vector<BasicBlock*>::iterator block_it;
	std::vector<BasicBlock*>::iterator block_start;

	// TODO: Optimize this

	for(block_it = l_BasicBlocks.begin(); block_it != l_BasicBlocks.end(); block_it++)
	{
		Instruction* l_pFirst, *l_pLast;
		BasicBlock* l_pBlock = *block_it;
		
		l_pFirst	= l_pBlock->getInstructions()->front();
		l_pLast		= l_pBlock->getInstructions()->back();

		if(l_pBlock->hasJmp())
		{
			for(i = 0; i < l_BasicBlocks.size(); i++)
			{
				
				// jump location
				if(((InstructionJmp*)l_pLast)->getJumpAddress() == l_BasicBlocks[i]->getInstructions()->front()->getAddress())
				{
					l_pBlock->addSuccessor(l_BasicBlocks[i]);
					l_BasicBlocks[i]->setPredecessor(l_pBlock);
				}
				
				// not jumping locatation
				if(l_pBlock->hasUnconditionnalJmp() == false)
				{
					if(l_BasicBlocks[i]->getInstructions()->front()->getAddress() == (l_pLast->getAddress() + getOpcodeLen(l_pLast->getOpcode())))
					{
						l_BasicBlocks[i]->setPredecessor(l_pBlock);
						l_pBlock->addSuccessor(l_BasicBlocks[i]);
					}
				}
			}
		}
		else
		{
			if(block_it != l_BasicBlocks.end() - 1)
			{
				l_pBlock->addSuccessor(*(block_it + 1));
				(*(block_it + 1))->setPredecessor(l_pBlock);
			}			
		}
	}

	//TODO: Free those basic blocks !
	return l_BasicBlocks;
}

bool Decompiler::BuildAstForFunction(int a_iIndex)
{
	std::vector<Instruction*>*				l_pFunction = m_functionCollector.getFunction(a_iIndex);	


	std::vector<BasicBlock*> l_BasicBlocks = generateBasicBlocks(l_pFunction);


	for(unsigned int i = 0; i < l_BasicBlocks.size(); i++)
	{
		BasicBlock* l_pBlock = l_BasicBlocks[i];

		printf("Block at %X\n", l_pBlock->getInstructions()->front()->getAddress());

		for(unsigned int j = 0; j < l_pBlock->getSuccessors()->size(); j ++)
		{
			printf("\t- Block at %X\n", l_pBlock->getSuccessors()->at(j)->getInstructions()->front()->getAddress());
		}
	}







	if((*l_pFunction->begin())->getName() != "enter" && l_pFunction->back()->getName() != "ret")
	{
		// we can't compute the function
		return false;
	}



	return true;
}

bool Decompiler::ValidateBinary()
{
	unsigned int	l_uiByteCodeLength;
	unsigned int	l_uiBytecodeAddr;
	int				l_iBytecodeLength;

	l_uiByteCodeLength = m_pScrHeader->getByteCodeLength();

	// if the first opcode isn't a function enter, then the code is invalid
	if(m_aByteCode[0][0] != 45)
	{
		return false;
	}

	for(l_uiBytecodeAddr = 0; l_uiBytecodeAddr < l_uiByteCodeLength;)
	{
		l_iBytecodeLength = getOpcodeLenByAddr(l_uiBytecodeAddr);
		if(l_iBytecodeLength == 0)
		{
			printf("0 length opcode %d !\n", m_aByteCode[l_uiBytecodeAddr / 0x4000][l_uiBytecodeAddr % 0x4000]);
			printf("Addr %d code size %d\n", l_uiBytecodeAddr, l_uiByteCodeLength);
			return false;
		}
		l_uiBytecodeAddr += l_iBytecodeLength;
	}

	return l_uiBytecodeAddr == l_uiByteCodeLength;
}

int Decompiler::getOpcodeLen(unsigned char a_bOpcode)
{
	int l_aOpcodeLen[256] =	
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	// 0                  
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	// 12h
		1, 2, 3, 4, 5, 5, 1, 1, 4, 0, 3, 1, 1, 1, 1, 1, 2, 2,	// 24h
		2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 3, 3, 3, 3, 3,	// 36h
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,	// 48h
		3, 3, 3, 4, 4, 4, 4, 4, 0, 1, 1, 2, 2, 2, 2, 1, 1, 1,	// 5Ah
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	// 6Ch
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 7Eh
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 90h
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0A2h
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0B4h
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0C6h
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0D8h
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0EAh
		0, 0, 0, 0};
	return l_aOpcodeLen[a_bOpcode];
}

int Decompiler::getOpcodeLenByAddr(unsigned int a_uiAddress)
{
	unsigned char l_bOpcode;

	l_bOpcode = m_aByteCode[a_uiAddress / 0x4000][a_uiAddress % 0x4000];

	if(l_bOpcode == 45)
	{
		return m_aByteCode[(a_uiAddress + 4) / 0x4000][(a_uiAddress + 4) % 0x4000] + 5;
	}
	else if(l_bOpcode == 98)
	{
		return 6 * m_aByteCode[(a_uiAddress + 1) / 0x4000][(a_uiAddress + 1) % 0x4000] + 2;
	}
	
	return getOpcodeLen(l_bOpcode);
}