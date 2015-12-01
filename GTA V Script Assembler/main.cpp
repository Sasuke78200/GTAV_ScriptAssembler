#include "main.h"


using namespace rapidjson;



void ShowHelp()
{
	printf("USAGE : -i input_path -o output_path [-n] [-v]\n\n");
	printf("Options :\n");
	printf("    -d\t\tDisassemble a compiled script\n");
	printf("    -hl\t\tDecompile a compiled script\n");
	printf("    -n\t\tInternal script name, by default it takes the output file name\n");
	printf("    -ns\t\tPrint natives namespace when disassembling.\n");
	printf("    -v\t\tNative functions hashes\n");
	printf("      \t\t0 for build 335\n");
	printf("      \t\t1 for build 350\n");
	printf("      \t\t2 for build 372\n");
	printf("      \t\t3 for build 393 (default)\n");
	printf("    -p\t\tScript plateform.\n");
	printf("      \t\t0 for PC (default)\n");
	printf("      \t\t1 for PS3\n");
	printf("\n");
}

std::string GetFileNameFromPath(std::string a_szPath)
{
	int l_iPosBegin, l_iPosEnd;

	l_iPosBegin = a_szPath.find_last_of("/\\");
	l_iPosEnd = a_szPath.find_last_of('.');


	if(l_iPosBegin == std::string::npos) l_iPosBegin = 0;
	else l_iPosBegin ++;
	if(l_iPosEnd == std::string::npos) l_iPosEnd = a_szPath.length();


	return a_szPath.substr(l_iPosBegin, l_iPosEnd - l_iPosBegin);
}

void ShowMissingInstructions()
{
	for(int i = 0; i < 127; i++)
	{
		Instruction* l = Instruction::allocFromOpcode(i);
		if(l)
			delete l;
		else
			printf("%d\n", i);
	}
}

int main(int a_iArgCount, char** a_pszArgs)
{
	CommandLine*	l_pCommandLine;	
	Assembler*		l_pAssembler;
	Disassembler*	l_pDisassembler;
	Decompiler*		l_pDecompiler;
	std::string*	l_pInput;
	std::string*	l_pOutput;
	std::string*	l_pScriptName;


	l_pCommandLine = CommandLine::Instance(a_iArgCount, a_pszArgs);
//	ShowMissingInstructions();

	if(a_iArgCount == 1)
	{
		ShowHelp();
	}
	else
	{
		if((l_pInput = l_pCommandLine->getVal("-i")) == 0)
		{
			ShowHelp();
			return 0;
		}

		if((l_pOutput = l_pCommandLine->getVal("-o")) == 0)
		{
			ShowHelp();
			return 0;
		}

		if((l_pScriptName = l_pCommandLine->getVal("-n")) == 0)
		{
			// process the script name
			l_pScriptName = new std::string(GetFileNameFromPath(*l_pOutput));
		}
		else
		{
			*l_pScriptName = l_pScriptName->substr(0, 64);
		}

		if(l_pCommandLine->getVal("-v") == 0)
		{
			l_pCommandLine->setVal("-v", "3");
		}

		if(l_pCommandLine->getVal("-p") == 0)
		{
			l_pCommandLine->setVal("-p", "0");
		}

		if(l_pCommandLine->getVal("-d") != 0)
		{
			l_pDisassembler = new Disassembler();
			l_pDisassembler->DisassembleFile((char*)l_pInput->c_str(), (char*)l_pOutput->c_str());
			delete l_pDisassembler;
		}
		else if(l_pCommandLine->getVal("-hl") != 0)
		{
			l_pDecompiler = new Decompiler();
			l_pDecompiler->DecompileFile((char*)l_pInput->c_str(), (char*)l_pOutput->c_str());
			delete l_pDecompiler;
		}
		else
		{
			l_pAssembler = new Assembler();
			l_pAssembler->AssembleFile((char*)l_pInput->c_str(), (char*)l_pOutput->c_str(), (char*)l_pScriptName->c_str());
			delete l_pAssembler;
		}
	}

#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}