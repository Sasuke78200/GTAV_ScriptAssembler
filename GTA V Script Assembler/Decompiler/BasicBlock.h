#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

class BasicBlock
{
public:
	BasicBlock();
	~BasicBlock();

	std::vector<Instruction*>*	getInstructions();
	void						addInstruction(Instruction* a_pInstruction);

	bool						hasJmp();
	bool						hasUnconditionnalJmp();
	void						addSuccessor(BasicBlock* a_pSuccessor);
	std::vector<BasicBlock*>*	getSuccessors();

	void						setPredecessor(BasicBlock* a_pPredecessor);
	BasicBlock*					getPredecessor();

	void						setIsVisited(bool a_bVisited);
	bool						hasBeenVisited();

	int							getBlockAddress();
	int							getEdgesCount();

private:

	BasicBlock*					m_pPredecessor;
	std::vector<BasicBlock*>	m_Successors;	
	std::vector<Instruction*>	m_Instructions;
	bool						m_bVisited;

};



#endif