#ifndef __YSC_HEADER_H__
#define __YSC_HEADER_H__

// the header of each .ysc is the exact representation of a scrProgram
// except that the vmt is the magic number

/*
class scrProgram
{
	void*			__vmt;				// + 0x00
	void*			m_Unk0008;			// + 0x08
	unsigned char**	m_aByteCode;		// + 0x10
	unsigned int 	m_Unk000C;			// + 0x18
	unsigned int 	m_uiByteCodeLength;	// + 0x1C
	unsigned int 	m_iArgCount;		// + 0x20
	unsigned int 	m_uiStaticsCount;	// + 0x24
	unsigned int 	m_uiGlobalsCount;	// + 0x28	
	unsigned int 	m_uiNativeCount;	// + 0x2C
	unsigned int*	m_pStatics;			// + 0x30
	unsigned int* 	m_pGlobals;			// + 0x3C
	//void(**m_pNatives)(NativeArg*);		// + 0x40	
	NativeFunction* m_pNatives;			// + 0x40
	unsigned int 	m_Unk0048;			// + 0x48
	unsigned int 	m_Unk004C;			// + 0x4C
	unsigned int*	m_NullPtr;			// + 0x50
	unsigned int 	m_uiScriptHash;		// + 0x58
	unsigned int 	m_uiScriptCount;	// + 0x5C
	char*			m_szScriptName;		// + 0x60
	char**			m_szStrings;		// + 0x68
	unsigned int 	m_uiStringsLength;	// + 0x70
	int 			m_iUnk0074;
	int				m_iUnk0078;
	int 			m_iUnk007C;
};
*/

class YscHeader
{
public:

	YscHeader();
	~YscHeader();


	void SetByteCodeLength(unsigned int a_uiLength);
	void SetScriptName(std::string a_szName);

	void WriteToFile(std::ofstream* a_pFileStream, 
		unsigned char**		a_pByteCode,
		NativeCollector*	a_pNativeCollector,
		StringCollector*	a_pStringCollector);

private:
	/* scrProgram/ysc Header*/
	unsigned long long	m_uiMagicNumber;			
	unsigned long long	m_uiUnk0008;				
	unsigned long long	m_uiByteCodePageOffset;		
	unsigned int		m_uiUnk000C;				
	unsigned int		m_uiByteCodeLength;			
	unsigned int		m_uiArgCount;
	unsigned int 		m_uiStaticsCount;
	unsigned int 		m_uiGlobalsCount;
	unsigned int 		m_uiNativeCount;
	unsigned long long	m_uiStaticsOffset;
	unsigned long long	m_uiGlobalsOffset;
	unsigned long long	m_uiNativesOffset;
	unsigned int 		m_uiUnk0048;
	unsigned int 		m_uiUnk004C;
	unsigned long long 	m_uiUnk0050;
	unsigned int 		m_uiScriptHash;
	unsigned int 		m_uiScriptCount;
	unsigned long long	m_uiScriptNameOffset;
	unsigned long long	m_uiStringsOffset;
	unsigned int		m_uiStringsLength;
	int 				m_iUnk0074;
	int					m_iUnk0078;
	int 				m_iUnk007C;
	/* end of the header*/


	std::string			m_szScriptName;
};




#endif