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
#include "Instructions/instruction_basic.h" // one byte instruction, like add, mul, comparisons, etc ...
#include "Instructions/instruction_ipush.h"
#include "Instructions/instruction_fpush.h"
#include "Instructions/instruction_native.h"
#include "Instructions/instruction_enter.h"
#include "Instructions/instruction_ret.h"
#include "Instructions/instruction_jmp.h"
#include "Instructions/instruction_spush.h"
#include "Instructions/instruction_call.h"
#include "Instructions/instruction_getframep.h"
#include "Instructions/instruction_getframe.h"
#include "Instructions/instruction_setframe.h"
#include "Instructions/instruction_getglobalp.h"
#include "Instructions/instruction_getglobal.h"
#include "Instructions/instruction_setglobal.h"
#include "Instructions/instruction_strcpy.h"
#include "Instructions/instruction_setstatic.h"
#include "Instructions/instruction_getstatic.h"
#include "Instructions/instruction_getstaticp.h"
#include "Instructions/instruction_switch.h"
#include "Instructions/instruction_arrayset.h"
#include "Instructions/instruction_arrayget.h"
#include "Instructions/instruction_arraygetp.h"

#include "ysc_header.h"

#include "assembler.h"
#include "disassembler.h"

#include "commandline.h"


#define ASSEMBLY_SPACE		" \t"

//TODO: Allow us to create statics !
//		Got some ideas to allow us to do it easily
#endif