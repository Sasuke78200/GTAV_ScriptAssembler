#include "../main.h"


BasicBlock::BasicBlock()
{
	this->m_pPredecessor		= 0;
	m_bVisited					= false;	
}

BasicBlock::~BasicBlock()
{

}

std::vector<Instruction*>*	BasicBlock::getInstructions()
{
	return &this->m_Instructions;
}

void BasicBlock::addInstruction(Instruction* a_pInstruction)
{
	this->m_Instructions.push_back(a_pInstruction);
}

bool BasicBlock::hasJmp()
{
	Instruction* l_pInstr = this->m_Instructions.back();
	return l_pInstr->getOpcode() >= 85 && l_pInstr->getOpcode() <= 92;
}

bool BasicBlock::hasUnconditionnalJmp()
{
	return this->m_Instructions.back()->getOpcode() == 85;
}

void BasicBlock::addSuccessor(BasicBlock* a_pSuccessor)
{
//	printf("AddSuccessor(%X) for %X\n", a_pSuccessor->getInstructions()->front()->getAddress(),	this->m_Instructions.front()->getAddress());
	this->m_Successors.push_back(a_pSuccessor);
}
std::vector<BasicBlock*>* BasicBlock::getSuccessors()
{
	return &this->m_Successors;
}

void BasicBlock::setPredecessor(BasicBlock* a_pPredecessor)
{
	if(this->m_pPredecessor != 0)
	{
		
	}
	this->m_pPredecessor = a_pPredecessor;
}

BasicBlock* BasicBlock::getPredecessor()
{
	return this->m_pPredecessor;
}

void BasicBlock::setIsVisited(bool a_bVisited)
{
	this->m_bVisited = a_bVisited;
}

bool BasicBlock::hasBeenVisited()
{
	return this->m_bVisited;
}

int BasicBlock::getBlockAddress()
{
	return this->m_Instructions.front()->getAddress();
}

int BasicBlock::getEdgesCount()
{
	return this->m_Successors.size();
}