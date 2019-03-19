#include "cocos2d.h"
#include "CCUserDefault.h"
#include "libs/libs.h"

USING_NS_CC;
using namespace std;
//////////////////////////////////////////////////////////////////////////
JZUserDefault::JZUserDefault()
{
}

JZUserDefault::~JZUserDefault()
{
}

bool JZUserDefault::getBoolForKey(const char* section, const char* pKey)
{
	return getBoolForKey(section, pKey, false);
}

bool JZUserDefault::getBoolForKey(const char* section, const char* pKey, bool defaultValue)
{
	bool rtn;
	if (!_decode(section, pKey, &rtn, SHZ_BOOL_TYPE))
		return defaultValue;
	return rtn;
}

int JZUserDefault::getIntegerForKey(const char* section, const char* pKey)
{
    return getIntegerForKey(section, pKey, 0);
}

int JZUserDefault::getIntegerForKey(const char* section, const char* pKey, int defaultValue)
{
	int rtn;
	if (!_decode(section, pKey, &rtn, SHZ_INT_TYPE))
		return defaultValue;
	return rtn;
}

float JZUserDefault::getFloatForKey(const char* section, const char* pKey)
{
    return getFloatForKey(section, pKey, 0.0f);
}

float JZUserDefault::getFloatForKey(const char* section, const char* pKey, float defaultValue)
{
	//string key = string(section)+"_"+pKey;
	//char buf[128]={0};
	//sprintf(buf, "%f", defaultValue);

	//std::string szValue = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), buf);
	//float fValue = defaultValue;
	//sscanf(szValue.c_str(), "%f", &fValue);
	//return fValue;

	float rtn;
	if (!_decode(section, pKey, &rtn, SHZ_FLOAT_TYPE))
		return defaultValue;
	return rtn;
}

double  JZUserDefault::getDoubleForKey(const char* section, const char* pKey)
{
    return getDoubleForKey(section, pKey, 0.0);
}

double JZUserDefault::getDoubleForKey(const char* section, const char* pKey, double defaultValue)
{	
	//string key = string(section)+"_"+pKey;
	//char buf[128]={0};
	//sprintf(buf, "%f", defaultValue);

	//std::string szValue = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), buf);
	//float fValue = defaultValue;
	//sscanf(szValue.c_str(), "%f", &fValue);
	//return (double)fValue;
	double rtn;
	if (!_decode(section, pKey, &rtn, SHZ_DOUBLE_TYPE))
		return defaultValue;
	return rtn;
}

std::string JZUserDefault::getStringForKey(const char* section, const char* pKey)
{
    return getStringForKey(section, pKey, "");
}

string JZUserDefault::getStringForKey(const char* section, const char* pKey, const std::string & defaultValue)
{
	//string key = string(section)+"_"+pKey;
	//return CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), defaultValue);
	std::string rtn;
	if (!_decode(section, pKey, &rtn, SHZ_STRING_TYPE))
		return defaultValue;
	return rtn;
}

void JZUserDefault::setBoolForKey(const char* section, const char* pKey, bool value)
{
	setIntegerForKey(section, pKey, value ? 1 : 0);
}

void JZUserDefault::setIntegerForKey(const char* section, const char* pKey, int value)
{
	//char buf[128]={0};
	//sprintf(buf, "%d", value);

	/*std::string key = string(section) + "_" + pKey;*/
	//setValueForKey(section, pKey, buf);
    
	char strValue[30];
	sprintf(strValue, "%d", value);
	setValueForKey(section, pKey, strValue);
//	CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), value);
}

void JZUserDefault::setFloatForKey(const char* section, const char* pKey, float value)
{
	char buf[128]={0};
	sprintf(buf, "%f", value);
	setValueForKey(section, pKey, buf);
}

void JZUserDefault::setDoubleForKey(const char* section, const char* pKey, double value)
{
	char buf[128]={0};
	sprintf(buf, "%f", value);
	setValueForKey(section, pKey, buf);
}

void JZUserDefault::setStringForKey(const char* section, const char* pKey, const std::string & value)
{
	setValueForKey(section, pKey, value.c_str());
}

void JZUserDefault::setValueForKey(const char* section, const char* pKey, const char* value)
{
	string key = string(section)+"_"+pKey;
	// ¼ÓÃÜ
	CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), encode(value).c_str());
	CCUserDefault::sharedUserDefault()->flush();
}

JZUserDefault* JZUserDefault::getInstance()
{
	static JZUserDefault sInstance;
    return &sInstance;
}

void JZUserDefault::destroyInstance()
{
}


bool JZUserDefault::_decode(const char* section, const char* pKey, void* rtn, TYPE type)
{
	CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
	string key = string(section) + "_" + pKey;
	std::string strRtn = userDefault->getStringForKey(key.c_str());
	
	if (strRtn.empty())
		return false;

	//strRtn.c_str();//
	std::string decodeStr = decode(strRtn);
	const char *decStr = decodeStr.c_str();

	switch (type)
	{
	case SHZ_BOOL_TYPE:

		*((bool*)rtn) = atoi(decStr) ? true : false;

		break;
	case SHZ_INT_TYPE:

		*((int*)rtn) = atoi(decStr);

		break;
	case SHZ_FLOAT_TYPE:

		*((float*)rtn) = atof(decStr);

		break;
	case SHZ_DOUBLE_TYPE:

		*((double*)rtn) = atof(decStr);

		break;
	case SHZ_STRING_TYPE:
		//strRtn;//
		*((std::string*)rtn) = decodeStr;

		break;
	default:
		break;
	}

	return true;
}