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
	// TODO: Print the global id
	return getName() + "";
}