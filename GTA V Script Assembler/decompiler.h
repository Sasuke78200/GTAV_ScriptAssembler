#ifndef __DECOMPILER_H__
#define __DECOMPILER_H__

class Decompiler
{
public:

	Decompiler();
	~Decompiler();

	bool	DecompileFile(char* a_szBinaryPath, char* a_szOutputPath);

	bool	ValidateBinary();
	void	ReadByteCode();
	void	ConvertToInstructions();
	void	ProcessInstructions();
	void	CollectFunctions();
	bool	BuildAST();
	bool	BuildAstForFunction(int a_iIndex);
	int		getOpcodeLen(unsigned char a_bOpcode);
	int		getOpcodeLenByAddr(unsigned int a_uiAddress);

private:

	std::vector<BasicBlock*>	generateBasicBlocks(std::vector<Instruction*>* a_pFunction);


	std::ifstream*				m_pBinaryFile;
	scrHeader*					m_pScrHeader;
	StringCollector				m_stringCollector;
	LabelCollector				m_labelCollector;
	NativeCollector				m_nativeCollector;
	FunctionCollector			m_functionCollector;

	unsigned char**				m_aByteCode;

	std::vector<Instruction*>	m_Instructions;
};





#endif