#include "../main.h"



InstructionRet::InstructionRet()
{
	setOpcode(46);
	setLength(3);
	setName("ret");
}

InstructionRet::~InstructionRet()
{
}


unsigned char* InstructionRet::getByteCode()
{
	m_aByteCode[0] = getOpcode();
	return m_aByteCode;
}

bool InstructionRet::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;
	int				l_iPos;
	unsigned char	l_byArgCount;
	unsigned char	l_usStackPop;

	// argcount stackpop

	l_iPos = a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);
	if(l_iPos == std::string::npos)
	{
		return false;
	}

	l_byArgCount		= atoi(a_szAssemblyLine.substr(0, l_iPos).c_str());
	a_szAssemblyLine	= trim(a_szAssemblyLine.substr(l_iPos + 1));
	l_usStackPop		= atoi(a_szAssemblyLine.c_str());

	m_aByteCode[1]	= l_byArgCount;
	m_aByteCode[2]	= l_usStackPop;
	return true;
}

std::string InstructionRet::toString()
{
	// TODO: Print arg count & stack pop
	return getName() + "";
}