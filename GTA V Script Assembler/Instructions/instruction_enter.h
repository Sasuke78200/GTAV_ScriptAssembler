#ifndef __INSTRUCTION_ENTER_H__
#define __INSTRUCTION_ENTER_H__


class InstructionEnter : public Instruction
{
public:

	InstructionEnter();
	~InstructionEnter();


	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);


private:

	// 1 byte	- opcode
	// 1 byte	- arg count
	// 2 bytes	- stack alloc count
	// 1 byte	- who knows ? // it will always be 0 for us
	unsigned char	m_aByteCode[5];



};



#endif