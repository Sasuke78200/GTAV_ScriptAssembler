#ifndef __INSTRUCTION_JMP_H__
#define __INSTRUCTION_JMP_H__



class InstructionJmp : public Instruction
{
public:

	InstructionJmp();
	~InstructionJmp();


	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);


	void				setLabelCollector(LabelCollector* a_pLabelCollector);
	void				setAddress(unsigned int a_uiAddress);


private:

	unsigned char	m_aByteCode[3];
	LabelCollector*	m_pLabelCollector;
	std::string		m_szJmpLabel;
	unsigned int	m_uiOurAddress;


};


#endif