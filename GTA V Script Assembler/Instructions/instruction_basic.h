#ifndef __INSTRUCTION_BAISC_H__
#define __INSTRUCTION_BASIC_H__




class InstructionBasic : public Instruction
{
public:
	InstructionBasic();
	~InstructionBasic();


	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();
	bool				Process(unsigned char* a_aByteCode);

};





#endif