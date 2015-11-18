#include "main.h"
#include "ysc_header.h"

StringCollector::StringCollector()
{
	this->m_iStringPageLength	= 0;
}

StringCollector::~StringCollector()
{
	freePages();
}

void StringCollector::freePages()
{
	std::vector<char*>::iterator it;


	for(it = m_stringPages.begin(); it != m_stringPages.end(); it++)
	{
		delete [] (*it);
	}
	m_stringPages.clear();
	this->m_iStringPageLength = 0;
}

int StringCollector::getPageCount()
{
	//return this->m_iStringPageLength / 0x4000;
	return m_stringPages.size();					// both are the same
}

int StringCollector::getStringPageLength()
{
	return this->m_iStringPageLength;
}

int StringCollector::addString(char* a_szString)
{
	int l_iStrLen;
	int	l_iPage;
	int l_iStringId;

	
	l_iStrLen = strlen(a_szString);

	// if the current page len + the string len exceeds the max page len (0x4000)
	// then we increase virtually the current page len to atteign 0x4000
	if((this->m_iStringPageLength % 0x4000) + l_iStrLen >= 0x4000)
	{
		this->m_iStringPageLength += 0x4000 - (this->m_iStringPageLength % 0x4000);
	}

	// in which page we're going to write our string
	l_iPage = this->m_iStringPageLength / 0x4000;

	// if we need to alloc a new page
	if(l_iPage >= getPageCount())
	{
		char* l_pNewPage = new char[0x4000];
		m_stringPages.push_back(l_pNewPage);
		memset(l_pNewPage, 0, 0x4000);
	}

	strcpy(&m_stringPages[l_iPage][this->m_iStringPageLength % 0x4000], a_szString);


	l_iStringId = this->m_iStringPageLength;
	this->m_iStringPageLength += l_iStrLen + 1;
	return l_iStringId;
}

/*
void StringCollector::importStringPage(YscHeader* a_pYscHeader, std::ifstream* a_pFileStream)
{
	freePages();

}*/

char** StringCollector::getStringPages()
{
	return &this->m_stringPages[0];
}

/*
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
		l_iLen += this->m_Strings[i].length() + 1;
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
		memset(m_ppStringPages[i], 0, 0x4000);

		for(unsigned int j = 0; j < this->m_Strings.size(); j++)
		{
			memcpy(&m_ppStringPages[i][l_iIndex], this->m_Strings[j].c_str(), this->m_Strings[j].length() + 1);
			l_iIndex += this->m_Strings[j].length() + 1;
		}
	}

	return m_ppStringPages;
}*/