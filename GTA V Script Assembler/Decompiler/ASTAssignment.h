#ifndef __AST_ASSIGNMENT_H__
#define __AST_ASSIGNMENT_H__

class AstAssignment : public AstNode
{
public:

	AstAssignment();
	~AstAssignment();

	std::string getType();

private:

	std::string		m_szIdentifier;
	AstNode*		m_pExpression;


};



#endif