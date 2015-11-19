#ifndef __INSTRUCTION_CALL_H__
#define __INSTRUCTION_CALL_H__


class InstructionCall : public Instruction
{
public:

	InstructionCall();
	~InstructionCall();
	
	void				setLabelCollector(LabelCollector* a_pLabelCollector);
	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();
	bool				Process(unsigned char* a_aByteCode);
	int					getCallAddress();
	void				setLabel(std::string a_szLabel);
private:


	// 1 byte for the opcode
	// 3 bytes for the absolute address
	unsigned char	m_aByteCode[4];


	LabelCollector*	m_pLabelCollector;
	std::string		m_szJmpLabel;
	bool			m_bJumpAlreadyComputed;

};




#endif