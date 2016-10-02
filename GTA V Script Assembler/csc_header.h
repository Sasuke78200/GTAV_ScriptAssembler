#ifndef __CSC_HEADER_H__
#define __CSC_HEADER_H__


class CscHeader : public scrHeader
{
public:

	CscHeader();
	~CscHeader();


	void setScriptName(std::string a_szName);

	void WriteToFile(std::ofstream* a_pFileStream, 
		unsigned char**		a_pByteCode,
		NativeCollector*	a_pNativeCollector,
		StringCollector*	a_pStringCollector);

	void ReadFromFile(std::ifstream* a_pFileStream);


	std::string		getName();

	/* Code page informations */
	int				getCodePageCount();
	int				getCodePageLength(unsigned int a_uiPage);
	int				getCodePageOffset(std::ifstream* a_pFileStream, int a_uiPage);
	void			setByteCodeLength(unsigned int a_uiLength);
	unsigned int	getByteCodeLength();

	/* String page informations */
	int				getStringPageCount();
	int				getStringPageLength(unsigned int a_uiPage);
	int				getStringPageOffset(std::ifstream* a_pFileStream, int a_uiPage);
	int				getStringsLength();

	/* Natives informations */
	int				getNativesCount();
	int				getNativesOffset();
	__int64			getNativeHash(int a_iIndex);

private:
	/* scrProgram/csc Header*/
	unsigned int	m_uiMagicNumber;			// + 0x00
	unsigned int	m_uiPgBase;					// + 0x04
	unsigned int	m_uiByteCodePageOffset;		// + 0x08
	unsigned int 	m_uiUnk000C;				// + 0x0C
	unsigned int 	m_uiByteCodeLength;			// + 0x10
	unsigned int 	m_uiArgCount;				// + 0x14
	unsigned int 	m_uiStaticsCount;			// + 0x18
	unsigned int 	m_uiGlobalsCount;			// + 0x1C
	unsigned int 	m_uiNativeCount;			// + 0x20
	unsigned int	m_uiStaticsOffset;			// + 0x24
	unsigned int 	m_uiGlobalsOffset;			// + 0x28
	unsigned int	m_uiNativesOffset;			// + 0x2C
	char			m_Unk0030[0x08];			// + 0x30
	unsigned int 	m_uiScriptHash;				// + 0x38
	unsigned int 	m_uiScriptCount;			// + 0x3C
	unsigned int	m_uiScriptNameOffset;		// + 0x40
	unsigned int	m_uiStringsOffset;			// + 0x44
	unsigned int 	m_uiStringsLength;			// + 0x48
	/* end of the header*/


	std::string			m_szScriptName;
	std::vector<__int64>
						m_NativesHashes;
};

#endif