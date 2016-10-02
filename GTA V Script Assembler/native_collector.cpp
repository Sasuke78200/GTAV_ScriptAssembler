#include "main.h"


NativeCollector::NativeCollector()
{
	m_iIndex = 0;
}

NativeCollector::~NativeCollector()
{
	freeNatives();
}


void NativeCollector::freeNatives()
{
	std::vector<Native*>::iterator it;


	for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
	{
		delete *it;
	}

	m_iIndex = 0;
}


Native* NativeCollector::getNative(std::string a_szName)
{
	std::vector<Native*>::iterator it;

	for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
	{
		if((*it)->m_szName == a_szName)
		{
			return *it;
		}
	}
	return nullptr;
}

Native* NativeCollector::getNative(int a_iIndex)
{
	std::vector<Native*>::iterator it;

	//printf("Index %d\n", a_iIndex);

	for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
	{
		if((*it)->m_iNativeIndex == a_iIndex)
		{
			return *it;
		}
	}
	//printf("fail\n");
	return nullptr;
}


int NativeCollector::addNative(std::string a_szName)
{
	Native*	l_pNative;

	l_pNative = getNative(a_szName);

	if(l_pNative == nullptr)
	{
		l_pNative = new Native();
		l_pNative->m_szName			= a_szName;
		l_pNative->m_iNativeIndex	= this->m_iIndex ++;
		
		this->m_natives.push_back(l_pNative);
	}
	return l_pNative->m_iNativeIndex;
}

int NativeCollector::getCount()
{
	return this->m_natives.size();
}


bool NativeCollector::retrieveHashes()
{
	FILE*							l_pNativeJsonFile = fopen("./jsons/natives.json", "rb");
	char							l_szBuffer[65535];
	rapidjson::FileReadStream		l_FileReadStream(l_pNativeJsonFile, l_szBuffer, sizeof(l_szBuffer));
	rapidjson::Document				l_Document;

	l_Document.ParseStream(l_FileReadStream);

	for (rapidjson::Value::ConstMemberIterator l_NamespaceObject = l_Document.MemberBegin(); l_NamespaceObject != l_Document.MemberEnd(); l_NamespaceObject++)
	{
		for (rapidjson::Value::ConstMemberIterator l_NativeObject = l_NamespaceObject->value.MemberBegin();
		l_NativeObject != l_NamespaceObject->value.MemberEnd(); l_NativeObject ++)
		{
			std::vector<Native*>::iterator it;

			for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
			{				
				if(l_NativeObject->value["name"].GetString() == (*it)->m_szName)
				{
					(*it)->m_ullNativeHash = std::stoull(l_NativeObject->name.GetString(), 0, 16);
				}
			}	
		}		
	}

	fclose(l_pNativeJsonFile);
	return true;
}
/*
void NativeCollector::translateHash(int a_iVersion)
{
	std::vector<Native*>::iterator	it;
	FILE*							l_pNativeJsonFile = fopen("./jsons/native_translation.json", "rb");
	char							l_szBuffer[65535];
	rapidjson::FileReadStream		l_FileReadStream(l_pNativeJsonFile, l_szBuffer, sizeof(l_szBuffer));
	rapidjson::Document				l_Document;

	l_Document.ParseStream(l_FileReadStream);



	if(a_iVersion < (int)l_Document["translation_tables"].Size())
	{
		int i;
		
		for(i = 0; i <= a_iVersion; i++)
		{
			for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
			{
				std::stringstream ss;
				ss << std::hex << std::uppercase << std::setw(16) << std::setfill('0') << (*it)->m_ullNativeHash;
				if(l_Document["translation_tables"][i].HasMember(ss.str().c_str()))
				{
					(*it)->m_ullNativeHash = std::stoull(std::string(l_Document["translation_tables"][i][ss.str().c_str()].GetString()), 0, 16);
				}
				else
				{
					printf("Couldn't translate native \"%s\" -> %llX\n", (*it)->m_szName.c_str(), (*it)->m_ullNativeHash);
				}
			}
		}
	}
	fclose(l_pNativeJsonFile);
}
*/

void NativeCollector::translateHash(int a_iVersion)
{
	std::vector<Native*>::iterator it;
	int i, j;
	unsigned int l_iNativesCount;
	// we're reading 335 hashes, so we don't need to translate them.
	if(a_iVersion == 0) return;

	l_iNativesCount = this->m_natives.size();

//	unsigned __int64 follow = 0xa9b6c2a8f9fe269a;

	for(i = a_iVersion; i >= 0; i--)
	{
		unsigned int l_iNativePasses = 0;
		for(j = 0; j < (sizeof(g_ullNativesHashTranslationTable[i])/2) / sizeof(unsigned __int64) && l_iNativesCount != l_iNativePasses; j++)
		{
			for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
			{
				if((*it)->m_ullNativeHash == g_ullNativesHashTranslationTable[i][j][1])
				{
//					if(follow == (*it)->m_ullNativeHash)
//					{
//						printf("0x%llX -> 0x%llX\n", follow, g_ullNativesHashTranslationTable[i][j][0]);
//						follow = g_ullNativesHashTranslationTable[i][j][0];
//					}
					(*it)->m_ullNativeHash = g_ullNativesHashTranslationTable[i][j][0];
					l_iNativePasses++;
					break;
				}
			}
		}
	}
}


void NativeCollector::importFromBinary(scrHeader* a_pScrHeader)
{
	int i;
	int l_iNativeCount;
	int l_iNativeVersion;

	freeNatives();

	l_iNativeCount = a_pScrHeader->getNativesCount();

	for(i = 0; i < l_iNativeCount; i++)
	{
		Native* l_pNative;
		std::stringstream l_ss;

		l_ss << "unk_0x" << std::hex << std::setw(16) << std::setfill('0') << a_pScrHeader->getNativeHash(i);
		l_pNative = getNative(addNative(l_ss.str()));
		l_pNative->m_ullNativeHash = a_pScrHeader->getNativeHash(i);
	}

	// Get which version of natives hashes we want to use
	l_iNativeVersion = atoi(CommandLine::Instance()->getVal("-v")->c_str());

	if(l_iNativeVersion > 0)
	{
		translateHash(l_iNativeVersion - 1);
	}	


	retrieveNames();
}

void NativeCollector::retrieveNames()
{
	FILE*							l_pNativeJsonFile = fopen("./jsons/natives.json", "rb");
	char							l_szBuffer[65535];
	rapidjson::FileReadStream		l_FileReadStream(l_pNativeJsonFile, l_szBuffer, sizeof(l_szBuffer));
	rapidjson::Document				l_Document;
	std::vector<Native*>::iterator	it;
	bool							l_bRetriveNameSpace;
	CommandLine*					l_pCommandLine;	

	l_Document.ParseStream(l_FileReadStream);

	l_pCommandLine = CommandLine::Instance();

	// if the options '-ns' exists, then we have to display natives' namespace
	l_bRetriveNameSpace = l_pCommandLine->getVal("-ns") != 0;	

	for(it = this->m_natives.begin(); it != this->m_natives.end(); it++)
	{
		for (rapidjson::Value::ConstMemberIterator l_NamespaceObject = l_Document.MemberBegin(); l_NamespaceObject != l_Document.MemberEnd(); l_NamespaceObject++)
		{
			std::stringstream l_ss;

			l_ss << "0x" << std::hex << std::setw(16) << std::setfill('0') << std::uppercase << (*it)->m_ullNativeHash;

			if(l_NamespaceObject->value.HasMember(l_ss.str().c_str()))
			{
				if(strlen(l_NamespaceObject->value[l_ss.str().c_str()]["name"].GetString()))
				{
					if(l_bRetriveNameSpace)
					{
						(*it)->m_szName = std::string(l_NamespaceObject->name.GetString()) + "::" + l_NamespaceObject->value[l_ss.str().c_str()]["name"].GetString();
					}
					else
					{
						(*it)->m_szName = l_NamespaceObject->value[l_ss.str().c_str()]["name"].GetString();
					}
				}
				else
				{
					if(l_bRetriveNameSpace)
					{
						(*it)->m_szName = std::string(l_NamespaceObject->name.GetString()) + "::" + (*it)->m_szName;
					}
				}
				l_NamespaceObject = l_Document.MemberEnd()-1;
			}	
		}
	}

	fclose(l_pNativeJsonFile);
}