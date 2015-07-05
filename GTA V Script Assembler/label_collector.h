#ifndef __LABEL_COLLECTOR_H__
#define __LABEL_COLLECTOR_H__

class LabelCollector
{
public:
	
	LabelCollector();
	~LabelCollector();

	bool			AddLabel(std::string a_szName, unsigned int a_uiAddress);
	unsigned int	getAddress(std::string a_szName);



private:

	std::map<std::string, unsigned int>			m_Labels;



};



#endif