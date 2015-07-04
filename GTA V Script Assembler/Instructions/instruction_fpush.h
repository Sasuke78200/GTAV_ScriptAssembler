#ifndef __INSTRUCTION_FPUSH_H__
#define __INSTRUCTION_FPUSH_H__


class InstructionfPush : public Instruction
{
public:

	InstructionfPush();
	~InstructionfPush();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);


private:
	unsigned char	m_aByteCode[5];
};




#endif