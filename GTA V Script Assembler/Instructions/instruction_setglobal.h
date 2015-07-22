#ifndef __INSTRUCTION_SETGLOBAL_H__
#define __INSTRUCTION_SETGLOBAL_H__


class InstructionSetGlobal : public Instruction
{
public:
	InstructionSetGlobal();
	~InstructionSetGlobal();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);


private:

	// 1 byte - opcode
	// 2~3 bytes - global id

	unsigned char	m_aByteCode[4];


};


#endif