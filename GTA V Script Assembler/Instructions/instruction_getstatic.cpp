#include "../main.h"


InstructionGetStatic::InstructionGetStatic()
{
	setName("getstatic");
	setLength(-1);
	setOpcode(-1);
}

InstructionGetStatic::~InstructionGetStatic()
{
}

unsigned char* InstructionGetStatic::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return this->m_aByteCode;
}

bool InstructionGetStatic::Process(std::string a_szAssemblyLine)
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

	setIndex(l_iValue);
	return true;
}

std::string InstructionGetStatic::toString()
{
	std::stringstream l_ss;
	l_ss << getName() + " " << getIndex();
	return l_ss.str();
}

bool InstructionGetStatic::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	if(*a_aByteCode == 59)
	{
		setLength(2);
	}
	else
	{
		setLength(3);
	}
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}

void InstructionGetStatic::setIndex(int a_iIndex)
{
	// 59 byte
	if(*(unsigned char*)&a_iIndex == a_iIndex)
	{
		setOpcode(59);
		setLength(2);
		m_aByteCode[1] = a_iIndex & 0xFF;
	}
	// 80 word
	else 
	{
		setOpcode(80);
		setLength(2);
		*(unsigned short*)&m_aByteCode[1] = a_iIndex & 0xFFFF;
	}	
}

int InstructionGetStatic::getIndex()
{
	// 60 byte
	if(getOpcode() == 59)
	{
		return (int)m_aByteCode[1];
	}
	
	// 80 word
	return *(unsigned short*)&m_aByteCode[1];
}