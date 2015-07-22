#ifndef __INSTRUCTION_GETGLOBAL_H__
#define __INSTRUCTION_GETGLOBAL_H__


class InstructionGetGlobal : public Instruction
{
public:
	InstructionGetGlobal();
	~InstructionGetGlobal();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);


private:

	// 1 byte - opcode
	// 2~3 bytes - global id

	unsigned char	m_aByteCode[4];


};


#endif