#include "main.h"



Disassembler::Disassembler()
{
	m_aByteCode = 0;
}

Disassembler::~Disassembler()
{
	// TODO: Free 'm_aByteCode'
}


bool Disassembler::DisassembleFile(char* a_szBinaryPath, char* a_szOutputPath)
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
	
	m_yscHeader.ReadFromFile(m_pBinaryFile);
		
	printf("Disassembling script \"%s\" ...\n", m_yscHeader.getName().c_str());

	ReadByteCode();

	if(ValidateBinary())
	{
		m_stringCollector.importFromBinary(&this->m_yscHeader, this->m_pBinaryFile);
		ConvertToInstructions();
		LinkStringPushes();
		PrintInstructionsToFile(a_szOutputPath);
		printf("Script disassembled !\n");
	}
	else
	{
		printf("Unable to validate the binary !\n");
	}
	delete m_pBinaryFile;
	m_pBinaryFile = 0;
	return true;
}

void Disassembler::ReadByteCode()
{
	int i;

	m_aByteCode = new unsigned char*[m_yscHeader.getCodePageCount()];

	for(i = 0; i < m_yscHeader.getCodePageCount(); i++)
	{
		m_aByteCode[i] = new unsigned char[m_yscHeader.getCodePageLength(i)];
		this->m_pBinaryFile->seekg(m_yscHeader.getCodePageOffset(this->m_pBinaryFile, i));
		this->m_pBinaryFile->read((char*)m_aByteCode[i], m_yscHeader.getCodePageLength(i));
	}
}

void Disassembler::ConvertToInstructions()
{
	int				l_uiByteCodeLength;
	int				l_uiBytecodeAddr;
	unsigned char	l_bOpcode;
	Instruction*	l_pInstruction;

	l_uiByteCodeLength = m_yscHeader.getByteCodeLength();

	m_Instructions.clear();


	for(l_uiBytecodeAddr = 0; l_uiBytecodeAddr < l_uiByteCodeLength;)
	{
		l_bOpcode = m_aByteCode[l_uiBytecodeAddr / 0x4000][l_uiBytecodeAddr % 0x4000];
	
		l_pInstruction = Instruction::allocFromOpcode(l_bOpcode);

		if(l_pInstruction)
		{
			l_pInstruction->Process(&m_aByteCode[l_uiBytecodeAddr / 0x4000][l_uiBytecodeAddr % 0x4000]);
			this->m_Instructions.push_back(l_pInstruction);
		}
		else
		{
			printf("Disassembler::ConvertToInstructions() -> Unk opcode %d\n", l_bOpcode);
		}

		l_uiBytecodeAddr += getOpcodeLenByAddr(l_uiBytecodeAddr);
	}
}

void Disassembler::LinkStringPushes()
{
	std::vector<Instruction*>::iterator it;

	for(it = this->m_Instructions.begin(); it != this->m_Instructions.end(); it++)
	{
		if((*it)->getOpcode() == 99)
		{
			std::vector<Instruction*>::iterator previous_it = it - 1;
			while((*previous_it)->getName() != "ipush") previous_it--;

			*previous_it; // this is the id of the string we need to push
		}		
	}

}

void Disassembler::PrintInstructionsToFile(char* a_szOutputPath)
{
	std::vector<Instruction*>::iterator it;
	std::ofstream l_OutputFile(a_szOutputPath);

	for(it = this->m_Instructions.begin(); it != this->m_Instructions.end(); it++)
	{
		std::string l_AssemblyLine = (*it)->toString() + "\n";
		l_OutputFile.write(l_AssemblyLine.c_str(), l_AssemblyLine.length());
	}


	l_OutputFile.close();
}


bool Disassembler::ValidateBinary()
{
	unsigned int	l_uiByteCodeLength;
	unsigned int	l_uiBytecodeAddr;
	int				l_iBytecodeLength;

	l_uiByteCodeLength = m_yscHeader.getByteCodeLength();

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

int Disassembler::getOpcodeLen(unsigned char a_bOpcode)
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

int Disassembler::getOpcodeLenByAddr(unsigned int a_uiAddress)
{
	unsigned char l_bOpcode;

	l_bOpcode = m_aByteCode[a_uiAddress / 0x4000][a_uiAddress % 0x4000];

	if(l_bOpcode == 45)
	{
		return m_aByteCode[(a_uiAddress + 4) / 0x4000][(a_uiAddress + 4) % 0x4000] + 5;
	}
	else if(l_bOpcode == 98)
	{
		return 6 * m_aByteCode[(a_uiAddress + 4) / 0x4000][(a_uiAddress + 4) % 0x4000] + 2;
	}
	
	return getOpcodeLen(l_bOpcode);
}