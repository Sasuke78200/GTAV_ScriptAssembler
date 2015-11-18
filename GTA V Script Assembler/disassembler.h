#ifndef __DISASSEMBLER_H__
#define __DISASSEMBLER_H__

class Disassembler
{
public:

	Disassembler();
	~Disassembler();

	bool	DisassembleFile(char* a_szBinaryPath, char* a_szOutputPath);

	void	ReadByteCode();
	void	ConvertToInstructions();
	void	ProcessInstructions();
	void	PrintInstructionsToFile(char* a_szOutputPath);
	bool	ValidateBinary();
	int		getOpcodeLen(unsigned char a_bOpcode);
	int		getOpcodeLenByAddr(unsigned int a_uiAddress);

private:

	std::ifstream*				m_pBinaryFile;
	YscHeader					m_yscHeader;
	StringCollector				m_stringCollector;
	LabelCollector				m_labelCollector;

	unsigned char**				m_aByteCode;

	std::vector<Instruction*>	m_Instructions;

};





#endif