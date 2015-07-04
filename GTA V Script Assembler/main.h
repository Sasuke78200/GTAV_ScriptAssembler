#ifndef __MAIN_H__
#define __MAIN_H__

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


#include "utils.h"

// all the instruction
// each instruction has his own class
#include "Instructions/instruction.h"
#include "Instructions/instruction_basic.h" // on byte instruction, like add, mul, comparisons, etc ...
#include "Instructions/instruction_ipush.h"
#include "Instructions/instruction_fpush.h"


#include "assembler.h"

#endif