#include "../main.h"


InstructionPAdd::InstructionPAdd()
{
	setLength(-1);
	setOpcode(-1);
	setName("padd");
}

InstructionPAdd::~InstructionPAdd()
{
}

unsigned char* InstructionPAdd::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return this->m_aByteCode;
}

bool InstructionPAdd::Process(std::string a_szAssemblyLine)
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

	setIndex((unsigned int)l_iValue);
	return true;
}

bool InstructionPAdd::Process(unsigned char* a_pByteCode)
{
	setOpcode(*a_pByteCode);
	if(*a_pByteCode == 63)
	{
		setLength(1);
	}
	else if(*a_pByteCode == 64) // 2 bytes version
	{
		setLength(2);
	}
	else // 3 bytes version (opcode 70)
	{
		setLength(3);
	}
	memcpy(this->m_aByteCode, a_pByteCode, getLength());
	return true;
}

std::string InstructionPAdd::toString()
{
	std::stringstream l_ss;	
	
	if(getOpcode() == 63)
		return getName();
	
	l_ss << getName() << " " << getIndex();
	return l_ss.str();
}

void InstructionPAdd::setIndex(unsigned int a_iIndex)
{
	if(a_iIndex == 0)
	{
		setOpcode(63);
		setLength(1);		
	}
	else if(*(unsigned char*)&a_iIndex == a_iIndex)
	{
		setOpcode(64);
		setLength(2);
		this->m_aByteCode[1] = a_iIndex & 0xFF;
	}
	else
	{
		setOpcode(70);
		setLength(3);
		*(unsigned short*)&this->m_aByteCode[1] = a_iIndex & 0xFFFF;
	}
}

unsigned int InstructionPAdd::getIndex()
{
	if(getOpcode() == 63)
		return 0;
	if(getOpcode() == 64)
		return (unsigned int)this->m_aByteCode[1];	
	return *(unsigned short*)&this->m_aByteCode[1];
}
