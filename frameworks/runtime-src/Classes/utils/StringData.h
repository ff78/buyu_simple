#ifndef _StringData_H_
#define _StringData_H_

#include <string>
#include <unordered_map>
//////////////////////////////////////////////////////////////////////////

#define SSTRING(key) StringData::shared()->stringFromKey(key)

//////////////////////////////////////////////////////////////////////////


class StringData
{
public:
	static StringData* shared();
public:
	const std::string& stringFromKey(const std::string& key);  
private:
    StringData();
    ~StringData();
    bool init();

private:
	std::unordered_map<std::string, std::string> _dic;
};

#endif // _StringData_H_
