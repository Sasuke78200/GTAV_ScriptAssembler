#ifndef __STRING_COLLECTOR_H__
#define __STRING_COLLECTOR_H__

class YscHeader;

class StringCollector
{
public:
	StringCollector();
	~StringCollector();
	/*
	int			addString(std::string a_szString);
	int			getStringId(std::string a_szString);
	int			getStringsLength();
	int			getStringPageCount();
	char**		constructStringsPage();
	void		importStringPage(YscHeader* a_pYscHeader, std::ifstream* a_pFileStream);
	*/
	
	int			addString(char* a_szString);
	char*		getString(int a_uiId);
	int			getPageCount();
	int			getStringPageLength();
	char**		getStringPages();
	void		freePages();
	void		importFromBinary(YscHeader* a_pYscHeader, std::ifstream* a_pFileStream);	

private:

	std::vector<char*>	m_stringPages;
	int					m_iStringPageLength;
};



#endif