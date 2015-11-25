#include "../main.h"


InstructioniPush::InstructioniPush()
{
	memset(m_aByteCode, 0, sizeof(m_aByteCode));
	setName("ipush");
}

InstructioniPush::~InstructioniPush()
{
}

unsigned char* InstructioniPush::getByteCode()
{
	*this->m_aByteCode = getOpcode();
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
	
	else if((a_iValue & 0xFFFFFF) == a_iValue) // 24 bits, so we're using opcode 97
	{
		*(int*)m_aByteCode = a_iValue << 8;
		setOpcode(97);
		setLength(4);
	}
	else // 32 bits, so we're using opcode 40
	{
		*(int*)&m_aByteCode[1] = a_iValue;
		setOpcode(40);
		setLength(5);
	}
}

int InstructioniPush::getValue()
{
	if(getOpcode() >= 109 && getOpcode() <= 117)
	{
		return getOpcode() - 110;
	}
	else if(getOpcode() == 37)
	{
		return m_aByteCode[1];
	}
	else if(getOpcode() == 67)
	{
		return *(short*)&m_aByteCode[1];
	}
	else if(getOpcode() == 97)
	{
		return *(int*)m_aByteCode >> 8;
	}
	else if(getOpcode() == 40)
	{
		return *(int*)&m_aByteCode[1];
	}
	assert(!"InstructioniPush::getValue() -> Invalid opcode !");
	return 0xDEADBEEF;
}

std::string InstructioniPush::toString()
{
	std::stringstream l_ss;

	l_ss << getName() << " " << getValue();

	return l_ss.str();
}

bool InstructioniPush::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	if(*a_aByteCode >= 109 && *a_aByteCode <= 117)
	{
		setLength(1);
	}
	else if(*a_aByteCode == 37)
	{
		setLength(2);
	}
	else if(*a_aByteCode == 67)
	{
		setLength(3);
	}
	else
	{
		setLength(5);
	}

	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}