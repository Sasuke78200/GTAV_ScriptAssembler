#ifndef __STRING_COLLECTOR_H__
#define __STRING_COLLECTOR_H__


class StringCollector
{
public:
	StringCollector();
	~StringCollector();

	int			addString(std::string a_szString);
	int			getStringId(std::string a_szString);
	int			getStringsLength();
	int			getStringPageCount();
	char**		constructStringsPage();


private:

	std::vector<std::string>			m_Strings;

	char**								m_ppStringPages;




};



#endif