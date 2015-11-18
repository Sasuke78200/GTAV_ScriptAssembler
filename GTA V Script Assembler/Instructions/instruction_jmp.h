#ifndef __INSTRUCTION_JMP_H__
#define __INSTRUCTION_JMP_H__



class InstructionJmp : public Instruction
{
public:

	InstructionJmp();
	~InstructionJmp();


	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();


	void				setLabelCollector(LabelCollector* a_pLabelCollector);
	bool				Process(unsigned char* a_aByteCode);
	short				getJumpAddress();
	void				setLabel(std::string a_szLabel);

private:

	unsigned char	m_aByteCode[3];
	LabelCollector*	m_pLabelCollector;
	std::string		m_szJmpLabel;
	bool			m_bJumpAlreadyComputed;
};


#endif