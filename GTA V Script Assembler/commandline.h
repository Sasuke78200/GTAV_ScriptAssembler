#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__


class CommandLine
{
public:

	CommandLine(int a_iArgCount, char** a_pszArgs);


	std::string*	getVal(std::string a_szKey);
	void			setVal(std::string a_szKey, std::string a_szVal);


	static CommandLine* Instance(int a_iArgCount = 0, char** a_pszArgs = 0)
	{
		static CommandLine* l_pInstance = 0;

		if(l_pInstance == 0)
		{
			l_pInstance = new CommandLine(a_iArgCount, a_pszArgs);
		}

		return l_pInstance;
	}



private:

	std::map<std::string, std::string>		m_Args;

};



#endif