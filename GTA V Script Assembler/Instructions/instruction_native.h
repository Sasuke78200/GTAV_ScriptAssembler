#ifndef __INSTRUCTION_NATIVE_H__
#define __INSTRUCTION_NATIVE_H__



class InstructionNative : public Instruction
{
public:
	InstructionNative();
	~InstructionNative();


	void				setNativeCollector(NativeCollector* a_pNativeCollector);
	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();
	bool				Process(unsigned char* a_aByteCode);	

	int					getArgCount();
	int					getReturnCount();

private:
	// 1 byte	- opcode
	// 1 byte	- arg count + return count
	// 2 bytes	- native id
	unsigned char	m_aByteCode[4];



	NativeCollector*	m_pNativeCollector;

};



#endif