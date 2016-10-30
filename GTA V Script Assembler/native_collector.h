#ifndef __NATIVE_COLLECTOR_H__
#define __NATIVE_COLLECTOR_H__

class scrHeader;

class Native
{
public:

	Native()
	{
		this->m_szName			= "";
		this->m_ullNativeHash	= 0;
		this->m_iNativeIndex	= -1;
	}

	std::string					m_szName;
	unsigned __int64			m_ullNativeHash;
	int							m_iNativeIndex;
};


class NativeCollector
{
public:
	
	NativeCollector();
	~NativeCollector();

	void				freeNatives();
	int					addNative(std::string a_szName);
	Native*				getNative(std::string a_szName);
	Native*				getNative(int a_iIndex);
	int					getCount();
	bool				retrieveHashes();
	void				translateHashBackward(int a_iVersion);
	void				translateHashForward(int a_iVersion);
	void				importFromBinary(scrHeader* a_pScrHeader);
	void				retrieveNames();

	/*
	bool				ProcessAssemblyLine(std::string a_szAssemblyLine);
	int					getNativeId(std::string a_szNativeIdentifier);

	int					getNativeCount();
	unsigned long long	getNativeFromId(int a_iId);
	void				ParseJson();
	void				TranslateHash(int a_iVersion);*/

private:
	//std::map<std::string, unsigned long long>				m_NativeHashes;


	std::vector<Native*>		m_natives;
	int							m_iIndex;

};



#endif