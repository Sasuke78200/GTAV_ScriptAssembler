#include "main.h"


#define offset(x) (0x50000000 | (x & 0x00FFFFFF))


YscHeader::YscHeader()
{
	m_uiMagicNumber				= 0x000000014005BE20;
	m_uiUnk0008					= 0;
	m_uiByteCodePageOffset		= 0;
	m_uiUnk000C					= 0;
	m_uiByteCodeLength			= 0;
	m_uiArgCount				= 0;
	m_uiStaticsCount			= 0;
	m_uiGlobalsCount			= 0;
	m_uiNativeCount				= 0;
	m_uiStaticsOffset			= 0;
	m_uiGlobalsOffset			= 0;
	m_uiNativesOffset			= 0;
	m_uiUnk0048					= 0;
	m_uiUnk004C					= 0;
	m_uiUnk0050					= 0;
	m_uiScriptHash				= 0;
	m_uiNull					= 0;
	m_uiScriptNameOffset		= 0;
	m_uiStringsOffset			= 0;
	m_uiStringsLength			= 0;
	m_iUnk0074					= 0;
	m_iUnk0078					= 0;
	m_iUnk007C					= 0;

	m_pNativeCollector			= nullptr;
	m_szScriptName				= "";

}

YscHeader::~YscHeader()
{

}

void YscHeader::SetByteCodeLength(unsigned int a_uiLength)
{
	this->m_uiByteCodeLength = a_uiLength;
}

void YscHeader::SetScriptName(std::string a_szName)
{
	this->m_uiScriptHash = jooat((char*)a_szName.c_str(), a_szName.length());
	this->m_szScriptName = a_szName;
}

void YscHeader::SetNativeCollector(NativeCollector* a_pNativeCollector)
{
	this->m_pNativeCollector = a_pNativeCollector;
}



void YscHeader::WriteToFile(std::ofstream* a_pFileStream, unsigned char** a_pByteCode, char** a_pStrings)
{
	char					l_szScriptName[64];
	unsigned long long*		l_pByteCodePageAddr; 
	// TODO: Write everything on the file !



	// the byte code page addresses will be right after the header (0x80)
	m_uiByteCodePageOffset	= offset(0x80);
	l_pByteCodePageAddr		= new unsigned long long[(m_uiByteCodeLength / 0x4000) + 1];

	// the first code page will be right after
	l_pByteCodePageAddr[0]	= 0x80 + (((m_uiByteCodeLength / 0x4000) + 1) * sizeof(unsigned long long));

	for(unsigned int i = 1; i < (m_uiByteCodeLength / 0x4000) + 1; i++)
	{
		l_pByteCodePageAddr[i] = l_pByteCodePageAddr[i - 1] + 0x4000;
	}
	for(unsigned int i = 0; i < (m_uiByteCodeLength / 0x4000) + 1; i++)
	{
		l_pByteCodePageAddr[i] = offset(l_pByteCodePageAddr[i]);
	}

	// get the natives function count
	this->m_uiNativeCount = this->m_pNativeCollector->getNativeCount();

	// set where the natives are going to be written on the file (right after the last byte code page)
	this->m_uiNativesOffset = offset((l_pByteCodePageAddr[m_uiByteCodeLength / 0x4000] & 0xFFFFFF) + m_uiByteCodeLength % 0x4000); // this is the address of the last bytecode page + plus the length of the last bycode page

	this->m_uiScriptNameOffset = offset((this->m_uiNativesOffset & 0xFFFFFF) + this->m_uiNativeCount * sizeof(unsigned long long));

	// write the header on the file
	a_pFileStream->seekp(0);
	a_pFileStream->write((char*)&m_uiMagicNumber, sizeof(m_uiMagicNumber));
	a_pFileStream->write((char*)&m_uiUnk0008, sizeof(m_uiUnk0008));
	a_pFileStream->write((char*)&m_uiByteCodePageOffset, sizeof(m_uiByteCodePageOffset));
	a_pFileStream->write((char*)&m_uiUnk000C, sizeof(m_uiUnk000C));
	a_pFileStream->write((char*)&m_uiByteCodeLength, sizeof(m_uiByteCodeLength));
	a_pFileStream->write((char*)&m_uiArgCount, sizeof(m_uiArgCount));
	a_pFileStream->write((char*)&m_uiStaticsCount, sizeof(m_uiStaticsCount));
	a_pFileStream->write((char*)&m_uiGlobalsCount, sizeof(m_uiGlobalsCount));
	a_pFileStream->write((char*)&m_uiNativeCount, sizeof(m_uiNativeCount));
	a_pFileStream->write((char*)&m_uiStaticsOffset, sizeof(m_uiStaticsOffset));
	a_pFileStream->write((char*)&m_uiGlobalsOffset, sizeof(m_uiGlobalsOffset));
	a_pFileStream->write((char*)&m_uiNativesOffset, sizeof(m_uiNativesOffset));
	a_pFileStream->write((char*)&m_uiUnk0048, sizeof(m_uiUnk0048));
	a_pFileStream->write((char*)&m_uiUnk004C, sizeof(m_uiUnk004C));
	a_pFileStream->write((char*)&m_uiUnk0050, sizeof(m_uiUnk0050));
	a_pFileStream->write((char*)&m_uiScriptHash, sizeof(m_uiScriptHash));
	a_pFileStream->write((char*)&m_uiNull, sizeof(m_uiNull));
	a_pFileStream->write((char*)&m_uiScriptNameOffset, sizeof(m_uiScriptNameOffset));
	a_pFileStream->write((char*)&m_uiStringsOffset, sizeof(m_uiStringsOffset));
	a_pFileStream->write((char*)&m_uiStringsLength, sizeof(m_uiStringsLength));
	a_pFileStream->write((char*)&m_iUnk0074, sizeof(m_iUnk0074));
	a_pFileStream->write((char*)&m_iUnk0078, sizeof(m_iUnk0078));
	a_pFileStream->write((char*)&m_iUnk007C, sizeof(m_iUnk007C));

	// write the bytecode pages addr
	a_pFileStream->write((char*)l_pByteCodePageAddr, sizeof(unsigned long long) * ((m_uiByteCodeLength / 0x4000) + 1));
	// write the bytecode pages
	for(unsigned int i = 0; i < (m_uiByteCodeLength / 0x4000) + 1; i++)
	{
		int l_iPageLen = 0x4000;
		if(i == (m_uiByteCodeLength / 0x4000)) l_iPageLen = m_uiByteCodeLength % 0x4000;

		a_pFileStream->write((char*)a_pByteCode[i], l_iPageLen);
	}

	// write the native list
	for(unsigned int i = 0; i < this->m_uiNativeCount; i++)
	{
		unsigned long long l_uiNativeHash = _rotr64(this->m_pNativeCollector->getNativeFromId(i), this->m_uiByteCodeLength + i);
		a_pFileStream->write((char*)&l_uiNativeHash, sizeof(l_uiNativeHash));
	}

	strncpy(l_szScriptName, this->m_szScriptName.c_str(), sizeof(l_szScriptName));

	a_pFileStream->write(l_szScriptName, 64);
	
}