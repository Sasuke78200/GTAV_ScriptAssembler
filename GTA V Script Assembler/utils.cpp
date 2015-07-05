#include "main.h"




// trim from start, http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end, http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends, http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

// if a char is inside a string (between two quotes)
bool isInsideAString(std::string a_szString, int a_iPos)
{
	unsigned int	i;
	bool			l_bString = false;


	for(i = 0; i < a_szString.length(); i++)
	{
		if(i == 0)
		{
			if(a_szString[i] == '"')
			{
				l_bString = !l_bString;
			}
		}
		else
		{
			if(a_szString[i] == '"' && a_szString[i] != '\\')
			{
				l_bString = !l_bString;
			}
		}

		if(i == a_iPos) break;
	}
	return l_bString;
}

// https://en.wikipedia.org/wiki/Jenkins_hash_function
unsigned int jooat(char *key, size_t len)
{
    unsigned int hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}