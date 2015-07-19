#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__


class CommandLine
{
public:

	CommandLine(int a_iArgCount, char** a_pszArgs);


	std::string*	getVal(std::string a_szKey);



private:

	std::map<std::string, std::string>		m_Args;




};



#endif