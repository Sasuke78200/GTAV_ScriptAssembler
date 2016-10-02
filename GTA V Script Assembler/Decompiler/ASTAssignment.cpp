#include "../main.h"



AstAssignment::AstAssignment()
{
	this->m_szIdentifier	= "identifier";
	this->m_pExpression		= nullptr;
}

AstAssignment::~AstAssignment()
{
}

std::string AstAssignment::getType()
{
	return "Assignment";
}