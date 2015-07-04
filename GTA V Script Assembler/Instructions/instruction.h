#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__


class Instruction
{
public:

	Instruction();
	~Instruction();


	void						setOpcode(unsigned char a_byOpcode);
	unsigned char				getOpcode();


	unsigned char				getLength();
	void						setLength(unsigned char a_byLength);


	virtual	unsigned char*		getByteCode() = 0;
	virtual bool				Process(std::string a_szAssemblyLine) = 0;

protected:

	unsigned char	l_byOpcode;
	unsigned char	l_byLength;

};





#endif