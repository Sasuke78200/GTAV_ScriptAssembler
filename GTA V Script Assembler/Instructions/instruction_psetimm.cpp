#include "../main.h"


InstructionPSetImm::InstructionPSetImm()
{
	setLength(-1);
	setOpcode(-1);
	setName("psetimm");
}

InstructionPSetImm::~InstructionPSetImm()
{
}

unsigned char* InstructionPSetImm::getByteCode()
{
	this->m_aByteCode[0] = getOpcode();
	return this->m_aByteCode;
}

bool InstructionPSetImm::Process(std::string a_szAssemblyLine)
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
	return false;
}

bool InstructionPSetImm::Process(unsigned char* a_pByteCode)
{
	setOpcode(*a_pByteCode);
	if(*a_pByteCode == 54) // 2 bytes version
	{
		setLength(2);
	}
	else // 3 bytes version (opcode 75)
	{
		setLength(3);
	}
	memcpy(this->m_aByteCode, a_pByteCode, getLength());
	return true;
}

std::string InstructionPSetImm::toString()
{
	std::stringstream l_ss;	
	l_ss << getName() << " " << getIndex();
	return l_ss.str();
}

void InstructionPSetImm::setIndex(unsigned int a_iIndex)
{
	if(*(unsigned char*)&a_iIndex == a_iIndex)
	{
		setOpcode(66);
		setLength(2);
		this->m_aByteCode[1] = a_iIndex & 0xFF;
	}
	else
	{
		setOpcode(72);
		setLength(3);
		*(unsigned short*)&this->m_aByteCode[1] = a_iIndex & 0xFFFF;
	}
}

unsigned int InstructionPSetImm::getIndex()
{
	if(getOpcode() == 66)
		return (unsigned int)this->m_aByteCode[1];
	return *(unsigned short*)&this->m_aByteCode[1];
}
