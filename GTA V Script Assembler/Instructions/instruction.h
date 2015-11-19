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

	void						setName(std::string a_szName);
	std::string					getName();

	void						setAddress(int a_iAddress);
	int							getAddress();

	virtual	unsigned char*		getByteCode() = 0;
	virtual bool				Process(std::string a_szAssemblyLine) = 0;
	virtual bool				Process(unsigned char*) = 0;
	virtual std::string			toString() = 0;

	static Instruction*			allocFromOpcode(unsigned char l_bOpcode);

protected:

	unsigned char	m_byOpcode;
	unsigned char	m_byLength;
	std::string		m_szName;
	int				m_iAddress;	// address of this instruction in the program
};





#endif