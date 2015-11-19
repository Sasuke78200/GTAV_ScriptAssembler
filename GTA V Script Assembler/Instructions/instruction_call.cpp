#include "../main.h"


InstructionCall::InstructionCall()
{
	setOpcode(93);
	setLength(4);
	setName("call");
	this->m_pLabelCollector = 0;
	m_bJumpAlreadyComputed = false;
}

InstructionCall::~InstructionCall()
{

}

void InstructionCall::setLabelCollector(LabelCollector* a_pLabelCollector)
{
	this->m_pLabelCollector = a_pLabelCollector;
}

unsigned char* InstructionCall::getByteCode()
{	
	*(int*)m_aByteCode = this->m_pLabelCollector->getAddress(this->m_szJmpLabel) << 8; // it's an absolute address + plus we only need 24 bits for the address
	m_aByteCode[0] = getOpcode();
	m_bJumpAlreadyComputed = true;
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
	return getName() + " " + this->m_szJmpLabel;
}


bool InstructionCall::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	m_bJumpAlreadyComputed = true;
	return true;
}

int InstructionCall::getCallAddress()
{
	if(m_bJumpAlreadyComputed == false)	// if the function address haven't been computed, let's compute it
		getByteCode();
	return *(int*)m_aByteCode >> 8;
}

void InstructionCall::setLabel(std::string a_szLabel)
{
	this->m_szJmpLabel = a_szLabel;
}