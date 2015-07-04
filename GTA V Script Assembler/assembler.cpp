#include "main.h"




Assembler::Assembler()
{
	m_AssemblyLines.clear();
	m_pAssemblyFile = nullptr;
}

Assembler::~Assembler()
{

}


bool Assembler::AssembleFile(char* a_szAssemblyPath, char* a_szOuputScriptPath)
{
	m_pAssemblyFile = new std::ifstream(a_szAssemblyPath, std::ifstream::in);


	CollectCode();
	CleanCode();
	if(ParseCode())
	{
	}


	m_AssemblyLines.clear();
	m_pAssemblyFile->close();

	delete m_pAssemblyFile;
	return false;
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

		l_iPos = it->second.find_first_of(" ");

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
		}
		else if(l_szOperation == "nop")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(0);
		}
		else if(l_szOperation == "addi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(1);
		}
		else if(l_szOperation == "subi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(2);
		}
		else if(l_szOperation == "muli")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(3);
		}
		else if(l_szOperation == "divi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(4);
		}
		else if(l_szOperation == "modi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(5);
		}		
		else if(l_szOperation == "noti")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(6);
		}
		else if(l_szOperation == "negi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(7);
		}		
		else if(l_szOperation == "icmpeq")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(8);
		}
		else if(l_szOperation == "icmpne")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(9);
		}
		else if(l_szOperation == "icmpgt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(10);
		}
		else if(l_szOperation == "icmpge")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(11);
		}
		else if(l_szOperation == "icmplt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(12);
		}
		else if(l_szOperation == "icmple")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(13);
		}
		else if(l_szOperation == "fadd")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(14);
		}
		else if(l_szOperation == "fsub")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(15);
		}
		else if(l_szOperation == "fmul")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(16);
		}
		else if(l_szOperation == "fdiv")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(17);
		}
		else if(l_szOperation == "fmod")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(18);
		}		
		else if(l_szOperation == "fneg")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(19);
		}
		else if(l_szOperation == "fcmpeq")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(20);
		}
		else if(l_szOperation == "fcmpne")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(21);
		}
		else if(l_szOperation == "fcmpgt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(22);
		}
		else if(l_szOperation == "fcmpge")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(23);
		}
		else if(l_szOperation == "fcmplt")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(24);
		}
		else if(l_szOperation == "fcmple")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(25);
		}
		else if(l_szOperation == "vadd")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(26);
		}
		else if(l_szOperation == "vsub")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(27);
		}
		else if(l_szOperation == "vmul")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(28);
		}
		else if(l_szOperation == "vdiv")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(29);
		}
		else if(l_szOperation == "vneg")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(30);
		}
		else if(l_szOperation == "iand")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(31);
		}
		else if(l_szOperation == "ior")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(32);
		}
		else if(l_szOperation == "ixor")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(33);
		}
		else if(l_szOperation == "itof")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(34);
		}
		else if(l_szOperation == "ftoi")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(35);
		}
		else if(l_szOperation == "dup2")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(36);
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
		}
		else if(l_szOperation == "drop")
		{
			l_pInstruction = new InstructionBasic();
			l_pInstruction->setOpcode(43);
		}
		else if(l_szOperation == "native")
		{
			printf("NOT SUPPORTED YET !!\n");
		}
		else
		{
			//printf("Line %d : Unknown operation \"%s\". !\n", it->first, l_szOperation.c_str());
			//return false;
		}

		if(l_pInstruction)
		{
			l_pInstruction->Process(l_szOperand);

			this->m_ByteCode.insert(std::pair<unsigned int, Instruction*>(l_uiAddress, l_pInstruction));
			l_uiAddress += l_pInstruction->getLength();
		}
		else
		{
			printf("-> %s\n", l_szOperation.c_str());
		}

	}

	return true;
}