#ifndef __STRING_COLLECTOR_H__
#define __STRING_COLLECTOR_H__

class scrHeader;

class StringCollector
{
public:
	StringCollector();
	~StringCollector();
	
	int			addString(char* a_szString);
	char*		getString(int a_uiId);
	int			getPageCount();
	int			getStringPageLength();
	char**		getStringPages();
	void		freePages();
	void		importFromBinary(scrHeader* a_pScrHeader, std::ifstream* a_pFileStream);	

private:

	std::vector<char*>	m_stringPages;
	int					m_iStringPageLength;
};



#endif