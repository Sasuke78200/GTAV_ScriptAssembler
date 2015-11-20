#ifndef __INSTRUCTION_SET_ARRAY_H__
#define __INSTRUCTION_SET_ARRAY_H__

/*

NOTE:
		Arrays works weirdly in GTA V.

		The first cell is reserved, it always contains the count of elements.
		I says element because structures are also used as arrays.
		So for example if an array contains 4 elements (4 integers, or 4 instances of a structure), then the first cell will be set to 4.
		But the array size will be different than 4 integers (64 bits integers).
		It will be, (4 * element_size + 1) * sizeof(__int64)
		element_size is set to 1 for a simple array otherwhise it represents the size of the structure.


		That element_size is the index used in all the opcode that manipulate arrays, such as setarray, getarray, getarrayp.

		-------------------------------------------------
		This is how the stack has to be set, before using this opcode
		stack - 0x10 -> val
		stack - 0x08 -> index
		stack + 0x00 -> array
		
		example: 

		to do something like, 
			int my_array[7];
			my_array[5] = 1337;

		ipush 1337
		ipush 5			// you don't have to care about the first cell, the script machine increment that index when executing the code
		any_instruction_to_get_my_array_here (getglobal, getglobalp, any instruction that will return you an array)
		setarray 1
*/



class InstructionSetArray : public Instruction
{
public:

	InstructionSetArray();
	~InstructionSetArray();



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