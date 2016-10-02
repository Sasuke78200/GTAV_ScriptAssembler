#include "../main.h"


AstFunctionDecl::AstFunctionDecl()
{
	this->m_iArgCount		= 0;
	this->m_iReturnCount	= 0;
	this->m_szFunctionName	= "";
}

AstFunctionDecl::~AstFunctionDecl()
{

}

std::string AstFunctionDecl::getType()
{
	return "FunctionDecl";
}

void AstFunctionDecl::setName(std::string a_szName)
{
	this->m_szFunctionName = a_szName;
}

std::string AstFunctionDecl::getName()
{
	return this->m_szFunctionName;
}

void AstFunctionDecl::setArgCount(int a_iArgCount)
{
	this->m_iArgCount = a_iArgCount;
}
int AstFunctionDecl::getArgCount()
{
	return this->m_iArgCount;
}

void AstFunctionDecl::setReturnCount(int a_iRetCount)
{
	this->m_iReturnCount = a_iRetCount;
}
int AstFunctionDecl::getReturnCount()
{
	return this->m_iReturnCount;
}

