#include "main.h"


using namespace rapidjson;



void ShowHelp()
{
	printf("USAGE : -i input_path -o output_path [-n]\n\n");
	printf("Options :\n");
	printf("    -n\t\tInternal script name, by default it takes the output file name\n");
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


int main(int a_iArgCount, char** a_pszArgs)
{
	CommandLine		l_CommandLine(a_iArgCount, a_pszArgs);
	Assembler		l_Assembler;
	std::string*	l_pInput;
	std::string*	l_pOutput;
	std::string*	l_pScriptName;


	if(a_iArgCount == 1)
	{
		ShowHelp();
	}
	else
	{
		if((l_pInput = l_CommandLine.getVal("-i")) == 0)
		{
			ShowHelp();
			return 0;
		}

		if((l_pOutput = l_CommandLine.getVal("-o")) == 0)
		{
			ShowHelp();
			return 0;
		}

		if((l_pScriptName = l_CommandLine.getVal("-n")) == 0)
		{
			// process the script name
			l_pScriptName = new std::string(GetFileNameFromPath(*l_pOutput));
		}
		else
		{
			*l_pScriptName = l_pScriptName->substr(0, 64);
		}

		l_Assembler.AssembleFile((char*)l_pInput->c_str(), (char*)l_pOutput->c_str(), (char*)l_pScriptName->c_str());
	}

#ifdef _DEBUG
	system("pause");
#endif


	return 0;
}