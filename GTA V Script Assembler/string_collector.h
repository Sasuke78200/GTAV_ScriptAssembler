#ifndef __STRING_COLLECTOR_H__
#define __STRING_COLLECTOR_H__

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
	int			getPageCount();
	int			getStringPageLength();
	char**		getStringPages();
	void		freePages();
	void		copyPage(char* a_pSrc, int a_iPageDest);

private:

	std::vector<char*>	m_stringPages;
	int					m_iStringPageLength;
};



#endif