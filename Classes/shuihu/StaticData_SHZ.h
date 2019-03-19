#ifndef SHZ___StaticData_H_
#define SHZ___StaticData_H_

#include "cocos2d.h"
//////////////////////////////////////////////////////////////////////////

#define STATIC_DATA_STRING(key)		SHZStaticData::shared()->stringFromKey(key)
#define STATIC_DATA_INT(key)		SHZStaticData::shared()->intFromKey(key)
#define STATIC_DATA_FLOAT(key)		SHZStaticData::shared()->floatFromKey(key)
#define STATIC_DATA_BOOLEAN(key)	SHZStaticData::shared()->booleanFromKey(key)
#define STATIC_DATA_POINT(key)		SHZStaticData::shared()->pointFromKey(key)
#define STATIC_DATA_RECT(key)		SHZStaticData::shared()->rectFromKey(key)
#define STATIC_DATA_SIZE(key)		SHZStaticData::shared()->sizeFromKey(key)
#define STATIC_DATA_ARRAY(key)		SHZStaticData::shared()->arrayFromKey(key)
#define STATIC_DATA_COLOR(key)		SHZStaticData::shared()->colorFromKey(key)
//////////////////////////////////////////////////////////////////////////


class SHZStaticData : public cocos2d::CCObject
{
public:
	static SHZStaticData* shared();
	static void purge();

public:
    const char*			stringFromKey(const std::string& key);
    int					intFromKey(const std::string& key);
	unsigned int		colorFromKey(const std::string& key);
	float				floatFromKey(const std::string& key);
    bool				booleanFromKey(const std::string& key);
    cocos2d::CCPoint	pointFromKey(const std::string& key);
    cocos2d::CCRect		rectFromKey(const std::string& key);
    cocos2d::CCSize		sizeFromKey(const std::string& key);
	cocos2d::CCArray*	arrayFromKey(const std::string& key);

	void setIP(const std::string& strIP){ m_strIP = strIP; };
	const std::string& getIP() const { return m_strIP; };

	void setPort(const int nPort){ m_nPort = nPort; };
	const int getPort() const { return m_nPort; };
private:
	bool init();
    SHZStaticData();
    ~SHZStaticData();
private:
	cocos2d::CCDictionary* _dictionary;

	std::string m_strIP;
	int m_nPort;
};
#endif // __StaticData_H_
