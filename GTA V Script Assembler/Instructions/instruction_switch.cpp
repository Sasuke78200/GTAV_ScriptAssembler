#include "../main.h"



InstructionSwitch::InstructionSwitch()
{
	this->m_pLabelCollector	= nullptr;
	this->m_pByteCode		= nullptr;
	setOpcode(98);
	setLength(-1);
	setName("switch");
}

InstructionSwitch::~InstructionSwitch()
{
	if(this->m_pByteCode != nullptr)
		delete [] this->m_pByteCode;
	this->m_pByteCode = nullptr;
}

unsigned char* InstructionSwitch::getByteCode()
{
	if(this->m_pByteCode == nullptr)
		buildByteCode();
	return this->m_pByteCode;
}

bool InstructionSwitch::Process(std::string a_szAssemblyLine)
{
	std::string l_szCase;
	int l_iPos;
	if(a_szAssemblyLine.length() == 0) return false;

	l_iPos = a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);

	while(l_iPos != std::string::npos || a_szAssemblyLine.length() != 0)
	{
		int l_iSeparatorPos;
		__int64 l_iValue;

		if(l_iPos == std::string::npos)
		{
			l_szCase = a_szAssemblyLine;
			a_szAssemblyLine = "";
		}
		else
		{
			l_szCase			= a_szAssemblyLine.substr(0, l_iPos);
			a_szAssemblyLine	= trim(a_szAssemblyLine.substr(l_iPos + 1));
			l_iPos				= a_szAssemblyLine.find_first_of(ASSEMBLY_SPACE);
		}

		l_iSeparatorPos = l_szCase.find(':');
		if(l_iSeparatorPos == std::string::npos) return false;

	

		if(l_szCase[0] == '0' && l_szCase[1] == 'x') // we're playing with an hexadecimal value
		{
			l_iValue = _strtoi64(l_szCase.substr(2, l_iSeparatorPos).c_str(), 0, 16);
		}
		else
		{	
			l_iValue = _strtoi64(l_szCase.substr(0, l_iSeparatorPos).c_str(), 0, 10);
		}
		if(addCase((int)l_iValue, l_szCase.substr(l_iSeparatorPos+1)) == false)
		{
			return false;
		}
	}

	setLength(2 + this->m_SwitchCases.size() * 6);

	return true;
}

bool InstructionSwitch::Process(unsigned char* a_pByteCode)
{
	int i;
	setOpcode(*a_pByteCode);
	setLength(a_pByteCode[1] * 6 + 2);

	freeCases();
	if(this->m_pByteCode != nullptr)
		delete [] this->m_pByteCode;

	this->m_pByteCode = new unsigned char[getLength()];
	memcpy(this->m_pByteCode, a_pByteCode, getLength());

	for(i = 0; i < getCaseCount(); i++)
	{
		std::stringstream l_ss;
		l_ss << "label_" << std::setfill('0') << std::setw(4) << std::hex << getCaseJmpAddress(i);
		if(addCase(*(int*)&this->m_pByteCode[2 + i * 6], l_ss.str()) == false) return false;
	}
	return true;
}

std::string InstructionSwitch::toString()
{
	std::vector<Case*>::iterator it;
	std::stringstream l_ss;

	l_ss << getName();

	for(it = this->m_SwitchCases.begin(); it != this->m_SwitchCases.end(); it++)
	{
		l_ss << " " << (*it)->m_iValue << ":" << (*it)->m_szJmpLabel;
	}


	return l_ss.str();
}

void InstructionSwitch::setLabelCollector(LabelCollector* a_pLabelCollector)
{
	this->m_pLabelCollector = a_pLabelCollector;
}

int InstructionSwitch::getCaseCount()
{
	return this->m_pByteCode[1];
}

int InstructionSwitch::getCaseValue(int a_iIndex)
{
	if(a_iIndex < getCaseCount())
	{
		return *(int*)&this->m_pByteCode[2 + a_iIndex * 6];
	}
	return -1;
}

int InstructionSwitch::getCaseJmpAddress(int a_iIndex)
{
	if(a_iIndex < getCaseCount())
	{
		// compute the absolute address from the rel
		return *(short*)&this->m_pByteCode[6 + a_iIndex * 6]	// rel addr
			+ (getAddress() + 8 + 6*a_iIndex);					// 
	}
	return -1;
}


bool InstructionSwitch::addCase(int a_iVal, std::string m_szJmpLabel)
{
	std::vector<Case*>::iterator	it;
	Case*							l_pCase;

	for(it = this->m_SwitchCases.begin(); it != this->m_SwitchCases.end(); it++)
	{
		if((*it)->m_iValue == a_iVal) return false;
	}

	l_pCase = new Case();
	l_pCase->m_iValue		= a_iVal;
	l_pCase->m_szJmpLabel	= m_szJmpLabel;

	this->m_SwitchCases.push_back(l_pCase);
	return true;
}

void InstructionSwitch::freeCases()
{
	std::vector<Case*>::iterator it;

	for(it = this->m_SwitchCases.begin(); it != this->m_SwitchCases.end(); it++)
	{
		delete *it;
	}
	this->m_SwitchCases.clear();
}

void InstructionSwitch::buildByteCode()
{
	int i;
	std::vector<Case*>::iterator it;

	i = 0;

	if(this->m_pByteCode)
		delete [] this->m_pByteCode;

	this->m_pByteCode = new unsigned char[2 + this->m_SwitchCases.size() * 6];


	setLength(2 + this->m_SwitchCases.size() * 6);
	this->m_pByteCode[0] = getOpcode();
	this->m_pByteCode[1] = this->m_SwitchCases.size();

	for(it = this->m_SwitchCases.begin(); it != this->m_SwitchCases.end(); it++)
	{
		// value
		*(int*)&this->m_pByteCode[2 + i * 6]		= (*it)->m_iValue;
		// rel address
		*(short*)&this->m_pByteCode[6 + i * 6]		= this->m_pLabelCollector->getAddress((*it)->m_szJmpLabel) - (getAddress() + 8 + i * 6);

		i++;
	}
}
