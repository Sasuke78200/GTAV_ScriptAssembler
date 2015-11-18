#include "../main.h"


InstructionCall::InstructionCall(LabelCollector* a_pLabelCollector)
{
	setOpcode(93);
	setLength(4);
	setName("call");
	this->m_pLabelCollector = a_pLabelCollector;
}

InstructionCall::~InstructionCall()
{

}


unsigned char* InstructionCall::getByteCode()
{	
	*(int*)m_aByteCode = this->m_pLabelCollector->getAddress(this->m_szJmpLabel) << 8; // it's an absolute address + plus we only need 24 bits for the address
	m_aByteCode[0] = getOpcode();
	return this->m_aByteCode;
}

bool InstructionCall::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;	

	m_szJmpLabel = a_szAssemblyLine;
	return true;
}

std::string InstructionCall::toString()
{
	// TODO: Print the function name
	return getName() + "";
}


bool InstructionCall::Process(unsigned char* a_aByteCode)
{
	return true;
}