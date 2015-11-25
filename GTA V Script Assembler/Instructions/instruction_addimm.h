#ifndef __INSTRUCTION_ADDIMM_H__
#define __INSTRUCTION_ADDIMM_H__


class InstructionAddImm : public Instruction
{
public:
	InstructionAddImm();
	~InstructionAddImm();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	bool				Process(unsigned char* a_pByteCode);
	std::string			toString();

	void				setValue(short a_iValue);
	short				getValue();

private:
	// 1 byte		- opcode
	// 1~2 bytes	- value

	unsigned char m_aByteCode[3];
};




#endif