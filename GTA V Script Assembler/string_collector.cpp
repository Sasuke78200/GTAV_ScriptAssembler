#include "main.h"



StringCollector::StringCollector()
{
	m_ppStringPages = 0;
}

StringCollector::~StringCollector()
{
	if(m_ppStringPages)
	{
		int l_iPageCount = getStringPageCount();
		for(int i = 0; i < l_iPageCount; i++)
		{
			if(m_ppStringPages[i])
			{
				delete [] m_ppStringPages[i];
			}
		}
		delete [] m_ppStringPages;
	}
}

int	StringCollector::addString(std::string a_szString)
{
	int l_iStringId;

	l_iStringId = getStringId(a_szString);

	// the string isn't already present on the list, so we add it.
	// otherwise we directly return its ID
	if(l_iStringId == 0xFFFFFFFF)
	{
		this->m_Strings.push_back(a_szString);
		// lazy way
		l_iStringId = getStringId(a_szString);
	}

	return l_iStringId;
}

int	StringCollector::getStringId(std::string a_szString)
{
	int		l_iTextId;

	l_iTextId = 0;

	// we lookup the string table, and try to calculate the string index
	for(unsigned int i = 0; i < this->m_Strings.size(); i++)
	{
		// we found our string
		if(m_Strings[i] == a_szString)
		{
			return l_iTextId;
		}
		else
		{
			int l_iStringLen = m_Strings[i].length() + 1;
			// TODO: Try to make sure each string don't go over the page size
			l_iTextId += l_iStringLen;
		}
	}


	// if we didn't find the string, we return -1 (0xFFFFFFFF)
	return 0xFFFFFFFF;
}

int	StringCollector::getStringsLength()
{
	int l_iLen;

	l_iLen = 0;

	for(unsigned int i = 0; i < this->m_Strings.size(); i++)
	{		
		l_iLen += this->m_Strings[i].length();
	}
	return l_iLen;
}

int	StringCollector::getStringPageCount()
{
	// each page is 0x4000 bytes
	return (getStringsLength() / 0x4000) + 1;
}

char** StringCollector::constructStringsPage()
{
	int l_iPageCount = getStringPageCount();
	//TODO: Construct the pages
	if(m_ppStringPages)
	{		
		for(int i = 0; i < l_iPageCount; i++)
		{
			if(m_ppStringPages[i])
			{
				delete [] m_ppStringPages[i];
			}
		}
		delete [] m_ppStringPages;
	}

	m_ppStringPages = new char*[l_iPageCount];

	for(int i = 0; i < l_iPageCount; i++)
	{
		int l_iIndex = 0;
		m_ppStringPages[i] = new char[0x4000];

		for(unsigned int j = 0; j < this->m_Strings.size(); j++)
		{
			memcpy(&m_ppStringPages[i][l_iIndex], this->m_Strings[j].c_str(), this->m_Strings[j].length() + 1);
			l_iIndex += this->m_Strings[j].length() + 1;
		}
	}

	return m_ppStringPages;
}