#include "../main.h"



Instruction::Instruction()
{	
	l_byOpcode = 0xFF;
	l_byLength = 0xFF;
}

Instruction::~Instruction()
{
}

void Instruction::setOpcode(unsigned char a_byOpcode)
{
	this->l_byOpcode = a_byOpcode;
}

unsigned char Instruction::getOpcode()
{
	return this->l_byOpcode;
}

unsigned char Instruction::getLength()
{
	return this->l_byLength;
}

void Instruction::setLength(unsigned char a_byLength)
{
	this->l_byLength = a_byLength;
}
