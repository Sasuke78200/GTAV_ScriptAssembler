#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__


class Assembler
{
public:
	Assembler();
	~Assembler();

	bool AssembleFile(char* a_szAssemblyPath, char* a_szOuputScriptPath);

private:
	void CollectCode();
	void CleanCode();
	bool ParseCode();
	void ConstructBinary(std::ofstream*	a_pBinaryStream);



	std::ifstream*							m_pAssemblyFile;
	std::map<unsigned int, std::string>		m_AssemblyLines;
	std::map<unsigned int, Instruction*>	m_ByteCode;


	NativeCollector							m_NativeCollector;
	LabelCollector							m_LabelCollector;
};



#endif