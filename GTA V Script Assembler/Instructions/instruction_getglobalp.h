#ifndef __INSTRUCTION_GETGLOBALP_H__
#define __INSTRUCTION_GETGLOBALP_H__


class InstructionGetGlobalP : public Instruction
{
public:
	InstructionGetGlobalP();
	~InstructionGetGlobalP();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();

private:

	// 1 byte - opcode
	// 2~3 bytes - global id

	unsigned char	m_aByteCode[4];


};


#endif