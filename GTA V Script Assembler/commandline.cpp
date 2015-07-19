#include "main.h"



CommandLine::CommandLine(int a_iArgCount, char** a_pszArgs)
{
	int i;

	for(i = 1; i < a_iArgCount; i++)
	{
		if(a_pszArgs[i][0] == '-')
		{
			if(i + 1 < a_iArgCount)
			{
				// if the key isn't already in our list
				if(this->m_Args.find(a_pszArgs[i]) == this->m_Args.end())
				{
					// we add the key + value

					this->m_Args.insert(std::pair<std::string, std::string>(a_pszArgs[i], a_pszArgs[i + 1]));
				}
			}
		}
	}
}

std::string* CommandLine::getVal(std::string a_szKey)
{
	std::map<std::string, std::string>::iterator it;
	if((it = this->m_Args.find(a_szKey)) != this->m_Args.end())
	{
		return &it->second;
	}
	return 0;
}