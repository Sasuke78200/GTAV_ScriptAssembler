#ifndef __INSTRUCTION_ARRAYP_GET_H__
#define __INSTRUCTION_ARRAYP_GET_H__

/*

NOTE:
		Arrays works weirdly in GTA V.

		The first cell reserved, it always contain the array size in element.
		I say element because structures are also used as arrays.
		So for example if an array contains 4 elements (4 integer, or 4 instance of a structure), then the first cell will be set to 4.
		But the array size will be different than 4 integers (64 bits integers).
		It will be, (4 * element_size + 1) * sizeof(__int64)
		element_size is set at 1 for a simple array otherwhise it represent the size of the structure.


		That element_size is the index used in all the opcode that manipulate arrays, such as setarray, getarray, getarrayp.

		-------------------------------------------------
		This is how the stack has to be set, before using this opcode
		stack - 0x08 -> index
		stack + 0x00 -> array
*/



class InstructionArrayGetP : public Instruction
{
public:

	InstructionArrayGetP();
	~InstructionArrayGetP();



	unsigned char*		getByteCode();
	bool				Process(std::string a_szAssemblyLine);
	bool				Process(unsigned char* a_pByteCode);
	std::string			toString();

	
	void				setElementSize(unsigned int a_iSize);
	unsigned int		getElementSize();


private:
	// 1 byte		- opcode
	// 2~3 bytes	- element size (multiplied by 8) ?
	//					maybe used if it's a structure, for example a structure with 4 element, we're going to use 4
	//					for a simple array, we always use 1


	unsigned char m_aByteCode[3];


};



#endif