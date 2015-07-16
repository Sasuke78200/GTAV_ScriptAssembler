#ifndef __INSTRUCTION_GETFRAMEP_H__
#define __INSTRUCTION_GETFRAMEP_H__


class InstructionGetFrameP : public Instruction
{
public:
	InstructionGetFrameP();
	~InstructionGetFrameP();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);



private:
	// 1 byte - opcode
	// 1/2 bytes - frame index
	unsigned char	m_aByteCode[3];
};





#endif