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
	m_uiScriptCount				= 1;
	m_uiScriptNameOffset		= 0;
	m_uiStringsOffset			= 0;
	m_uiStringsLength			= 0;
	m_iUnk0074					= 0;
	m_iUnk0078					= 0;
	m_iUnk007C					= 0;

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
	std::transform(a_szName.begin(), a_szName.end(), a_szName.begin(), std::tolower);
	this->m_uiScriptHash = jooat((char*)a_szName.c_str(), a_szName.length());
	this->m_szScriptName = a_szName;
}



void YscHeader::WriteToFile(std::ofstream* a_pFileStream, unsigned char** a_pByteCode, NativeCollector*	a_pNativeCollector, StringCollector* a_pStringCollector)
{
	char					l_szScriptName[64];
	unsigned long long*		l_pByteCodePageAddr;
	unsigned long long*		l_pStringPageAddr;

	this->m_uiStringsLength		= a_pStringCollector->getStringsLength();
	this->m_uiNativeCount		= a_pNativeCollector->getNativeCount();
	this->m_uiUnk000C			= 0x99B407C5;
	this->m_uiScriptCount		= 1;

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
	a_pFileStream->write((char*)&m_uiScriptCount, sizeof(m_uiScriptCount));
	a_pFileStream->write((char*)&m_uiScriptNameOffset, sizeof(m_uiScriptNameOffset));
	a_pFileStream->write((char*)&m_uiStringsOffset, sizeof(m_uiStringsOffset));
	a_pFileStream->write((char*)&m_uiStringsLength, sizeof(m_uiStringsLength));
	a_pFileStream->write((char*)&m_iUnk0074, sizeof(m_iUnk0074));
	a_pFileStream->write((char*)&m_iUnk0078, sizeof(m_iUnk0078));
	a_pFileStream->write((char*)&m_iUnk007C, sizeof(m_iUnk007C));

	

	// alloc the bytecode pages addr table
	l_pByteCodePageAddr = new unsigned long long[(this->m_uiByteCodeLength / 0x4000) + 1];

	// write the bytecode pages
	for(unsigned int i = 0; i < (this->m_uiByteCodeLength / 0x4000) + 1; i++)
	{
		int l_iPageLen = 0x4000;

		//if(i == (this->m_uiByteCodeLength / 0x4000)) l_iPageLen = this->m_uiByteCodeLength % 0x4000;

		l_pByteCodePageAddr[i] = offset(a_pFileStream->tellp());
		a_pFileStream->write((char*)a_pByteCode[i], l_iPageLen);
	}
	
	// write the string pages

	if(a_pStringCollector->getStringsLength() != 0)
	{
		int		l_iPageCount = a_pStringCollector->getStringPageCount();
		int		l_iPageLen;
		char**	l_ppStringPages = a_pStringCollector->constructStringsPage();

		l_pStringPageAddr = new unsigned long long[l_iPageCount];


		for(int i = 0; i < l_iPageCount; i++)
		{
			l_iPageLen = 0x4000;
			l_pStringPageAddr[i] = offset(a_pFileStream->tellp());
			a_pFileStream->write((char*)l_ppStringPages[i], l_iPageLen);
		}
	}

	// write the native table
	if(this->m_uiNativeCount)
	{
		this->m_uiNativesOffset = offset(a_pFileStream->tellp());

		for(unsigned int i = 0; i < this->m_uiNativeCount; i++)
		{
			unsigned long long l_uiNativeHash = a_pNativeCollector->getNativeFromId(i);
			l_uiNativeHash = _rotr64(l_uiNativeHash, this->m_uiByteCodeLength + i);
			a_pFileStream->write((char*)&l_uiNativeHash, sizeof(unsigned long long));
		}
	}

	strncpy(l_szScriptName, this->m_szScriptName.c_str(), sizeof(l_szScriptName));
	this->m_uiScriptNameOffset = offset(a_pFileStream->tellp());
	a_pFileStream->write(l_szScriptName, sizeof(l_szScriptName));


	// write the bytecode page addr table
	m_uiByteCodePageOffset = offset(a_pFileStream->tellp());
	a_pFileStream->write((char*)l_pByteCodePageAddr, ((this->m_uiByteCodeLength / 0x4000) + 1) * sizeof(unsigned long long));

	// write the string page addr table
	if(m_uiStringsLength)
	{
		m_uiStringsOffset = offset(a_pFileStream->tellp());
		a_pFileStream->write((char*)l_pStringPageAddr, a_pStringCollector->getStringPageCount() * sizeof(unsigned long long));
	}


	// rewrite bytecode offset
	a_pFileStream->seekp(0x10);
	a_pFileStream->write((char*)&m_uiByteCodePageOffset, sizeof(m_uiByteCodePageOffset));
	// rewrite native table offset
	a_pFileStream->seekp(0x40);
	a_pFileStream->write((char*)&m_uiNativesOffset, sizeof(m_uiNativesOffset));
	// rewrite script name offset
	a_pFileStream->seekp(0x60);
	a_pFileStream->write((char*)&m_uiScriptNameOffset, sizeof(m_uiScriptNameOffset));
	// rewrite strings offset
	a_pFileStream->seekp(0x68);
	a_pFileStream->write((char*)&m_uiStringsOffset, sizeof(m_uiScriptNameOffset));
}