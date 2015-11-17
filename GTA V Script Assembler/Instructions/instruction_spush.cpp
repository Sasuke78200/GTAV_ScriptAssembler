#include "../main.h"

InstructionsPush::InstructionsPush(StringCollector* a_pStringCollector)
{
	setOpcode(99);
	setName("spush");

	this->m_pStringCollector = a_pStringCollector;
}

InstructionsPush::~InstructionsPush()
{
}

unsigned char* InstructionsPush::getByteCode()
{
	return m_aByteCode;
}

bool InstructionsPush::Process(std::string a_szAssemblyLine)
{
	int		l_iOpenQuote, l_iCloseQuote;
	bool	l_bContinueToSearch;
	int		l_iStringId;

	if(a_szAssemblyLine.length() == 0) return false;


	l_iOpenQuote	= a_szAssemblyLine.find_first_of('"');
	if(l_iOpenQuote == std::string::npos) return false;


	l_bContinueToSearch = true;
	l_iCloseQuote = l_iOpenQuote;


	while(l_bContinueToSearch)
	{
		int l_iPos;
		l_iPos = a_szAssemblyLine.find_first_of('"', l_iCloseQuote + 1);

		if(l_iPos == std::string::npos)
		{
			return false;
		}

		if(a_szAssemblyLine[l_iPos - 1] != '\\')
		{			
			l_bContinueToSearch = false;
		}
		l_iCloseQuote = l_iPos;
	}

	// if there's more text after the closing double quote, the line is incorrect, we return false
	if(a_szAssemblyLine.length() - 1 != l_iCloseQuote) return false;

	a_szAssemblyLine = a_szAssemblyLine.substr(l_iOpenQuote + 1, l_iCloseQuote - l_iOpenQuote - 1);

	// add the string to the string collector
	l_iStringId = this->m_pStringCollector->addString(a_szAssemblyLine);


	// a string push is represented by two pushes
	// an integer push, which push the string index
	// and the opcode 99 which says to the script machine, that we want to get the string pointer at the index X
	this->m_StringIndexInstruction.setValue(l_iStringId);


	memcpy(m_aByteCode, this->m_StringIndexInstruction.getByteCode(), this->m_StringIndexInstruction.getLength());
	m_aByteCode[this->m_StringIndexInstruction.getLength()] = getOpcode();

	setLength(this->m_StringIndexInstruction.getLength() + 1);
	return true;
}


std::string InstructionsPush::toString()
{
	// TODO: Print the string
	return getName() + "";
}