#include "main.h"


LabelCollector::LabelCollector()
{
}

LabelCollector::~LabelCollector()
{
}

bool LabelCollector::AddLabel(std::string a_szName, unsigned int a_uiAddress)
{
	if(this->m_Labels.find(a_szName) == this->m_Labels.end())
	{
		this->m_Labels.insert(std::pair<std::string, unsigned int>(a_szName, a_uiAddress));
		return true;
	}
	return false;
}

unsigned int LabelCollector::getAddress(std::string a_szName)
{
	std::map<std::string, unsigned int>::iterator it;

	std::transform(a_szName.begin(), a_szName.end(), a_szName.begin(), ::tolower);

	it = this->m_Labels.find(a_szName); 
	
	if(it == this->m_Labels.end())
	{
		return 0xFFFFFFFF;
	}	
	return it->second;
}
