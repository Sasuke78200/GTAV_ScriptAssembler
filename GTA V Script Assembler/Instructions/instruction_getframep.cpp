#include "../main.h"



InstructionGetFrameP::InstructionGetFrameP()
{
	setOpcode(-1);
	setLength(-1);
	setName("getframep");
}

InstructionGetFrameP::~InstructionGetFrameP()
{

}

unsigned char* InstructionGetFrameP::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionGetFrameP::Process(std::string a_szAssemblyLine)
{
	short l_iIndex;
	if(a_szAssemblyLine.length() == 0) return false;

	l_iIndex = atoi(a_szAssemblyLine.c_str());


	if(*(char*)&l_iIndex == l_iIndex)	// if the index fit in one byte
	{
		m_aByteCode[1] = l_iIndex;
		setLength(2);
		setOpcode(55);
	}
	else
	{
		*(short*)&m_aByteCode[1] = l_iIndex;
		setLength(3);
		setOpcode(76);
	}

	return true;
}

std::string InstructionGetFrameP::toString()
{
	// TODO: Print the index
	return getName();
}