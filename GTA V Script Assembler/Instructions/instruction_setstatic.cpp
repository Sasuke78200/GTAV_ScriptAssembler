#include "../main.h"


InstructionSetStatic::InstructionSetStatic()
{
	setName("setstatic");
	setLength(-1);
	setOpcode(-1);
}

InstructionSetStatic::~InstructionSetStatic()
{
}

unsigned char* InstructionSetStatic::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return this->m_aByteCode;
}
bool InstructionSetStatic::Process(std::string a_szAssemblyLine)
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

std::string InstructionSetStatic::toString()
{
	std::stringstream l_ss;
	l_ss << getName() + " " << getIndex();
	return l_ss.str();
}

bool InstructionSetStatic::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	if(*a_aByteCode == 60)
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

void InstructionSetStatic::setIndex(int a_iIndex)
{
	// 60 byte
	if(*(unsigned char*)&a_iIndex == a_iIndex)
	{
		setOpcode(60);
		setLength(2);
		m_aByteCode[1] = a_iIndex & 0xFF;
	}
	// 81 word
	else 
	{
		setOpcode(81);
		setLength(2);
		*(unsigned short*)m_aByteCode[1] = a_iIndex & 0xFFFF;
		printf("Index2 words\n");
	}	
}

int InstructionSetStatic::getIndex()
{
	// 60 byte
	if(getOpcode() == 60)
	{
		return (int)m_aByteCode[1];
	}
	
	// 81 word
	return *(unsigned short*)m_aByteCode[1];
}