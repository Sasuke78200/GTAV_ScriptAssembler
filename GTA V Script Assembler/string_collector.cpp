#include "main.h"


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

char* StringCollector::getString(int a_uiId)
{
	if(a_uiId / 0x4000 < getPageCount())
	{
		return &this->m_stringPages[a_uiId / 0x4000][a_uiId % 0x4000];
	}
	assert(!"StringCollector::getString() -> Invalid string index !");
	return nullptr;
}

char** StringCollector::getStringPages()
{
	return &this->m_stringPages[0];
}


void StringCollector::importFromBinary(scrHeader* a_pScrHeader, std::ifstream* a_pFileStream)
{
	// strings pages informations
	int l_iBinaryPageCount;
	int l_iBinaryLength;

	int i;

	// 
	l_iBinaryPageCount	= a_pScrHeader->getStringPageCount();
	l_iBinaryLength		= a_pScrHeader->getStringsLength();

	freePages();

	for(i = 0; i < l_iBinaryPageCount; i++)
	{
		char* l_pNewPage = new char[0x4000];

		this->m_stringPages.push_back(l_pNewPage);

		a_pFileStream->seekg(a_pScrHeader->getStringPageOffset(a_pFileStream, i));
		a_pFileStream->read(l_pNewPage, a_pScrHeader->getStringPageLength(i));
	}
	this->m_iStringPageLength = l_iBinaryLength;
}