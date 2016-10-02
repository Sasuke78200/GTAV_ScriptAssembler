#ifndef __AST_FUNCTION_DECL_H__
#define __AST_FUNCTION_DECL_H__

class AstFunctionDecl : public AstNode
{
public:

	AstFunctionDecl();
	~AstFunctionDecl();

	std::string		getType();

	void			setName(std::string a_szName);
	std::string		getName();

	void			setArgCount(int a_iArgCount);
	int				getArgCount();

	void			setReturnCount(int a_iRetCount);
	int				getReturnCount();

private:

	std::string		m_szFunctionName;
	int				m_iArgCount;
	int				m_iReturnCount;

};




#endif