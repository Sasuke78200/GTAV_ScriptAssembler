#ifndef __SCR_HEADER_H__
#define __SCR_HEADER_H__


class scrHeader
{
public:
	scrHeader();
	~scrHeader();

	virtual void setScriptName(std::string a_szName) = 0;

	virtual void WriteToFile(std::ofstream* a_pFileStream, 
		unsigned char**		a_pByteCode,
		NativeCollector*	a_pNativeCollector,
		StringCollector*	a_pStringCollector) = 0;

	virtual void			ReadFromFile(std::ifstream* a_pFileStream) = 0;


	virtual std::string		getName() = 0;

	/* Code page informations */
	virtual int				getCodePageCount() = 0;
	virtual int				getCodePageLength(unsigned int a_uiPage) = 0;
	virtual int				getCodePageOffset(std::ifstream* a_pFileStream, int a_uiPage) = 0;
	virtual void			setByteCodeLength(unsigned int a_uiLength) = 0;
	virtual unsigned int	getByteCodeLength() = 0;

	/* String page informations */
	virtual int				getStringPageCount() = 0;
	virtual int				getStringPageLength(unsigned int a_uiPage) = 0;
	virtual int				getStringPageOffset(std::ifstream* a_pFileStream, int a_uiPage) = 0;
	virtual int				getStringsLength() = 0;

	/* Natives informations */
	virtual int				getNativesCount() = 0;
	virtual int				getNativesOffset() = 0;
	virtual __int64			getNativeHash(int a_iIndex) = 0;


private:
};


#endif