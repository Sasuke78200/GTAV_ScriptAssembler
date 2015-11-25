#include "main.h"




Assembler::Assembler()
{
	m_AssemblyLines.clear();
	m_pAssemblyFile = nullptr;
	m_szScriptName = "ScriptName";
}

Assembler::~Assembler()
{

}

bool Assembler::AssembleFile(char* a_szAssemblyPath, char* a_szOuputScriptPath, char* a_szScriptName)
{
	std::ofstream l_BinaryFile(a_szOuputScriptPath, std::ofstream::binary | std::ofstream::out);
	m_pAssemblyFile = new std::ifstream(a_szAssemblyPath, std::ifstream::in);


	m_szScriptName = a_szScriptName;

	CollectCode();
	CleanCode();

	if(ParseCode())
	{
		CollectNativesHashes();
		ConstructBinary(&l_BinaryFile);
	}
	else
	{
		printf("Couldn't parse code !\n");
	}

	
	m_AssemblyLines.clear();
	m_pAssemblyFile->close();
	l_BinaryFile.close();

	delete m_pAssemblyFile;
	return false;
}

void Assembler::ConstructBinary(std::ofstream* a_pBinaryStream)
{
	YscHeader			l_yscHeader;
	unsigned int		l_uiByteCodeLength;
	unsigned char**		l_pByteCode;
	int					l_iPageCount;
	int					l_iPageLen;
	int					i;

	// construct byte code pages

	// get byte code length
	l_uiByteCodeLength = 0;
	for(std::vector<Instruction*>::iterator it = this->m_ByteCode.begin(); it != this->m_ByteCode.end(); it++)
	{
		l_uiByteCodeLength += (*it)->getLength();
	}

	l_yscHeader.setByteCodeLength(l_uiByteCodeLength);

	// alloc the bytecode pages
	l_iPageCount = l_uiByteCodeLength / 0x4000 + 1;
	l_pByteCode = new unsigned char*[l_iPageCount];

	for(i = 0; i < l_iPageCount; i++)
	{
		l_iPageLen =  0x4000;
		//if(i == l_iPageCount - 1) l_iPageLen = l_uiByteCodeLength % 0x4000;
		l_pByteCode[i] = new unsigned char[l_iPageLen];
		memset(l_pByteCode[i], 0, l_iPageLen);
	}

	// fill them with the code
	for(std::vector<Instruction*>::iterator it = this->m_ByteCode.begin(); it != this->m_ByteCode.end(); it++)
	{	
		memcpy(&l_pByteCode[(*it)->getAddress() / 0x4000][(*it)->getAddress() % 0x4000], (*it)->getByteCode(), (*it)->getLength());
	}

	l_yscHeader.setScriptName(m_szScriptName);
	// todo: string collector !
	l_yscHeader.WriteToFile(a_pBinaryStream, 
		l_pByteCode, 
		&this->m_NativeCollector,
		&this->m_StringCollector);
}

void Assembler::CollectCode()
{
	std::string l_szLine;
	int l_iLine = 1;


	m_pAssemblyFile->seekg(0);

	// read all the lines and put them on the vector that will contains the code
	while(std::getline(*m_pAssemblyFile, l_szLine))
	{
		this->m_AssemblyLines.insert(std::pair<int, std::string>(l_iLine, l_szLine));
		l_iLine++;
	}


	// process each line code, to remove comments, unused spaces, etc ...
	for(std::map<unsigned int, std::string>::iterator it = this->m_AssemblyLines.begin(); it != this->m_AssemblyLines.end(); it++)
	{
		int l_iPos;

		it->second = trim(it->second);

		// remove the comments.
		l_iPos = 0;

		while((l_iPos = it->second.find_first_of("//", l_iPos)) != std::string::npos)
		{			
			if(isInsideAString(it->second, l_iPos) == false)
			{
				if(l_iPos == 0)
				{
					it->second = "";
					break;
				}
				else
				{
					it->second = it->second.substr(0, l_iPos - 1);
				}
			}
			l_iPos += 2;
		}
		it->second = trim(it->second);
	}
}

void Assembler::CleanCode()
{
	std::map<unsigned int, std::string>::iterator it;

	it = this->m_AssemblyLines.begin();
	do
	{
		if(it->second.length() == 0)
		{
			this->m_AssemblyLines.erase(it++);
		}
		else
		{
			it++;
		}
	}
	while(it != this->m_AssemblyLines.end());
}

bool Assembler::CollectNativesHashes()
{
	int l_iNativeVersion;

	// link native name with their hashes
	this->m_NativeCollector.retrieveHashes();

	// Get which version of natives hashes we want to use
	l_iNativeVersion = atoi(CommandLine::Instance()->getVal("-v")->c_str());

	if(l_iNativeVersion > 0)
	{
		//m_NativeCollector.TranslateHash(0);		// 335 -> 350
		//m_NativeCollector.TranslateHash(1);		// 350 -> 372
		//m_NativeCollector.TranslateHash(2);		// 372 -> 393
		m_NativeCollector.translateHash(l_iNativeVersion - 1);
	}
	return true;
}

bool Assembler::ParseCode()
{
	unsigned int l_uiAddress;
	std::map<unsigned int, std::string>::iterator it;


	l_uiAddress = 0;

	for(it = this->m_AssemblyLines.begin(); it != this->m_AssemblyLines.end(); it++)
	{
		Instruction*	l_pInstruction;
		std::string		l_szOperation;
		std::string		l_szOperand;
		int				l_iPos;

		l_iPos = it->second.find_first_of(ASSEMBLY_SPACE);

		

		if(l_iPos != std::string::npos)
		{
			l_szOperation	= it->second.substr(0, l_iPos);
			l_szOperand		= it->second.substr(l_iPos + 1);
			l_szOperand		= trim(l_szOperand);
		}
		else
		{
			l_szOperation	= it->second;
			l_szOperand		= "";
		}


		std::transform(l_szOperation.begin(), l_szOperation.end(), l_szOperation.begin(), ::tolower); // lower the operation name

		l_pInstruction = 0;

		if(l_szOperation[0] == ':') // we got a label there
		{
			if(m_LabelCollector.AddLabel(l_szOperation.substr(1), l_uiAddress) == false)
			{
				printf("Unable to collect label '%s'.\n", l_szOperation.substr(1).c_str());
				return false;
			}
		}
		else if(l_szOperation == "nop")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(0);
			l_pInstruction->setName("nop");
		}
		else if(l_szOperation == "addi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(1);
			l_pInstruction->setName("addi");
		}
		else if(l_szOperation == "subi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(2);
			l_pInstruction->setName("subi");
		}
		else if(l_szOperation == "muli")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(3);
			l_pInstruction->setName("muli");
		}
		else if(l_szOperation == "divi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(4);
			l_pInstruction->setName("divi");
		}
		else if(l_szOperation == "modi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(5);
			l_pInstruction->setName("modi");
		}		
		else if(l_szOperation == "noti")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(6);
			l_pInstruction->setName("noti");
		}
		else if(l_szOperation == "negi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(7);
			l_pInstruction->setName("negi");
		}		
		else if(l_szOperation == "icmpeq")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(8);
			l_pInstruction->setName("icmpeq");
		}
		else if(l_szOperation == "icmpne")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(9);
			l_pInstruction->setName("icmpne");
		}
		else if(l_szOperation == "icmpgt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(10);
			l_pInstruction->setName("icmpgt");
		}
		else if(l_szOperation == "icmpge")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(11);
			l_pInstruction->setName("icmpge");
		}
		else if(l_szOperation == "icmplt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(12);
			l_pInstruction->setName("icmplt");
		}
		else if(l_szOperation == "icmple")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(13);
			l_pInstruction->setName("icmple");
		}
		else if(l_szOperation == "fadd")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(14);
			l_pInstruction->setName("fadd");
		}
		else if(l_szOperation == "fsub")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(15);
			l_pInstruction->setName("fsub");
		}
		else if(l_szOperation == "fmul")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(16);
			l_pInstruction->setName("fmul");
		}
		else if(l_szOperation == "fdiv")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(17);
			l_pInstruction->setName("fdiv");
		}
		else if(l_szOperation == "fmod")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(18);
			l_pInstruction->setName("fmod");
		}		
		else if(l_szOperation == "fneg")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(19);
			l_pInstruction->setName("fneg");
		}
		else if(l_szOperation == "fcmpeq")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(20);
			l_pInstruction->setName("fcmpeq");
		}
		else if(l_szOperation == "fcmpne")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(21);
			l_pInstruction->setName("fcmpne");
		}
		else if(l_szOperation == "fcmpgt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(22);
			l_pInstruction->setName("fcmpgt");
		}
		else if(l_szOperation == "fcmpge")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(23);
			l_pInstruction->setName("fcmpge");
		}
		else if(l_szOperation == "fcmplt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(24);
			l_pInstruction->setName("fcmplt");
		}
		else if(l_szOperation == "fcmple")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(25);
			l_pInstruction->setName("fcmple");
		}
		else if(l_szOperation == "vadd")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(26);
			l_pInstruction->setName("vadd");
		}
		else if(l_szOperation == "vsub")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(27);
			l_pInstruction->setName("vsub");
		}
		else if(l_szOperation == "vmul")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(28);
			l_pInstruction->setName("vmul");
		}
		else if(l_szOperation == "vdiv")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(29);
			l_pInstruction->setName("vdiv");
		}
		else if(l_szOperation == "vneg")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(30);
			l_pInstruction->setName("vneg");
		}
		else if(l_szOperation == "iand")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(31);
			l_pInstruction->setName("iand");
		}
		else if(l_szOperation == "ior")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(32);
			l_pInstruction->setName("ior");
		}
		else if(l_szOperation == "ixor")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(33);
			l_pInstruction->setName("ixor");
		}
		else if(l_szOperation == "itof")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(34);
			l_pInstruction->setName("itof");
		}
		else if(l_szOperation == "ftoi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(35);
			l_pInstruction->setName("ftoi");
		}
		else if(l_szOperation == "dup2")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(36);
			l_pInstruction->setName("dup2");
		}
		else if(l_szOperation == "ipush")
		{
			l_pInstruction = new InstructioniPush();
		}
		else if(l_szOperation == "fpush")
		{
			l_pInstruction = new InstructionfPush();
		}
		else if(l_szOperation == "dup")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(42);
			l_pInstruction->setName("dup");
		}
		else if(l_szOperation == "drop")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(43);
			l_pInstruction->setName("drop");
		}
		else if(l_szOperation == "native")
		{
			l_pInstruction = new InstructionNative();
			((InstructionNative*)l_pInstruction)->setNativeCollector(&this->m_NativeCollector);
		}
		else if(l_szOperation == "enter")		
		{
			l_pInstruction = new InstructionEnter();
		}
		else if(l_szOperation == "ret")		
		{
			l_pInstruction = new InstructionRet();
		}
		else if(l_szOperation == "pget")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(47);
			l_pInstruction->setName("pget");
		}
		else if(l_szOperation == "pset")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(48);
			l_pInstruction->setName("pset");
		}
		else if(l_szOperation == "ppeekset")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(49);
			l_pInstruction->setName("ppeekset");
		}
		else if(l_szOperation == "explode")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setName("explode");
			l_pInstruction->setOpcode(50);
		}
		else if(l_szOperation == "implode")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setName("implode");
			l_pInstruction->setOpcode(51);
		}
		else if(l_szOperation == "jmp")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(85);
			l_pInstruction->setName("jmp");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}		
		else if(l_szOperation == "jmpf")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(86);
			l_pInstruction->setName("jmpf");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
			
		}		
		else if(l_szOperation == "jmpneq")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(87);
			l_pInstruction->setName("jmpneq");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}
		else if(l_szOperation == "jmpeq")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(88);
			l_pInstruction->setName("jmpeq");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}
		else if(l_szOperation == "jmpgt")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(89);
			l_pInstruction->setName("jmpgt");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}
		else if(l_szOperation == "jmpge")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(90);
			l_pInstruction->setName("jmpge");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}
		else if(l_szOperation == "jmplt")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(91);
			l_pInstruction->setName("jmplt");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}
		else if(l_szOperation == "jmple")
		{
			l_pInstruction = new InstructionJmp();
			l_pInstruction->setOpcode(92);
			l_pInstruction->setName("jmple");
			((InstructionJmp*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);			
		}
		else if(l_szOperation == "spush")
		{
			l_pInstruction = new InstructionsPush();
			((InstructionsPush*)l_pInstruction)->setStringCollector(&this->m_StringCollector);
		}
		else if(l_szOperation == "call")
		{
			l_pInstruction = new InstructionCall();
			((InstructionCall*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
		}
		else if(l_szOperation == "getframep")
		{
			l_pInstruction = new InstructionGetFrameP();
		}
		else if(l_szOperation == "getframe")
		{
			l_pInstruction = new InstructionGetFrame();
		}
		else if(l_szOperation == "setframe")
		{
			l_pInstruction = new InstructionSetFrame();
		}
		else if(l_szOperation == "getglobalp")
		{
			l_pInstruction = new InstructionGetGlobalP();
		}
		else if(l_szOperation == "getglobal")
		{
			l_pInstruction = new InstructionGetGlobal();
		}
		else if(l_szOperation == "setglobal")
		{
			l_pInstruction = new InstructionSetGlobal();
		}
		else if(l_szOperation == "jooat")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(100);
			l_pInstruction->setName("jooat");
		}
		else if(l_szOperation == "strcpy")
		{
			l_pInstruction = new InstructionStrcpy();
		}
		else if(l_szOperation == "getstatic")
		{
			l_pInstruction = new InstructionGetStatic();
		}
		else if(l_szOperation == "getstaticp")
		{
			l_pInstruction = new InstructionGetStaticP();
		}
		else if(l_szOperation == "setstatic")
		{
			l_pInstruction = new InstructionSetStatic();
		}
		else if(l_szOperation == "switch")
		{
			l_pInstruction = new InstructionSwitch();
			((InstructionSwitch*)l_pInstruction)->setLabelCollector(&this->m_LabelCollector);
		}
		else if(l_szOperation == "arraygetp")
		{
			l_pInstruction = new InstructionArrayGetP();
		}
		else if(l_szOperation == "arrayget")
		{
			l_pInstruction = new InstructionArrayGet();
		}
		else if(l_szOperation == "arrayset")
		{
			l_pInstruction = new InstructionArraySet();
		}
		else if(l_szOperation == "pgetimm")
		{
			l_pInstruction = new InstructionPGetImm();
		}
		else if(l_szOperation == "psetimm")
		{
			l_pInstruction = new InstructionPSetImm();
		}
		else if(l_szOperation == "padd")
		{
			l_pInstruction = new InstructionPAdd();
		}
		else if(l_szOperation == "addimm")
		{
			l_pInstruction = new InstructionAddImm();
		}
		else if(l_szOperation == "mulimm")
		{
			l_pInstruction = new InstructionMulImm();
		}
		else
		{
			printf("Line %d : Unknown operation \"%s\". !\n", it->first, l_szOperation.c_str());
			return false;
		}

		if(l_pInstruction)
		{
			if(l_pInstruction->Process(l_szOperand))
			{
				l_pInstruction->setAddress(l_uiAddress);
				this->m_ByteCode.push_back(l_pInstruction);
				l_uiAddress += l_pInstruction->getLength();
			}
			else
			{
				printf("Error line %d : -> %s\n", it->first, it->second.c_str());
				return false;
			}
		}
		else
		{
			if(l_szOperation[0] != ':')
				printf("-> %d: %s\n", it->first, l_szOperation.c_str());
		}
	}
	return true;
}
