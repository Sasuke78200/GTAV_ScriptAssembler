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
	*this->m_aByteCode = getOpcode();
	return m_aByteCode;
}

bool InstructionRet::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;
	int				l_iPos;
	unsigned char	l_byArgCount;
	unsigned char	l_usStackPop;

	// stackpop argcount

	l_iPos = a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);
	if(l_iPos == std::string::npos)
	{
		return false;
	}

	l_byArgCount		= atoi(a_szAssemblyLine.substr(0, l_iPos).c_str());
	a_szAssemblyLine	= trim(a_szAssemblyLine.substr(l_iPos + 1));
	l_usStackPop		= atoi(a_szAssemblyLine.c_str());

	m_aByteCode[1]	= l_usStackPop;
	m_aByteCode[2]	= l_byArgCount;
	return true;
}

std::string InstructionRet::toString()
{
	std::stringstream l_ss;
	l_ss << getName() << " " << getReturnCount() << " " << getPopStackCount();
	return l_ss.str();
}

bool InstructionRet::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}

int InstructionRet::getReturnCount()
{
	return m_aByteCode[2];
}

int InstructionRet::getPopStackCount()
{
	return m_aByteCode[1];
}