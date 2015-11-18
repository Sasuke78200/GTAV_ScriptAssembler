#include "../main.h"

InstructionsPush::InstructionsPush()
{
	setOpcode(99);
	setName("spush");

	this->m_pStringCollector = 0;
}

InstructionsPush::~InstructionsPush()
{
}

void InstructionsPush::setStringCollector(StringCollector* a_pStringCollector)
{
	this->m_pStringCollector = a_pStringCollector;
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
	l_iStringId = this->m_pStringCollector->addString((char*)a_szAssemblyLine.c_str());


	// a string push is represented by two pushes
	// an integer push, which push the string index
	// and the opcode 99 which says to the script machine, that we want to get the string pointer at the index X
	setIndex(l_iStringId);
	return true;
}


std::string InstructionsPush::toString()
{
	return getName() + " \"" + this->m_pStringCollector->getString(this->m_StringIndexInstruction.getValue()) + "\"";
}

bool InstructionsPush::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	return true;
}

void InstructionsPush::setIndex(int a_iIndex)
{
	this->m_StringIndexInstruction.setValue(a_iIndex);

	memcpy(m_aByteCode, this->m_StringIndexInstruction.getByteCode(), this->m_StringIndexInstruction.getLength());
	m_aByteCode[this->m_StringIndexInstruction.getLength()] = getOpcode();

	setLength(this->m_StringIndexInstruction.getLength() + 1);
}