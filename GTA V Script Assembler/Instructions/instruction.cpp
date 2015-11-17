#include "../main.h"



Instruction::Instruction()
{	
	l_byOpcode	= 0xFF;
	l_byLength	= 0xFF;
	l_szName	= "instruction";
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

void Instruction::setName(std::string a_szName)
{
	this->l_szName = a_szName;
}

std::string Instruction::getName()
{
	return this->l_szName;
}