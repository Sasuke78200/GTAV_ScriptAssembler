#ifndef __INSTRUCTION_SETSTATIC_H__
#define __INSTRUCTION_SETSTATIC_H__


class InstructionSetStatic : public Instruction
{
public:

	InstructionSetStatic();
	~InstructionSetStatic();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();
	bool				Process(unsigned char* a_aByteCode);

	void				setIndex(int a_iIndex);
	int					getIndex();

private:
	// 1 byte	- opcode
	// 1~2		- index

	unsigned char m_aByteCode[3];



};




#endif