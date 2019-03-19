#include "StaticData_SHZ.h"
#include "Network/NetManager.h"

USING_NS_CC;

using namespace std;

//定义文件的路径
#define STATIC_DATA_PATH "static_data.plist"

//定义StaticData对象
static SHZStaticData* _sharedStaticData = 0;

//得到StaticData单例对象
SHZStaticData* SHZStaticData::shared()
{
	if(_sharedStaticData == 0){
		_sharedStaticData = new SHZStaticData();
		_sharedStaticData->init();
	}
	return _sharedStaticData;
}

//当内存不足时调用
void SHZStaticData::purge()
{
	CC_SAFE_RELEASE_NULL(_sharedStaticData);
}


SHZStaticData::SHZStaticData()
	//: m_strIP("ip.108jz.com")
	//: m_strIP("120.26.6.159")
	//, m_nPort(8300)
{
	m_strIP = NetManager::getInstance()->getLoginServerIp();
	m_nPort = NetManager::getInstance()->getLoginServerPort();
}

SHZStaticData::~SHZStaticData()
{
	CC_SAFE_RELEASE_NULL(_dictionary);
}


bool SHZStaticData::init()
{
	_dictionary = CCDictionary::createWithContentsOfFile("static_data.plist");//STATIC_DATA_PATH);
	CC_SAFE_RETAIN(_dictionary);
	return true;
}

//根据键值得到String类型数据
const char* SHZStaticData::stringFromKey(const std::string& key)
{
	//return "42.96.196.120";
	return m_strIP.c_str();
	//return _dictionary->valueForKey(key)->getCString();
}

//根据键值得到int类型数据
int SHZStaticData::intFromKey(const std::string& key)
{
	return _dictionary->valueForKey(key)->intValue();
}


unsigned int SHZStaticData::colorFromKey(const std::string& key)
{
	const char* str = _dictionary->valueForKey(key)->getCString();
	unsigned int val=0;
	sscanf(str, "%x", &val);
	return val;
}

//根据键值得到float类型数据
float SHZStaticData::floatFromKey(const std::string& key)
{
	return _dictionary->valueForKey(key)->floatValue();
}

//根据键值得到bool类型数据
bool SHZStaticData::booleanFromKey(const std::string& key)
{
	return _dictionary->valueForKey(key)->boolValue();
}

//根据键值得到point类型数据
cocos2d::CCPoint SHZStaticData::pointFromKey(const std::string& key)
{
	return PointFromString(_dictionary->valueForKey(key)->getCString());
}

//根据键值得到rect类型数据
cocos2d::CCRect SHZStaticData::rectFromKey(const std::string& key)
{
	return RectFromString(_dictionary->valueForKey(key)->getCString());
}

//根据键值得到size类型数据
cocos2d::CCSize SHZStaticData::sizeFromKey(const std::string& key)
{
	return SizeFromString(_dictionary->valueForKey(key)->getCString());
}

//根据键值得到size类型数据
cocos2d::CCArray* SHZStaticData::arrayFromKey(const std::string& key)
{
	return (CCArray*)_dictionary->objectForKey(key);
}

