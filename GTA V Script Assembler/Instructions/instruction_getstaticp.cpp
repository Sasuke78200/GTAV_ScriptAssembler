#include "../main.h"


InstructionGetStaticP::InstructionGetStaticP()
{
	setName("getstaticp");
	setLength(-1);
	setOpcode(-1);
}

InstructionGetStaticP::~InstructionGetStaticP()
{
}

unsigned char* InstructionGetStaticP::getByteCode()
{
	return this->m_aByteCode;
}
bool InstructionGetStaticP::Process(std::string a_szAssemblyLine)
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

std::string InstructionGetStaticP::toString()
{
	std::stringstream l_ss;
	l_ss << getName() + " " << getIndex();
	return l_ss.str();
}

bool InstructionGetStaticP::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	if(*a_aByteCode == 58)
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

void InstructionGetStaticP::setIndex(int a_iIndex)
{
	// 58 byte
	if(*(char*)&a_iIndex == a_iIndex)
	{
		setOpcode(59);
		setLength(2);
		m_aByteCode[1] = a_iIndex & 0xFF;
	}
	// 79 word
	else 
	{
		setOpcode(79);
		setLength(2);
		*(short*)m_aByteCode[1] = a_iIndex & 0xFFFF;
		printf("Index2 words\n");
	}	
}

int InstructionGetStaticP::getIndex()
{
	// 58 byte
	if(getOpcode() == 58)
	{
		return (int)m_aByteCode[1];
	}
	
	// 79 word
	return *(short*)m_aByteCode[1];
}