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
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionGetGlobalP::Process(std::string a_szAssemblyLine)
{
	int l_iGlobalId;

	if(a_szAssemblyLine.length() == 0) return false;

	l_iGlobalId = atoi(a_szAssemblyLine.c_str());

	if((l_iGlobalId & 0xFFFF) == l_iGlobalId)
	{
		*(short*)&m_aByteCode[1] = l_iGlobalId;
		setOpcode(82);
		setLength(3);
	}
	else
	{
		*(int*)m_aByteCode = _byteswap_ulong(l_iGlobalId);	
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
		l_ss << *(short*)&m_aByteCode[1];
	}
	else
	{
		l_ss << *(int*)m_aByteCode;
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