#ifndef __INSTRUCTION_SETFRAME_H__
#define __INSTRUCTION_SETFRAME_H__


class InstructionSetFrame : public Instruction
{
public:
	InstructionSetFrame();
	~InstructionSetFrame();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);



private:
	// 1 byte - opcode
	// 1/2 bytes - frame index
	unsigned char	m_aByteCode[3];
};





#endif