#ifndef __INSTRUCTION_P_SET_IMM_H__
#define __INSTRUCTION_P_SET_IMM_H__


class InstructionPSetImm : public Instruction
{
public:

	InstructionPSetImm();
	~InstructionPSetImm();



	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	bool				Process(unsigned char* a_pByteCode);
	std::string			toString();

	
	void				setIndex(unsigned int a_iIndex);
	unsigned int		getIndex();


private:
	// 1 byte		- opcode
	// 2~3 bytes	- Where in the pointer we write


	unsigned char m_aByteCode[3];


};



#endif