#include "../main.h"

InstructionfPush::InstructionfPush()
{
	setOpcode(41);
	setLength(5);
}

InstructionfPush::~InstructionfPush()
{

}

unsigned char* InstructionfPush::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionfPush::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return 0;


	float l_fValue = (float) atof(a_szAssemblyLine.c_str());
	*(int*)&m_aByteCode[1] = *(int*)&l_fValue;
	return true;
}
