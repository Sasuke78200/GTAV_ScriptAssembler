#ifndef __INSTRUCTION_GETFRAME_H__
#define __INSTRUCTION_GETFRAME_H__


class InstructionGetFrame : public Instruction
{
public:
	InstructionGetFrame();
	~InstructionGetFrame();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();



private:
	// 1 byte - opcode
	// 1/2 bytes - frame index
	unsigned char	m_aByteCode[3];
};





#endif