#include "main.h"


NativeCollector::NativeCollector()
{
}

NativeCollector::~NativeCollector()
{
}


bool NativeCollector::ProcessAssemblyLine(std::string a_szAssemblyLine)
{
	std::string				l_szNativeIdentifier;
	int						l_iPos;
	unsigned long long		l_uiNativeHash;
	// native param_count return_count

	l_iPos = a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);

	if(l_iPos == std::string::npos)
	{
		printf("Error: NativeCollector::ProcessAssemblyLine() -> Cannot find first space !\n");
		return false;
	}

	l_szNativeIdentifier = a_szAssemblyLine.substr(0, l_iPos);


	// so there we got our native identifier which can be its name or its has on hex or dec


	if(l_szNativeIdentifier.length() > 2 && l_szNativeIdentifier[0] == '0' && l_szNativeIdentifier[1] == 'x') // hex hash
	{
		l_uiNativeHash = std::stoull(l_szNativeIdentifier.substr(2), 0, 16);		
	}
	else if(l_szNativeIdentifier[0] >= '0' && l_szNativeIdentifier[0] <= '9' || l_szNativeIdentifier[0] == '-') // dec hash
	{
		l_uiNativeHash = std::stoull(l_szNativeIdentifier, 0, 10);
	}
	else // name
	{
		l_uiNativeHash = getNativeHashFromName(l_szNativeIdentifier);
	}


	if(l_uiNativeHash == 0)
	{
		return false; 
	}

	for(int i = 0; i < (int)m_NativeHashes.size(); i++)
	{
		if(m_NativeHashes[i] == l_uiNativeHash)
			return true;
	}

	m_NativeHashes.push_back(l_uiNativeHash);
	return true;
}

int NativeCollector::getNativeId(std::string a_szNativeIdentifier)
{
	unsigned long long l_uiNativeHash;


	if(a_szNativeIdentifier.length() > 2 && a_szNativeIdentifier[0] == '0' && a_szNativeIdentifier[1] == 'x') // hex hash
	{
		l_uiNativeHash = std::stoull(a_szNativeIdentifier.substr(2), 0, 16);		
	}
	else if(a_szNativeIdentifier[0] >= '0' && a_szNativeIdentifier[0] <= '9' || a_szNativeIdentifier[0] == '-') // dec hash
	{
		l_uiNativeHash = std::stoull(a_szNativeIdentifier, 0, 10);
	}
	else // name
	{
		l_uiNativeHash = getNativeHashFromName(a_szNativeIdentifier);
	}

	for(int i = 0; i < (int)m_NativeHashes.size(); i++)
	{
		if(m_NativeHashes[i] == l_uiNativeHash)
			return i;
	}
	return -1;
}

unsigned long long NativeCollector::getNativeHashFromName(std::string a_szNativeName)
{
	// TODO: read into the json list where natives name are associated with their hashes.
	return 0;
}

int NativeCollector::getNativeCount()
{
	return this->m_NativeHashes.size();
}

unsigned long long NativeCollector::getNativeFromId(int a_iId)
{
	if(a_iId >= 0 && a_iId < (int)this->m_NativeHashes.size())
	{
		return this->m_NativeHashes[a_iId];
	}
	return -1;
}
