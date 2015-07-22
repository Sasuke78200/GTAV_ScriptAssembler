#ifndef __INSTRUCTION_IPUSH_H__
#define __INSTRUCTION_IPUSH_H__


class InstructioniPush : public Instruction
{
public:

	InstructioniPush();
	~InstructioniPush();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	void				setValue(int a_iValue);

private:

	unsigned char	m_aByteCode[5];		// max size for an ipush is 5 bytes (1 for the opcode & 4 for the value)


};




#endif