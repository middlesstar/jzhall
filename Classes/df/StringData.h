#ifndef _StringData_H_
#define _StringData_H_

#include <string>
#include <unordered_map>
using namespace std;

//////////////////////////////////////////////////////////////////////////

#define SSTRING(key) StringData::shared()->stringFromKey(key)

//////////////////////////////////////////////////////////////////////////


class StringData
{
public:
	static StringData* shared();
public:
	const std::string& stringFromKey(const std::string& key);  

	void clear();
	bool init(const string& strFile);
private:
    StringData();
    ~StringData();

private:
	std::unordered_map<std::string, std::string> _dic;
};

#endif // _StringData_H_
