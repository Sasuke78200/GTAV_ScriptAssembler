#ifndef __FUNCTION_COLLECTOR_H__
#define __FUNCTION_COLLECTOR_H__

class Instruction;

class FunctionCollector
{
public:
	FunctionCollector();
	~FunctionCollector();

	void NewFunction();
	void AddInstruction(Instruction* a_pInstruction);
	std::vector<Instruction*>* getFunction(int a_iIndex);
	int getCount();

private:


	std::vector<std::vector<Instruction*>*> m_functions;

};


#endif