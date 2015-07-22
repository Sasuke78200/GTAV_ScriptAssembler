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
	if(a_szAssemblyLine.length() == 0) return 0;

	__int64 l_iValue;

	if(a_szAssemblyLine[0] == '0' && a_szAssemblyLine[1] == 'x') // we're playing with an hexadecimal value
	{
		l_iValue = _strtoi64(a_szAssemblyLine.substr(2).c_str(), 0, 16);
	}
	else
	{		
		l_iValue = _strtoi64(a_szAssemblyLine.c_str(), 0, 10);
	}

	setValue(l_iValue);
	return true;
}

void InstructioniPush::setValue(int a_iValue)
{
	if(a_iValue >= -1 && a_iValue <= 7)
	{
		setOpcode(110 + a_iValue);
		setLength(1);
	}
	else if(*(char*)&a_iValue == a_iValue) // 8 bits, so we're using opcode 37
	{
		m_aByteCode[1] = (a_iValue & 0xFF);
		setOpcode(37);
		setLength(2);
	}
	else if(*(short*)&a_iValue == a_iValue) // 16 bits, so we're using opcode 67
	{
		*(short*)&m_aByteCode[1] = (a_iValue & 0xFFFF);
		setOpcode(67);
		setLength(3);
	}
	/*
	else if((l_iValue & 0xFFFFFF) == l_iValue) // 24 bits, so we're using opcode 97
	{
		*(int*)&m_aByteCode[1] = (l_iValue & 0xFFFFFF);
		setOpcode(97);
		setLength(4);
	}*/
	else // 32 bits, so we're using opcode 40
	{
		*(int*)&m_aByteCode[1] = a_iValue;
		setOpcode(40);
		setLength(5);
	}
}