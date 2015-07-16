#ifndef __MAIN_H__
#define __MAIN_H__

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "utils.h"

// rapidjson
#include "rapidjson/document.h" 
#include "rapidjson/filereadstream.h"

// collectors
#include "label_collector.h"
#include "native_collector.h"
#include "string_collector.h"

// all the instruction
#include "Instructions/instruction.h"
#include "Instructions/instruction_basic.h" // on byte instruction, like add, mul, comparisons, etc ...
#include "Instructions/instruction_ipush.h"
#include "Instructions/instruction_fpush.h"
#include "Instructions/instruction_native.h"
#include "Instructions/instruction_enter.h"
#include "Instructions/instruction_ret.h"
#include "Instructions/instruction_jmp.h"
#include "Instructions/instruction_spush.h"
#include "Instructions/instruction_call.h"
#include "Instructions/instruction_getframe.h"
#include "Instructions/instruction_setframe.h"

#include "ysc_header.h"

#include "assembler.h"


#define ASSEMBLY_SPACE		" \t"


#endif