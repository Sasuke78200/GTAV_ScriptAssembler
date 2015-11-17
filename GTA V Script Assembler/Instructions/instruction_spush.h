#ifndef __INSTRUCTION_SPUSH_H__
#define __INSTRUCTION_SPUSH_H__

class InstructionsPush : public Instruction
{
public:
	
	InstructionsPush(StringCollector* a_pStringCollector);
	~InstructionsPush();

	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	std::string			toString();

private:

	// 1~5 bytes for the string index
	// 1 byte for the opcode (99)	
	unsigned char				m_aByteCode[5];

	StringCollector*			m_pStringCollector;
	InstructioniPush			m_StringIndexInstruction;

};


#endif