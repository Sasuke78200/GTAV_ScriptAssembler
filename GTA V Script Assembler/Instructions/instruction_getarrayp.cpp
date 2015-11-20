#include "../main.h"


InstructionGetArrayP::InstructionGetArrayP()
{
	setLength(-1);
	setOpcode(-1);
	setName("getarrayp");
}

InstructionGetArrayP::~InstructionGetArrayP()
{
}

unsigned char* InstructionGetArrayP::getByteCode()
{
	this->m_aByteCode[0] = getOpcode();
	return this->m_aByteCode;
}

bool InstructionGetArrayP::Process(std::string a_szAssemblyLine)
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

	setElementSize((unsigned int)l_iValue);
	return false;
}

bool InstructionGetArrayP::Process(unsigned char* a_pByteCode)
{
	setOpcode(*a_pByteCode);
	if(*a_pByteCode == 52) // 2 bytes version
	{
		setLength(2);
	}
	else // 3 bytes version (opcode 73)
	{
		setLength(3);
	}
	memcpy(this->m_aByteCode, a_pByteCode, getLength());
	return true;
}

std::string InstructionGetArrayP::toString()
{
	std::stringstream l_ss;	
	l_ss << getName() << " " << getElementSize();
	return l_ss.str();
}

void InstructionGetArrayP::setElementSize(unsigned int a_iSize)
{
	if(*(unsigned char*)&a_iSize == a_iSize)
	{
		setOpcode(52);
		setLength(2);
		this->m_aByteCode[1] = a_iSize & 0xFF;
	}
	else
	{
		setOpcode(73);
		setLength(3);
		*(unsigned short*)&this->m_aByteCode[1] = a_iSize & 0xFFFF;
	}
}

unsigned int InstructionGetArrayP::getElementSize()
{
	if(getOpcode() == 52)
		return (unsigned int)this->m_aByteCode[1];
	return *(unsigned short*)&this->m_aByteCode[1];
}
