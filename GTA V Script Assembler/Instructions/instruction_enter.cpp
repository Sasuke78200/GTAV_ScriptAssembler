#include "../main.h"


InstructionEnter::InstructionEnter()
{
	setOpcode(45);
	setLength(5);
	setName("enter");
}

InstructionEnter::~InstructionEnter()
{

}

unsigned char* InstructionEnter::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return m_aByteCode;
}

bool InstructionEnter::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;
	int				l_iPos;
	unsigned char	l_byArgCount;
	unsigned short	l_usStackAlloc;

	// argcount stackalloc


	l_iPos = a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);
	if(l_iPos == std::string::npos)
	{
		return false;
	}

	l_byArgCount		= atoi(a_szAssemblyLine.substr(0, l_iPos).c_str());
	a_szAssemblyLine	= trim(a_szAssemblyLine.substr(l_iPos + 1));
	l_usStackAlloc		= atoi(a_szAssemblyLine.c_str());

	m_aByteCode[1]						= l_byArgCount;
	*(unsigned short*)&m_aByteCode[2]	= l_usStackAlloc;
	m_aByteCode[4]						= 0;
	return true;
}

std::string InstructionEnter::toString()
{
	std::stringstream l_ss;
	l_ss << getName() << " " << getArgCount() << " " << getStackAllocCount();
	return l_ss.str();
}

bool InstructionEnter::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}

int InstructionEnter::getArgCount()
{
	return m_aByteCode[1];
}

int InstructionEnter::getStackAllocCount()
{
	return *(unsigned short*)&m_aByteCode[2];
}
