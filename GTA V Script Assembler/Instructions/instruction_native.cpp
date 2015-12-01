#include "../main.h"



InstructionNative::InstructionNative()
{
	setOpcode(44);
	setLength(4);
	setName("native");
	this->m_pNativeCollector = 0;
}

InstructionNative::~InstructionNative()
{
}


void InstructionNative::setNativeCollector(NativeCollector* a_pNativeCollector)
{
	this->m_pNativeCollector = a_pNativeCollector;
}

unsigned char* InstructionNative::getByteCode()
{
	*this->m_aByteCode = getOpcode();
	return m_aByteCode;
}

bool InstructionNative::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;
	int					l_iPos;
	std::string			l_szNativeIdentifier;
	unsigned char		l_byReturnCount;
	unsigned char		l_byArgCount;
	int					l_iNativeId;
	Native*				l_pNative;


	// nativeidentifier argcount retcount


	l_iPos = a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);

	if(l_iPos == std::string::npos)
	{
		return false;
	}

	l_szNativeIdentifier	= a_szAssemblyLine.substr(0, l_iPos);
	a_szAssemblyLine		= trim(a_szAssemblyLine.substr(l_iPos + 1));
	l_iPos					= a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);

	if(l_iPos == std::string::npos)
	{
		return false;
	}

	l_byArgCount = atoi(a_szAssemblyLine.substr(0, l_iPos).c_str());

	a_szAssemblyLine		= trim(a_szAssemblyLine.substr(l_iPos + 1));
	if(a_szAssemblyLine.length() == 0)
	{
		return false;
	}

	l_byReturnCount = atoi(a_szAssemblyLine.c_str());

	l_pNative = m_pNativeCollector->getNative(m_pNativeCollector->addNative(l_szNativeIdentifier));

	if(l_pNative)
	{
		l_iNativeId = l_pNative->m_iNativeIndex;
	}
	else
	{
		return false;
	}


	if(l_byReturnCount > 3) return false;

	m_aByteCode[1]				= (l_byArgCount << 2) | (l_byReturnCount&3);
	m_aByteCode[2]				= (l_iNativeId >> 8) & 0xFF;
	m_aByteCode[3]				= l_iNativeId & 0xFF;
	setPushCount(l_byReturnCount);
	setPopCount(l_byArgCount);
	return true;
}

std::string InstructionNative::toString()
{
	std::stringstream l_ss;
	Native* l_pNative;
	l_ss << getName() << " ";

	l_pNative = this->m_pNativeCollector->getNative((int)((m_aByteCode[2] << 2) + m_aByteCode[3]));
	if(l_pNative)
	{
		l_ss << l_pNative->m_szName;
	}
	else
	{
		l_ss << "Can't find native " << (int)((m_aByteCode[2] << 2) + m_aByteCode[3]);
	}

	l_ss << " " << getArgCount() << " " << getReturnCount();
	return l_ss.str();
}

bool InstructionNative::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	setPushCount(getReturnCount());
	setPopCount(getArgCount());
	return true;
}

int InstructionNative::getArgCount()
{
	return (m_aByteCode[1] >> 2) & 0x3F;
}

int InstructionNative::getReturnCount()
{
	return m_aByteCode[1] & 3;
}