#include "../main.h"



InstructionMulImm::InstructionMulImm()
{
	setLength(-1);
	setOpcode(-1);
	setName("mulimm");
}

InstructionMulImm::~InstructionMulImm()
{
}

unsigned char* InstructionMulImm::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return this->m_aByteCode;
}

bool InstructionMulImm::Process(std::string a_szAssemblyLine)
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

bool InstructionMulImm::Process(unsigned char* a_pByteCode)
{
	setOpcode(*a_pByteCode);
	if(*a_pByteCode == 62)
	{
		setLength(2);
	}
	else
	{
		setLength(3);
	}
	memcpy(this->m_aByteCode, a_pByteCode, getLength());
	return true;
}

std::string InstructionMulImm::toString()
{
	std::stringstream l_ss;
	l_ss << getName() << " " << getValue();
	return l_ss.str();
}

void InstructionMulImm::setValue(short a_iValue)
{
	if(*(char*)&a_iValue == a_iValue)
	{
		setOpcode(62);
		setLength(2);
		this->m_aByteCode[1] = a_iValue & 0xFF;
	}
	else
	{
		setOpcode(69);
		setLength(3);
		*(short*)&this->m_aByteCode[1] = a_iValue;
	}
}

short InstructionMulImm::getValue()
{
	if(getOpcode() == 62)	// 2 bytes version
		return this->m_aByteCode[1];
	return *(short*)&this->m_aByteCode[1]; // 3 bytes version
}