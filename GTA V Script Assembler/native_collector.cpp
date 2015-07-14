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

	m_NativeHashes.insert(std::pair<std::string, unsigned long long>(l_szNativeIdentifier, l_uiNativeHash));
	return true;
}

int NativeCollector::getNativeId(std::string a_szNativeIdentifier)
{
	int l_iIndex = 0;
	std::map<std::string, unsigned long long>::iterator it;

	for(it = this->m_NativeHashes.begin(); it != this->m_NativeHashes.end(); it++)
	{
		if(it->first == a_szNativeIdentifier)
		{
			return l_iIndex;
		}
		l_iIndex++;
	}
	return -1;
}


int NativeCollector::getNativeCount()
{
	return this->m_NativeHashes.size();
}

unsigned long long NativeCollector::getNativeFromId(int a_iId)
{
	int l_iIndex = 0;


	if(a_iId >= 0 && a_iId < (int)this->m_NativeHashes.size())
	{
		for(std::map<std::string, unsigned long long>::iterator it = this->m_NativeHashes.begin(); it != this->m_NativeHashes.end(); it++)
		{
			if(l_iIndex == a_iId)
			{
				 return it->second;
			}
			l_iIndex++;
		}
	}
	return -1;
}

void NativeCollector::ParseJson()
{
	FILE*							l_pNativeJsonFile = fopen("natives.json", "rb");
	char							l_szBuffer[65535];
	rapidjson::FileReadStream		l_FileReadStream(l_pNativeJsonFile, l_szBuffer, sizeof(l_szBuffer));
	rapidjson::Document				l_Document;

	l_Document.ParseStream(l_FileReadStream);

	for (rapidjson::Value::ConstMemberIterator l_NamespaceObject = l_Document.MemberBegin(); l_NamespaceObject != l_Document.MemberEnd(); l_NamespaceObject++)
	{
		for (rapidjson::Value::ConstMemberIterator l_NativeObject = l_NamespaceObject->value.MemberBegin();
		l_NativeObject != l_NamespaceObject->value.MemberEnd(); l_NativeObject ++)
		{
			std::map<std::string, unsigned long long>::iterator it;

			for(it = this->m_NativeHashes.begin(); it != this->m_NativeHashes.end(); it++)
			{
				if(l_NativeObject->value["name"].GetString() == it->first)
				{
					it->second = std::stoull(l_NativeObject->name.GetString(), 0, 16);
				}
			}	
		}		
	}

	fclose(l_pNativeJsonFile);
}

void NativeCollector::TranslateHash(int a_iVersion)
{
	std::map<std::string, unsigned long long>::iterator it;
	FILE*							l_pNativeJsonFile = fopen("native_translation.json", "rb");
	char							l_szBuffer[65535];
	rapidjson::FileReadStream		l_FileReadStream(l_pNativeJsonFile, l_szBuffer, sizeof(l_szBuffer));
	rapidjson::Document				l_Document;

	l_Document.ParseStream(l_FileReadStream);



	if(a_iVersion < l_Document["translation_tables"].Size())
	{
		int i;
		
		for(i = 0; i <= a_iVersion; i++)
		{
			for(it = this->m_NativeHashes.begin(); it != this->m_NativeHashes.end(); it++)
			{
				std::stringstream ss;
				ss << std::hex << std::uppercase << std::setw(16) << std::setfill('0') << it->second;
				if(l_Document["translation_tables"][i].HasMember(ss.str().c_str()))
				{
					it->second = std::stoull(std::string(l_Document["translation_tables"][i][ss.str().c_str()].GetString()), 0, 16);
				}
				else
				{
					printf("Couldn't translate native \"%s\" -> %llX\n", it->first.c_str(), it->second);
				}
			}
		}
	}


	fclose(l_pNativeJsonFile);
}