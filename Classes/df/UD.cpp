#include "UD.h"
#include "libs/libs.h"

USING_NS_CC;

#define BUFFER_LEN	100

UD* UD::_instance = nullptr;

UD* UD::getInstance()
{
	if (nullptr == _instance) { _instance = new UD(); }

	return _instance;
}

// set
void UD::setBoolForKey(const char* section, const char* pKey, bool value)
{
	setIntegerForKey(section, pKey, value == true ? 1 : 0);
}
void UD::setIntegerForKey(const char* section, const char* pKey, int value)
{
	char buffer[BUFFER_LEN];
	sprintf(buffer, "%d", value);
	setStringForKey(section, pKey, buffer);
}
void UD::setFloatForKey(const char* section, const char* pKey, float value)
{
	char buffer[BUFFER_LEN];
	sprintf(buffer, "%f", value);
	setStringForKey(section, pKey, buffer);
}
void UD::setDoubleForKey(const char* section, const char* pKey, double value)
{
	char buffer[BUFFER_LEN];
	sprintf(buffer, "%f", value);
	setStringForKey(section, pKey, buffer);
}
void UD::setStringForKey(const char* section, const char* pKey, const std::string& value)
{
	std::string key = std::string(section) + "_" + pKey;
	UserDefault::getInstance()->setStringForKey(key.c_str(), encode(value).c_str());
}

// get
bool UD::getBoolForKey(const char* section, const char* pKey, bool defaultValue)
{
	bool rtn;
	if (!subDecode(section, pKey, &rtn, BOOL_TYPE)) { return defaultValue; }
	return rtn;
}
int	UD::getIntegerForKey(const char* section, const char* pKey, int defaultValue)
{
	int rtn;
	if (!subDecode(section, pKey, &rtn, INT_TYPE)) { return defaultValue; }
	return rtn;
}
float UD::getFloatForKey(const char* section, const char* pKey, float defaultValue)
{
	float rtn;
	if (!subDecode(section, pKey, &rtn, FLOAT_TYPE)) { return defaultValue; }
	return rtn;
}
double UD::getDoubleForKey(const char* section, const char* pKey, double defaultValue)
{
	double rtn;
	if (!subDecode(section, pKey, &rtn, DOUBLE_TYPE)) { return defaultValue; }
	return rtn;
}
std::string	UD::getStringForKey(const char* section, const char* pKey, const std::string& defaultValue)
{
	std::string rtn;
	if (!subDecode(section, pKey, &rtn, STRING_TYPE)) { return defaultValue; }
	return rtn;
}

bool UD::subDecode(const char* section, const char* pKey, void* rtn, TYPE type)
{
	CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
	std::string key = std::string(section) + "_" + pKey;
	std::string strRtn = userDefault->getStringForKey(key.c_str());

	if (strRtn.empty())
		return false;

	std::string decodeStr = decode(strRtn);
	const char *decStr = decodeStr.c_str();

	switch (type)
	{
	case BOOL_TYPE:

		*((bool*)rtn) = atoi(decStr) ? true : false;

		break;
	case INT_TYPE:

		*((int*)rtn) = atoi(decStr);

		break;
	case FLOAT_TYPE:

		*((float*)rtn) = atof(decStr);

		break;
	case DOUBLE_TYPE:

		*((double*)rtn) = atof(decStr);

		break;
	case STRING_TYPE:

		*((std::string*)rtn) = decodeStr;

		break;
	default:
		break;
	}

	return true;
}