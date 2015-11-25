#include "../main.h"



Instruction::Instruction()
{	
	m_byOpcode	= 0xFF;
	m_iLength	= -1;
	m_szName	= "instruction";
	m_iAddress	= -1;
}

Instruction::~Instruction()
{
}

void Instruction::setOpcode(unsigned char a_byOpcode)
{
	this->m_byOpcode = a_byOpcode;
}

unsigned char Instruction::getOpcode()
{
	return this->m_byOpcode;
}

int Instruction::getLength()
{
	return this->m_iLength;
}

void Instruction::setLength(int a_iLength)
{
	this->m_iLength = a_iLength;
}

void Instruction::setName(std::string a_szName)
{
	this->m_szName = a_szName;
}

std::string Instruction::getName()
{
	return this->m_szName;
}

Instruction* Instruction::allocFromOpcode(unsigned char l_bOpcode)
{
	if((l_bOpcode >= 0 && l_bOpcode <= 36) 
		|| l_bOpcode == 42 || l_bOpcode == 43
		|| (l_bOpcode >= 47 && l_bOpcode <= 51)
		|| l_bOpcode == 100)
	{
		return new InstructionBasic();
	}
	else if(l_bOpcode >= 85 && l_bOpcode <= 92)
	{
		return new InstructionJmp();
	}
	else if(l_bOpcode == 93)
	{
		return new InstructionCall();
	}
	else if(l_bOpcode == 45)
	{
		return new InstructionEnter();
	}
	else if(l_bOpcode == 56 || l_bOpcode == 77)
	{
		return new InstructionGetFrame();
	}
	else if(l_bOpcode == 55 || l_bOpcode == 76)
	{
		return new InstructionGetFrameP();
	}
	else if(l_bOpcode == 83 || l_bOpcode == 95)
	{
		return new InstructionGetGlobal();
	}
	else if(l_bOpcode == 82 || l_bOpcode == 94)
	{
		return new InstructionGetGlobalP();
	}
	else if(l_bOpcode == 44)
	{
		return new InstructionNative();
	}
	else if(l_bOpcode == 46)
	{
		return new InstructionRet();
	}	
	else if(l_bOpcode == 57 || l_bOpcode == 78)
	{
		return new InstructionSetFrame();
	}
	else if(l_bOpcode == 84 || l_bOpcode == 96)
	{
		return new InstructionSetGlobal();
	}
	else if(l_bOpcode == 99)
	{
		return new InstructionsPush();
	}
	else if(l_bOpcode == 101)
	{
		return new InstructionStrcpy();
	}
	else if((l_bOpcode >= 109 && l_bOpcode <= 117) || l_bOpcode == 37 || l_bOpcode == 67 || l_bOpcode == 97 || l_bOpcode == 40)
	{
		return new InstructioniPush();
	}
	else if((l_bOpcode >= 118 && l_bOpcode <= 126) || l_bOpcode == 41)
	{
		return new InstructionfPush();
	}
	else if(l_bOpcode == 58 || l_bOpcode == 79)
	{
		return new InstructionGetStaticP();
	}
	else if(l_bOpcode == 59 || l_bOpcode == 80)
	{
		return new InstructionGetStatic();
	}
	else if(l_bOpcode == 60 || l_bOpcode == 81)
	{
		return new InstructionSetStatic();
	}
	else if(l_bOpcode == 98)
	{
		return new InstructionSwitch();
	}
	else if(l_bOpcode == 52 || l_bOpcode == 73)
	{
		return new InstructionArrayGetP();
	}
	else if(l_bOpcode == 53 || l_bOpcode == 74)
	{
		return new InstructionArrayGet();
	}
	else if(l_bOpcode == 54 || l_bOpcode == 75)
	{
		return new InstructionArraySet();
	}
	else if(l_bOpcode == 65 || l_bOpcode == 71)
	{
		return new InstructionPGetImm();
	}
	else if(l_bOpcode == 66 || l_bOpcode == 72)
	{
		return new InstructionPSetImm();
	}
	else if(l_bOpcode == 63 || l_bOpcode == 64 || l_bOpcode == 70)
	{
		return new InstructionPAdd();
	}
	else if(l_bOpcode == 61 || l_bOpcode == 68)
	{
		return new InstructionAddImm();
	}
	else if(l_bOpcode == 62 || l_bOpcode == 69)
	{
		return new InstructionMulImm();
	}
	return 0;
}

void Instruction::setAddress(int a_iAddress)
{
	this->m_iAddress = a_iAddress;
}

int Instruction::getAddress()
{
	return this->m_iAddress;
}