#include "../main.h"


InstructionSetGlobal::InstructionSetGlobal()
{
	setName("setglobal");
}

InstructionSetGlobal::~InstructionSetGlobal()
{

}

unsigned char* InstructionSetGlobal::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionSetGlobal::Process(std::string a_szAssemblyLine)
{
	int l_iGlobalId;

	if(a_szAssemblyLine.length() == 0) return false;


	l_iGlobalId = atoi(a_szAssemblyLine.c_str());

	if((l_iGlobalId & 0xFFFF) == l_iGlobalId)
	{
		*(short*)&m_aByteCode[1] = l_iGlobalId;
		setOpcode(84);
		setLength(3);
	}
	else
	{
		*(int*)m_aByteCode = _byteswap_ulong(l_iGlobalId);		
		setOpcode(96);
		setLength(4);
	}
	return true;
}

std::string InstructionSetGlobal::toString()
{
	// TODO: Print the global id
	return getName() + "";
}

bool InstructionSetGlobal::Process(unsigned char* a_aByteCode)
{
	return true;
}