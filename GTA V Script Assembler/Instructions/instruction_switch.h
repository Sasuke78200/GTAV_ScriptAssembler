#ifndef __INSTRUCTION_SWITCH_H__
#define __INSTRUCTION_SWITCH_H__


class Case
{
public:
	int				m_iValue;
	std::string		m_szJmpLabel;
};

class InstructionSwitch : public Instruction
{
public:
	InstructionSwitch();
	~InstructionSwitch();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	bool				Process(unsigned char*);
	std::string			toString();

	void				setLabelCollector(LabelCollector* a_pLabelCollector);

	int					getCaseCount();
	int					getCaseValue(int a_iIndex);
	int					getCaseJmpAddress(int a_iIndex);
	bool				addCase(int a_iVal, std::string m_szJmpLabel);

	void				freeCases();
	void				buildByteCode();
private:

	unsigned char*		m_pByteCode;
	LabelCollector*		m_pLabelCollector;
	std::vector<Case*>	m_SwitchCases;
};


#endif