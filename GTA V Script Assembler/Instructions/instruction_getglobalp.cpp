#include "../main.h"


InstructionGetGlobalP::InstructionGetGlobalP()
{
	setName("getglobalp");
}

InstructionGetGlobalP::~InstructionGetGlobalP()
{

}

unsigned char* InstructionGetGlobalP::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return m_aByteCode;
}

bool InstructionGetGlobalP::Process(std::string a_szAssemblyLine)
{
	int l_iGlobalId;

	if(a_szAssemblyLine.length() == 0) return false;

	l_iGlobalId = atoi(a_szAssemblyLine.c_str());

	if((l_iGlobalId & 0xFFFF) == l_iGlobalId)
	{
		*(unsigned short*)&m_aByteCode[1] = l_iGlobalId;
		setOpcode(82);
		setLength(3);
	}
	else
	{
		*(unsigned int*)m_aByteCode = l_iGlobalId << 8;	
		setOpcode(94);
		setLength(4);
	}
	return true;
}

std::string InstructionGetGlobalP::toString()
{
	std::stringstream l_ss;
	l_ss << getName() << " ";

	if(getOpcode() == 82)
	{
		l_ss << *(unsigned short*)&m_aByteCode[1];
	}
	else
	{
		l_ss << ((*(unsigned int*)m_aByteCode) >> 8);

	}

	return l_ss.str();
}

bool InstructionGetGlobalP::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	if(*a_aByteCode == 82)
	{
		setLength(3);
	}
	else
	{
		setLength(4);
	}
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}