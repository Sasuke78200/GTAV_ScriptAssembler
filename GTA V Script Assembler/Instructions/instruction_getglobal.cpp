#include "../main.h"


InstructionGetGlobal::InstructionGetGlobal()
{
	setName("getglobal");
}

InstructionGetGlobal::~InstructionGetGlobal()
{

}

unsigned char* InstructionGetGlobal::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionGetGlobal::Process(std::string a_szAssemblyLine)
{
	int l_iGlobalId;

	if(a_szAssemblyLine.length() == 0) return false;


	l_iGlobalId = atoi(a_szAssemblyLine.c_str());


	if((l_iGlobalId & 0xFFFF) == l_iGlobalId)
	{
		*(short*)&m_aByteCode[1] = l_iGlobalId;
		setOpcode(83);
		setLength(3);
	}
	else
	{
		*(int*)m_aByteCode = _byteswap_ulong(l_iGlobalId);	
		setOpcode(95);
		setLength(4);
	}
	return true;
}

std::string InstructionGetGlobal::toString()
{
	// TODO: Print the global index
	return getName() + "";
}