#include "../main.h"






InstructionJmp::InstructionJmp()
{
	setOpcode(-1);
	setLength(3);
	setName("jmp");
	m_bJumpAlreadyComputed = false;
}

InstructionJmp::~InstructionJmp()
{
}


unsigned char* InstructionJmp::getByteCode()
{
	unsigned int l_uiLabelAddress;

	//TODO: Verify that this label exist in the class assembler
	l_uiLabelAddress = this->m_pLabelCollector->getAddress(this->m_szJmpLabel);

	*(short*)&m_aByteCode[1]	= l_uiLabelAddress - (getAddress() + getLength());
	*this->m_aByteCode			= getOpcode();

	m_bJumpAlreadyComputed = true;

	return m_aByteCode;
}

bool InstructionJmp::Process(std::string a_szAssemblyLine)
{
	if(a_szAssemblyLine.length() == 0) return false;
	
	m_szJmpLabel = a_szAssemblyLine;
	return true;
}
 
std::string InstructionJmp::toString()
{
	return getName() + " " + this->m_szJmpLabel;
}

void InstructionJmp::setLabelCollector(LabelCollector* a_pLabelCollector)
{
	this->m_pLabelCollector = a_pLabelCollector;
}


bool InstructionJmp::Process(unsigned char* a_aByteCode)
{
	setOpcode(*a_aByteCode);
	switch(*a_aByteCode)
	{
		case 85:
			setName("jmp");
			break;		
		case 86:
			setName("jmpf");
			break;		
		case 87:
			setName("jmpneq");
			break;
		case 88:
			setName("jmpeq");
			break;
		case 89:
			setName("jmpgt");
			break;
		case 90:
			setName("jmpge");
			break;
		case 91:
			setName("jmplt");
			break;
		case 92:
			setName("jmple");
			break;
	}
	m_bJumpAlreadyComputed = true;
	memcpy(this->m_aByteCode, a_aByteCode, getLength());
	return true;
}

short InstructionJmp::getJumpAddress()
{
	if(m_bJumpAlreadyComputed == false)
		getByteCode(); //compute it
	return ((short)(getAddress() + getLength())) + *(short*)&m_aByteCode[1];
}

void InstructionJmp::setLabel(std::string a_szLabel)
{
	this->m_szJmpLabel = a_szLabel;
}