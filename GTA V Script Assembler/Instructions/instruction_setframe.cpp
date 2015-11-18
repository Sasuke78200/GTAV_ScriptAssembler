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
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionSetFrame::Process(std::string a_szAssemblyLine)
{
	short l_iIndex;
	if(a_szAssemblyLine.length() == 0) return false;

	l_iIndex = atoi(a_szAssemblyLine.c_str());



	if(*(char*)&l_iIndex == l_iIndex)	// if the index fit in one byte
	{
		m_aByteCode[1] = l_iIndex;
		setLength(2);
		setOpcode(57);
	}
	else
	{
		*(short*)&m_aByteCode[1] = l_iIndex;
		setLength(3);
		setOpcode(78);
	}

	return true;
}

std::string InstructionSetFrame::toString()
{
	// TODO: Print the index
	return getName() + "";
}

bool InstructionSetFrame::Process(unsigned char* a_aByteCode)
{
	return true;
}