#include "../main.h"


InstructionBasic::InstructionBasic()
{
	setOpcode(0);
	setLength(1);
}

InstructionBasic::~InstructionBasic()
{

}

unsigned char* InstructionBasic::getByteCode()
{
	return &l_byOpcode;
}

bool InstructionBasic::Process(std::string a_szAssemblyLine)
{
	return true;
}