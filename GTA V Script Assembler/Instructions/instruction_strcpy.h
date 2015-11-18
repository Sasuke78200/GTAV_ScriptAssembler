#ifndef __INSTRUCTION_STRCPY_H__
#define __INSTRUCTION_STRCPY_H__

class InstructionStrcpy : public Instruction
{
public:

	InstructionStrcpy();
	~InstructionStrcpy();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();
	bool				Process(unsigned char* a_aByteCode);

private:

	// 1 byte - opcode
	// 1 byte - len

	unsigned char m_aByteCode[2];
};



#endif