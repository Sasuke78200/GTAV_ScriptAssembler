#include "main.h"


using namespace rapidjson;


int main(int a_iArgCount, char** a_pszArgs)
{
	Assembler l_Assembler;
	// collect labels
	// collect strings
	// collect natives
	// parse code
	// construct byte code pages
	// construct string pages
	// construct natives pages
	// construct script header
	// write everything on a file


	l_Assembler.AssembleFile("C:/Users/Sasuke78200/Desktop/main.sc", "C:/Users/Sasuke78200/Desktop/main.ysc");
	system("pause");
	return 0;
}