#ifndef __INSTRUCTION_RET_H__
#define __INSTRUCTION_RET_H__



class InstructionRet : public Instruction
{
public:
	InstructionRet();
	~InstructionRet();


	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();
	bool				Process(unsigned char* a_aByteCode);

private:
	// 1 byte	- opcode
	// 1 byte	- return count
	// 1 bytes	- pop stack
	unsigned char	m_aByteCode[3];


};



#endif