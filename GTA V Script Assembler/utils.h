#ifndef __UTILS_H__
#define __UTILS_H__


// trim from start
std::string &ltrim(std::string &s);
// trim from end
std::string &rtrim(std::string &s);
// trim from both ends
std::string &trim(std::string &s);

// if a char is inside a string (between two quotes)
bool isInsideAString(std::string a_szString, int a_iPos);

// https://en.wikipedia.org/wiki/Jenkins_hash_function
unsigned int jooat(char *key, size_t len);

#endif