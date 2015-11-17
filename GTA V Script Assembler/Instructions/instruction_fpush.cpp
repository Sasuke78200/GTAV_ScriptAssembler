#include "../main.h"

InstructionfPush::InstructionfPush()
{
	setOpcode(-1);
	setLength(-1);
	setName("fpush");
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
	setValue(l_fValue);
	
	return true;
}
// need to be checked !
#define is_float_integer(f) (((int)f) == f)


void InstructionfPush::setValue(float a_fValue)
{
	if(is_float_integer(a_fValue) && a_fValue >= -1.0 && a_fValue <= 7.0)
	{
		setOpcode(119 + (int)a_fValue);
		setLength(1);
	}
	else
	{
		*(int*)&m_aByteCode[1] = *(int*)&a_fValue;
		setOpcode(41);
		setLength(5);
	}
}

std::string InstructionfPush::toString()
{
	// TODO: Print the value
	return getName() + "";
}