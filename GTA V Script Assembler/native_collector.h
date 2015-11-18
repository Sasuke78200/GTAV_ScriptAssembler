#ifndef __NATIVE_COLLECTOR_H__
#define __NATIVE_COLLECTOR_H__

class NativeCollector
{
public:
	
	NativeCollector();
	~NativeCollector();


	bool				ProcessAssemblyLine(std::string a_szAssemblyLine);
	int					getNativeId(std::string a_szNativeIdentifier);

	int					getNativeCount();
	unsigned long long	getNativeFromId(int a_iId);
	void				ParseJson();
	void				TranslateHash(int a_iVersion);

private:
	std::map<std::string, unsigned long long>				m_NativeHashes;
};



#endif