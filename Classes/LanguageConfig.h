#ifndef __LanguageConfig_H__
#define __LanguageConfig_H__

#include <string>
#include <map>
using namespace std;

#define GETSTRING(id) LanguageConfig::getInstance()->getString(id)

class LanguageConfig
{
public:
	LanguageConfig();
	~LanguageConfig();

	static LanguageConfig* getInstance();

	bool init(const char *filename);
	const string& getString(int id);
protected:
	map<int,string> m_mapLanguage;	
};

#endif // __LanguageConfig_H__
