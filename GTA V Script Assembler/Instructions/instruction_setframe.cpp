#include "../main.h"



InstructionSetFrame::InstructionSetFrame()
{
	setOpcode(-1);
	setLength(-1);
	setName("setframe");
}

InstructionSetFrame::~InstructionSetFrame()
{

}

unsigned char* InstructionSetFrame::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return m_aByteCode;
}

bool InstructionSetFrame::Process(std::string a_szAssemblyLine)
{
	short l_iIndex;
	if(a_szAssemblyLine.length() == 0) return false;

	l_iIndex = atoi(a_szAssemblyLine.c_str());



	if(*(unsigned char*)&l_iIndex == l_iIndex)	// if the index fit in one byte
	{
		m_aByteCode[1] = l_iIndex;
		setLength(2);
		setOpcode(57);
	}
	else
	{
		*(unsigned short*)&m_aByteCode[1] = l_iIndex;
		setLength(3);
		setOpcode(78);
	}

	return true;
}

std::string InstructionSetFrame::toString()
{
	std::stringstream l_ss;

	l_ss << getName() << " ";

	if(getOpcode() == 57)
	{
		l_ss << (int)m_aByteCode[1];
	}
	else
	{
		l_ss << *(unsigned short*)&m_aByteCode[1];
	}

	return l_ss.str();
}

bool InstructionSetFrame::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	if(*a_aByteCode == 57)
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