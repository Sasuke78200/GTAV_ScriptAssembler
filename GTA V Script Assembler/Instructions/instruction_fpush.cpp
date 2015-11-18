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

float InstructionfPush::getValue()
{
	if(getOpcode() >= 118 && getOpcode() <= 126)
	{
		return (float)(getOpcode() - 119);
	}
	return *(float*)&m_aByteCode[1];
}

std::string InstructionfPush::toString()
{
	std::stringstream l_ss;
	// TODO: Print the value

	l_ss << getName() << " " << getValue();

	return l_ss.str();
}

bool InstructionfPush::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);

	if(*a_aByteCode == 41)
	{
		setLength(5);
	}
	else
	{
		setLength(1);
	}

	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}