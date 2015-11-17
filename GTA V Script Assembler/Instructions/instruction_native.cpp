#include "../main.h"



InstructionNative::InstructionNative(NativeCollector* a_pNativeCollector)
{
	setOpcode(44);
	setLength(4);
	setName("native");
	m_pNativeCollector = a_pNativeCollector;
}

InstructionNative::~InstructionNative()
{
}


unsigned char* InstructionNative::getByteCode()
{
	m_aByteCode[0] = getOpcode();
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


	l_iNativeId = m_pNativeCollector->getNativeId(l_szNativeIdentifier);


	if(l_byReturnCount > 3) return false;

	m_aByteCode[1]				= (l_byArgCount << 2) | (l_byReturnCount&3);
	m_aByteCode[2]				= (l_iNativeId >> 8) & 0xFF;
	m_aByteCode[3]				= l_iNativeId & 0xFF;
	return true;
}

std::string InstructionNative::toString()
{
	// TODO: Print narive name, arg count & ret count
	return getName() + "";
}