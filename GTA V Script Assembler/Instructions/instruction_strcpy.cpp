#include "../main.h"




InstructionStrcpy::InstructionStrcpy()
{
	setOpcode(101);
	setLength(2);
	setName("strcpy");
}

InstructionStrcpy::~InstructionStrcpy()
{
}

unsigned char* InstructionStrcpy::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionStrcpy::Process(std::string a_szAssemblyLine)
{
	int l_iLen;
	if(a_szAssemblyLine.length() == 0) return false;

	l_iLen = atoi(a_szAssemblyLine.c_str());

	if((l_iLen > 0 && l_iLen < 256) == false)
	{
		return false;
	}

	m_aByteCode[1] = l_iLen;
	return true;
}

std::string InstructionStrcpy::toString()
{
	std::stringstream l_ss;
	l_ss << getName() << " " << (int)m_aByteCode[1];
	return l_ss.str();
}

bool InstructionStrcpy::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}