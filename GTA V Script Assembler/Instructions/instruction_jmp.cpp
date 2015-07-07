#include "../main.h"






InstructionJmp::InstructionJmp()
{
	setOpcode(-1);
	setLength(3);
}

InstructionJmp::~InstructionJmp()
{
}


unsigned char* InstructionJmp::getByteCode()
{
	unsigned int l_uiLabelAddress;

	//TODO: Verify that this label exist in the class assembler
	l_uiLabelAddress = this->m_pLabelCollector->getAddress(this->m_szJmpLabel);

	*(short*)&m_aByteCode[1]	= l_uiLabelAddress - (m_uiOurAddress + getLength());
	m_aByteCode[0]				= getOpcode();


	return m_aByteCode;
}

bool InstructionJmp::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;
	
	m_szJmpLabel = a_szAssemblyLine;
	return true;
}
 

void InstructionJmp::setLabelCollector(LabelCollector* a_pLabelCollector)
{
	this->m_pLabelCollector = a_pLabelCollector;
}

void InstructionJmp::setAddress(unsigned int a_uiAddress)
{
	this->m_uiOurAddress = a_uiAddress;
}