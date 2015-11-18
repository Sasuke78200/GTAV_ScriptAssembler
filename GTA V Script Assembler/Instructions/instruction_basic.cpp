#include "../main.h"


InstructionBasic::InstructionBasic()
{
	setOpcode(0);
	setLength(1);
}

InstructionBasic::~InstructionBasic()
{

}

unsigned char* InstructionBasic::getByteCode()
{
	return &l_byOpcode;
}

bool InstructionBasic::Process(std::string a_szAssemblyLine)
{
	return true;
}

std::string InstructionBasic::toString()
{
	return getName();
}


bool InstructionBasic::Process(unsigned char* a_aByteCode)
{
	switch(*a_aByteCode)
	{
	case 0:
		setName("nop");
		break;
	case 1:
		setName("addi");
		break;
	case 2:
		setName("subi");
		break;
	case 3:
		setName("muli");
		break;
	case 4:
		setName("divi");
		break;
	case 5:
		setName("modi");
		break;		
	case 6:
		setName("noti");
		break;
	case 7:
		setName("negi");
		break;		
	case 8:
		setName("icmpeq");
		break;
	case 9:
		setName("icmpne");
		break;
	case 10:
		setName("icmpgt");
		break;
	case 11:
		setName("icmpge");
		break;
	case 12:
		setName("icmplt");
		break;
	case 13:
		setName("icmple");
		break;
	case 14:
		setName("fadd");
		break;
	case 15:
		setName("fsub");
		break;
	case 16:
		setName("fmul");
		break;
	case 17:
		setName("fdiv");
		break;
	case 18:
		setName("fmod");
		break;		
	case 19:
		setName("fneg");
		break;
	case 20:
		setName("fcmpeq");
		break;
	case 21:
		setName("fcmpne");
		break;
	case 22:
		setName("fcmpgt");
		break;
	case 23:
		setName("fcmpge");
		break;
	case 24:
		setName("fcmplt");
		break;
	case 25:
		setName("fcmple");
		break;
	case 26:
		setName("vadd");
		break;
	case 27:
		setName("vsub");
		break;
	case 28:
		setName("vmul");
		break;
	case 29:
		setName("vdiv");
		break;
	case 30:
		setName("vneg");
		break;
	case 31:
		setName("iand");
		break;
	case 32:
		setName("ior");
		break;
	case 33:
		setName("ixor");
		break;
	case 34:
		setName("itof");
		break;
	case 35:
		setName("ftoi");
		break;
	case 36:
		setName("dup2");
		break;
	case 42:
		setName("dup");
		break;
	case 43:
		setName("drop");
		break;

	case 47:
		setName("pget");
		break;
	case 48:
		setName("pset");
		break;
	case 49:
		setName("ppeekset");
		break;
	case 100:
		setName("jooat");
		break;
	}
	return true;
}