#include "main.h"


FunctionCollector::FunctionCollector()
{
}

FunctionCollector::~FunctionCollector()
{
	std::vector<std::vector<Instruction*>*>::iterator it;
	for(it = m_functions.begin(); it != m_functions.end(); it++)
	{
		(*it)->clear();
		delete *it;
	}
	m_functions.clear();
}

void FunctionCollector::NewFunction()
{
	m_functions.push_back(new std::vector<Instruction*>);
}

void FunctionCollector::AddInstruction(Instruction* a_pInstruction)
{
	m_functions.back()->push_back(a_pInstruction);
}

std::vector<Instruction*>* FunctionCollector::getFunction(int a_iIndex)
{
	return m_functions[a_iIndex];
}

int FunctionCollector::getCount()
{
	return m_functions.size();
}
