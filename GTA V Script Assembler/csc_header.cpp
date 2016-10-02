#include "main.h"

#define offset(x) (0x50000000 | (x & 0x00FFFFFF))


CscHeader::CscHeader()
{
	m_uiMagicNumber				= 0;//x14005BE20;
	m_uiPgBase					= 0;
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
	memset(m_Unk0030, 0, sizeof(m_Unk0030));
	m_uiScriptHash				= 0;
	m_uiScriptCount				= 1;
	m_uiScriptNameOffset		= 0;
	m_uiStringsOffset			= 0;
	m_uiStringsLength			= 0;


	m_szScriptName				= "";
}

CscHeader::~CscHeader()
{

}

void CscHeader::setByteCodeLength(unsigned int a_uiLength)
{
	this->m_uiByteCodeLength = a_uiLength;
}

unsigned int CscHeader::getByteCodeLength()
{
	return this->m_uiByteCodeLength;
}

void CscHeader::setScriptName(std::string a_szName)
{
	std::transform(a_szName.begin(), a_szName.end(), a_szName.begin(), std::tolower);
	this->m_uiScriptHash = jooat((char*)a_szName.c_str(), a_szName.length());
	this->m_szScriptName = a_szName;
}



void CscHeader::WriteToFile(std::ofstream* a_pFileStream, unsigned char** a_pByteCode, NativeCollector*	a_pNativeCollector, StringCollector* a_pStringCollector)
{
	char					l_szScriptName[64];
	unsigned long*			l_pByteCodePageAddr;
	unsigned long*			l_pStringPageAddr;

	assert(!"Not implemented yet !");
	return;

	this->m_uiStringsLength		= a_pStringCollector->getStringPageLength();
	this->m_uiNativeCount		= a_pNativeCollector->getCount();
	this->m_uiUnk000C			= 0x99B407C5; //TODO: ??
	this->m_uiScriptCount		= 1;

	// write the header on the file
	a_pFileStream->seekp(0);
	a_pFileStream->write((char*)&m_uiMagicNumber, sizeof(m_uiMagicNumber));
	a_pFileStream->write((char*)&m_uiPgBase, sizeof(m_uiPgBase));
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
	a_pFileStream->write((char*)m_Unk0030, sizeof(m_Unk0030));
	a_pFileStream->write((char*)&m_uiScriptHash, sizeof(m_uiScriptHash));
	a_pFileStream->write((char*)&m_uiScriptCount, sizeof(m_uiScriptCount));
	a_pFileStream->write((char*)&m_uiScriptNameOffset, sizeof(m_uiScriptNameOffset));
	a_pFileStream->write((char*)&m_uiStringsOffset, sizeof(m_uiStringsOffset));
	a_pFileStream->write((char*)&m_uiStringsLength, sizeof(m_uiStringsLength));
	

	// alloc the bytecode pages addr table
	l_pByteCodePageAddr = new unsigned long[(this->m_uiByteCodeLength / 0x4000) + 1];

	// write the bytecode pages
	for(unsigned int i = 0; i < (this->m_uiByteCodeLength / 0x4000) + 1; i++)
	{
		int l_iPageLen = 0x4000;

		//if(i == (this->m_uiByteCodeLength / 0x4000)) l_iPageLen = this->m_uiByteCodeLength % 0x4000;

		l_pByteCodePageAddr[i] = offset(a_pFileStream->tellp());
		a_pFileStream->write((char*)a_pByteCode[i], l_iPageLen);
	}
	
	// write the string pages

	if(a_pStringCollector->getStringPageLength() != 0)
	{
		int		l_iPageCount = a_pStringCollector->getPageCount();
		int		l_iPageLen;
		char**	l_ppStringPages = a_pStringCollector->getStringPages();

		l_pStringPageAddr = new unsigned long[l_iPageCount];


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
			unsigned long l_uiNativeHash = a_pNativeCollector->getNative(i)->m_ullNativeHash;
			a_pFileStream->write((char*)&l_uiNativeHash, sizeof(unsigned long));
		}
	}

	strncpy_s(l_szScriptName, this->m_szScriptName.c_str(), sizeof(l_szScriptName));
	this->m_uiScriptNameOffset = offset(a_pFileStream->tellp());
	a_pFileStream->write(l_szScriptName, sizeof(l_szScriptName));


	// write the bytecode page addr table
	m_uiByteCodePageOffset = offset(a_pFileStream->tellp());
	a_pFileStream->write((char*)l_pByteCodePageAddr, ((this->m_uiByteCodeLength / 0x4000) + 1) * sizeof(unsigned long));

	// write the string page addr table
	if(m_uiStringsLength)
	{
		m_uiStringsOffset = offset(a_pFileStream->tellp());
		a_pFileStream->write((char*)l_pStringPageAddr, a_pStringCollector->getPageCount() * sizeof(unsigned long));
	}


	// rewrite bytecode offset
	a_pFileStream->seekp(0x08);
	a_pFileStream->write((char*)&m_uiByteCodePageOffset, sizeof(m_uiByteCodePageOffset));
	// rewrite native table offset
	a_pFileStream->seekp(0x2C);
	a_pFileStream->write((char*)&m_uiNativesOffset, sizeof(m_uiNativesOffset));
	// rewrite script name offset
	a_pFileStream->seekp(0x40);
	a_pFileStream->write((char*)&m_uiScriptNameOffset, sizeof(m_uiScriptNameOffset));
	// rewrite strings offset
	a_pFileStream->seekp(0x44);
	a_pFileStream->write((char*)&m_uiStringsOffset, sizeof(m_uiScriptNameOffset));
}



void CscHeader::ReadFromFile(std::ifstream* a_pFileStream)
{
	int i;
	a_pFileStream->seekg(0);

	// Read the header
	a_pFileStream->read((char*)&this->m_uiMagicNumber, sizeof(this->m_uiMagicNumber));
	a_pFileStream->read((char*)&this->m_uiPgBase, sizeof(this->m_uiPgBase));
	a_pFileStream->read((char*)&this->m_uiByteCodePageOffset, sizeof(this->m_uiByteCodePageOffset));		
	a_pFileStream->read((char*)&this->m_uiUnk000C, sizeof(this->m_uiUnk000C));
	a_pFileStream->read((char*)&this->m_uiByteCodeLength, sizeof(this->m_uiByteCodeLength));
	a_pFileStream->read((char*)&this->m_uiArgCount, sizeof(this->m_uiArgCount));
	a_pFileStream->read((char*)&this->m_uiStaticsCount, sizeof(this->m_uiStaticsCount));
	a_pFileStream->read((char*)&this->m_uiGlobalsCount, sizeof(this->m_uiGlobalsCount));
	a_pFileStream->read((char*)&this->m_uiNativeCount, sizeof(this->m_uiNativeCount));
	a_pFileStream->read((char*)&this->m_uiStaticsOffset, sizeof(this->m_uiStaticsOffset));
	a_pFileStream->read((char*)&this->m_uiGlobalsOffset, sizeof(this->m_uiGlobalsOffset));
	a_pFileStream->read((char*)&this->m_uiNativesOffset, sizeof(this->m_uiNativesOffset));
	a_pFileStream->read((char*)this->m_Unk0030, sizeof(this->m_Unk0030));
	a_pFileStream->read((char*)&this->m_uiScriptHash, sizeof(this->m_uiScriptHash));
	a_pFileStream->read((char*)&this->m_uiScriptCount, sizeof(this->m_uiScriptCount));
	a_pFileStream->read((char*)&this->m_uiScriptNameOffset, sizeof(this->m_uiScriptNameOffset));
	a_pFileStream->read((char*)&this->m_uiStringsOffset, sizeof(this->m_uiStringsOffset));
	a_pFileStream->read((char*)&this->m_uiStringsLength, sizeof(this->m_uiStringsLength));

	this->m_uiMagicNumber = _byteswap_ulong(this->m_uiMagicNumber);			
	this->m_uiPgBase = _byteswap_ulong(this->m_uiPgBase);					
	this->m_uiByteCodePageOffset = _byteswap_ulong(this->m_uiByteCodePageOffset);		
	this->m_uiUnk000C = _byteswap_ulong(this->m_uiUnk000C);				
	this->m_uiByteCodeLength = _byteswap_ulong(this->m_uiByteCodeLength);			
	this->m_uiArgCount = _byteswap_ulong(this->m_uiArgCount);				
	this->m_uiStaticsCount = _byteswap_ulong(this->m_uiStaticsCount);			
	this->m_uiGlobalsCount = _byteswap_ulong(this->m_uiGlobalsCount);			
	this->m_uiNativeCount = _byteswap_ulong(this->m_uiNativeCount);			
	this->m_uiStaticsOffset = _byteswap_ulong(this->m_uiStaticsOffset);			
	this->m_uiGlobalsOffset = _byteswap_ulong(this->m_uiGlobalsOffset);			
	this->m_uiNativesOffset = _byteswap_ulong(this->m_uiNativesOffset);	
	this->m_uiScriptHash = _byteswap_ulong(this->m_uiScriptHash);				
	this->m_uiScriptCount = _byteswap_ulong(this->m_uiScriptCount);			
	this->m_uiScriptNameOffset = _byteswap_ulong(this->m_uiScriptNameOffset);		
	this->m_uiStringsOffset = _byteswap_ulong(this->m_uiStringsOffset);			
	this->m_uiStringsLength = _byteswap_ulong(this->m_uiStringsLength);			


//	_byteswap_ulong


	// Remove the high order byte because we don't need it (it's an identifier, always 0x50)
	this->m_uiByteCodePageOffset	&= 0x00FFFFFF;
	this->m_uiStaticsOffset			&= 0x00FFFFFF;
	this->m_uiGlobalsOffset			&= 0x00FFFFFF;
	this->m_uiNativesOffset			&= 0x00FFFFFF;	
	this->m_uiScriptNameOffset		&= 0x00FFFFFF;
	this->m_uiStringsOffset			&= 0x00FFFFFF;


	// read the script name
	a_pFileStream->seekg(this->m_uiScriptNameOffset);
	m_szScriptName.resize(64);
	a_pFileStream->read(&m_szScriptName[0], 64);

	// read the natives hashes
	this->m_NativesHashes.clear();
	a_pFileStream->seekg(getNativesOffset());

	for(i = 0; i < getNativesCount(); i++)
	{
		unsigned int l_iNativeHash;
		a_pFileStream->read((char*)&l_iNativeHash, sizeof(l_iNativeHash));
		this->m_NativesHashes.push_back(l_iNativeHash);
	}
}

std::string CscHeader::getName()
{
	return this->m_szScriptName;
}


int CscHeader::getCodePageCount()
{
	return (this->m_uiByteCodeLength / 0x4000) + 1;
}

int CscHeader::getCodePageLength(unsigned int a_uiPage)
{
	if((this->getCodePageCount() - 1) == a_uiPage)
	{
		return this->m_uiByteCodeLength % 0x4000;
	}
	return 0x4000;
}

int CscHeader::getCodePageOffset(std::ifstream* a_pFileStream, int a_uiPage)
{
	int l_uiPageOffset;

	l_uiPageOffset = -1;

	if(a_uiPage < getCodePageCount())
	{
		a_pFileStream->seekg(this->m_uiByteCodePageOffset + 8 * a_uiPage);
		a_pFileStream->read((char*)&l_uiPageOffset, 4);
		l_uiPageOffset &= 0x00FFFFFF;
	}
	return l_uiPageOffset;
}

int CscHeader::getStringPageCount()
{
	return (this->m_uiStringsLength / 0x4000) + 1;
}

int CscHeader::getStringPageLength(unsigned int a_uiPage)
{
	if((this->getStringPageCount() - 1) == a_uiPage)
	{
		return this->m_uiStringsLength % 0x4000;
	}
	return 0x4000;
}

int CscHeader::getStringPageOffset(std::ifstream* a_pFileStream, int a_uiPage)
{
	int l_uiPageOffset;

	l_uiPageOffset = -1;

	if(a_uiPage < getStringPageCount())
	{
		a_pFileStream->seekg(this->m_uiStringsOffset + 8 * a_uiPage);
		a_pFileStream->read((char*)&l_uiPageOffset, 4);
		l_uiPageOffset &= 0x00FFFFFF;
	}
	return l_uiPageOffset;
}

int CscHeader::getStringsLength()
{
	return this->m_uiStringsLength;
}

int CscHeader::getNativesCount()
{
	return this->m_uiNativeCount;
}

int CscHeader::getNativesOffset()
{
	return this->m_uiNativesOffset;
}

__int64 CscHeader::getNativeHash(int a_iIndex)
{
	if(a_iIndex < getNativesCount())
	{
		return this->m_NativesHashes[a_iIndex];
	}
	return -1;
}