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
		printf("Binary validated !\n");
		ConvertToInstructions();
		PrintInstructionsToFile(a_szOutputPath);
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

		/*

		if(l_bOpcode == 0)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setName("nop");
		}
		else if(l_bOpcode == 1)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(1);
			l_pInstruction->setName("addi");
		}
		else if(l_bOpcode == 2)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(2);
			l_pInstruction->setName("subi");
		}
		else if(l_bOpcode == 3)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(3);
			l_pInstruction->setName("muli");
		}
		else if(l_bOpcode == 4)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(4);
			l_pInstruction->setName("divi");
		}
		else if(l_bOpcode == 5)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(5);
			l_pInstruction->setName("modi");
		}		
		else if(l_bOpcode == 6)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(6);
			l_pInstruction->setName("noti");
		}
		else if(l_bOpcode == 7)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(7);
			l_pInstruction->setName("negi");
		}		
		else if(l_bOpcode == 8)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(8);
			l_pInstruction->setName("icmpeq");
		}
		else if(l_bOpcode == 9)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(9);
			l_pInstruction->setName("icmpne");
		}
		else if(l_bOpcode == 10)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(10);
			l_pInstruction->setName("icmpgt");
		}
		else if(l_bOpcode == 11)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(11);
			l_pInstruction->setName("icmpge");
		}
		else if(l_bOpcode == 12)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(12);
			l_pInstruction->setName("icmplt");
		}
		else if(l_bOpcode == 13)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(13);
			l_pInstruction->setName("icmple");
		}
		else if(l_bOpcode == 14)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(14);
			l_pInstruction->setName("fadd");
		}
		else if(l_bOpcode == 15)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(15);
			l_pInstruction->setName("fsub");
		}
		else if(l_bOpcode == 16)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(16);
			l_pInstruction->setName("fmul");
		}
		else if(l_bOpcode == 17)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(17);
			l_pInstruction->setName("fdiv");
		}
		else if(l_bOpcode == 18)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(18);
			l_pInstruction->setName("fmod");
		}		
		else if(l_bOpcode == 19)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(19);
			l_pInstruction->setName("fneg");
		}
		else if(l_bOpcode == 20)
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(20);
			l_pInstruction->setName("fcmpeq");
		}
		else if(l_bOpcode == "fcmpne")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(21);
			l_pInstruction->setName("fcmpne");
		}
		else if(l_bOpcode == "fcmpgt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(22);
			l_pInstruction->setName("fcmpgt");
		}
		else if(l_bOpcode == "fcmpge")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(23);
			l_pInstruction->setName("fcmpge");
		}
		else if(l_bOpcode == "fcmplt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(24);
			l_pInstruction->setName("fcmplt");
		}
		else if(l_bOpcode == "fcmple")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(25);
			l_pInstruction->setName("fcmple");
		}
		else if(l_bOpcode == "vadd")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(26);
			l_pInstruction->setName("vadd");
		}
		else if(l_bOpcode == "vsub")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(27);
			l_pInstruction->setName("vsub");
		}
		else if(l_bOpcode == "vmul")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(28);
			l_pInstruction->setName("vmul");
		}
		else if(l_bOpcode == "vdiv")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(29);
			l_pInstruction->setName("vdiv");
		}
		else if(l_bOpcode == "vneg")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(30);
			l_pInstruction->setName("vneg");
		}
		else if(l_bOpcode == "iand")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(31);
			l_pInstruction->setName("iand");
		}
		else if(l_bOpcode == "ior")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(32);
			l_pInstruction->setName("ior");
		}
		else if(l_bOpcode == "ixor")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(33);
			l_pInstruction->setName("ixor");
		}
		else if(l_bOpcode == "itof")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(34);
			l_pInstruction->setName("itof");
		}
		else if(l_bOpcode == "ftoi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(35);
			l_pInstruction->setName("ftoi");
		}
		else if(l_bOpcode == "dup2")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(36);
			l_pInstruction->setName("dup2");
		}

		else if(l_bOpcode == "dup")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(42);
			l_pInstruction->setName("dup");
		}
		else if(l_bOpcode == "drop")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(43);
			l_pInstruction->setName("drop");
		}

		else if(l_bOpcode == "pget")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(47);
			l_pInstruction->setName("pget");
		}
		else if(l_bOpcode == "pset")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(48);
			l_pInstruction->setName("pset");
		}
		else if(l_bOpcode == "ppeekset")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(49);
			l_pInstruction->setName("ppeekset");
		}
		else if(l_bOpcode == "jmp")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(85);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmp");
		}		
		else if(l_bOpcode == "jmpf")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(86);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmpf");
		}		
		else if(l_bOpcode == "jmpneq")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(87);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmpneq");
		}
		else if(l_bOpcode == "jmpeq")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(88);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmpeq");
		}
		else if(l_bOpcode == "jmpgt")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(89);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmpgt");
		}
		else if(l_bOpcode == "jmpge")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(90);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmpge");
		}
		else if(l_bOpcode == "jmplt")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(91);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmplt");
		}
		else if(l_bOpcode == "jmple")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(92);
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			((InstructionJmp*)l_pInstruction)->setAddress(l_uiAddress);
			((InstructionJmp*)l_pInstruction)->setName("jmple");
		}

		else if(l_bOpcode == "jooat")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(100);
			l_pInstruction->setName("jooat");
		}
		*/

		l_pInstruction = Instruction::allocFromOpcode(l_bOpcode);

		if(l_pInstruction)
		{
			l_pInstruction->Process(&m_aByteCode[l_uiBytecodeAddr / 0x4000][l_uiBytecodeAddr % 0x4000]);
			this->m_Instructions.push_back(l_pInstruction);
		}

		l_uiBytecodeAddr += getOpcodeLenByAddr(l_uiBytecodeAddr);
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
	int l_uiByteCodeLength;
	int l_uiBytecodeAddr;

	l_uiByteCodeLength = m_yscHeader.getByteCodeLength();

	// if the first opcode isn't a function enter, then the code is invalid
	if(m_aByteCode[0][0] != 45)
	{
		return false;
	}

	for(l_uiBytecodeAddr = 0; l_uiBytecodeAddr < l_uiByteCodeLength;)
	{
		l_uiBytecodeAddr += getOpcodeLenByAddr(l_uiBytecodeAddr);
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