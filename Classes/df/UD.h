#ifndef _UD_H_
#define _UD_H_

#include "cocos2d.h"

enum TYPE { BOOL_TYPE, INT_TYPE, FLOAT_TYPE, DOUBLE_TYPE, STRING_TYPE };

class UD
{
public:
	static UD* getInstance();
	static inline void destroyInstance() { CC_SAFE_DELETE(_instance); }

public:
	// set
	void setBoolForKey(const char* section, const char* pKey, bool value);
	void setIntegerForKey(const char* section, const char* pKey, int value);
	void setFloatForKey(const char* section, const char* pKey, float value);
	void setDoubleForKey(const char* section, const char* pKey, double value);
	void setStringForKey(const char* section, const char* pKey, const std::string& value);
	// get
	bool getBoolForKey(const char* section, const char* pKey, bool defaultValue = false);
	int	getIntegerForKey(const char* section, const char* pKey, int defaultValue = 0);
	float getFloatForKey(const char* section, const char* pKey, float defaultValue = 0.0f);
	double getDoubleForKey(const char* section, const char* pKey, double defaultValue = 0.0);
	std::string	getStringForKey(const char* section, const char* pKey, const std::string& defaultValue = "");

private:
	UD() {}
	bool subDecode(const char* section, const char* pKey, void* rtn, TYPE type);

private:
	static UD* _instance;
};

#endif