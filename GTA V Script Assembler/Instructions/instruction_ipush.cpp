#include "../main.h"


InstructioniPush::InstructioniPush()
{
	memset(m_aByteCode, 0, sizeof(m_aByteCode));
}

InstructioniPush::~InstructioniPush()
{
}

unsigned char* InstructioniPush::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructioniPush::Process(std::string a_szAssemblyLine)
{	
	int l_iValue;

	if(a_szAssemblyLine[0] == '0' && a_szAssemblyLine[1] == 'x') // we're playing with an hexadecimal value
	{
		l_iValue = strtol(a_szAssemblyLine.substr(2).c_str(), 0, 16);
	}
	else
	{
		l_iValue = strtol(a_szAssemblyLine.c_str(), 0, 10);
	}


	if((l_iValue & 0xFF) == l_iValue) // 8 bits, so we're using opcode 37
	{
		m_aByteCode[1] = (l_iValue & 0xFF);
		setOpcode(37);
		setLength(2);
	}
	else if((l_iValue & 0xFFFF) == l_iValue) // 16 bits, so we're using opcode 67
	{
		*(short*)&m_aByteCode[1] = (l_iValue & 0xFFFF);
		setOpcode(67);
		setLength(3);
	}
	else if((l_iValue & 0xFFFFFFFF) == l_iValue) // 32 bits, so we're using opcode 40
	{
		*(int*)&m_aByteCode[1] = l_iValue;
		setOpcode(40);
		setLength(5);
	}
	else
	{
		printf("InstructioniPush::Process(%s) -> wtf !!!!\n", a_szAssemblyLine.c_str());
		return false;
	}
	return true;
}