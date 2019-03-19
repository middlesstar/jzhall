#ifndef _CCUserDefault_H_
#define _CCUserDefault_H_

#include <string>

#define SHZ_BOOL_TYPE	1
#define SHZ_INT_TYPE	2
#define SHZ_FLOAT_TYPE	3
#define SHZ_DOUBLE_TYPE	4
#define SHZ_STRING_TYPE	5
#define TYPE		int

class JZUserDefault
{
public:
	bool		getBoolForKey(const char* section, const char* pKey);
	bool		getBoolForKey(const char* section, const char* pKey, bool defaultValue);
	int			getIntegerForKey(const char* section, const char* pKey);
	int			getIntegerForKey(const char* section, const char* pKey, int defaultValue);
	float		getFloatForKey(const char* section, const char* pKey);
	float		getFloatForKey(const char* section, const char* pKey, float defaultValue);
	double		getDoubleForKey(const char* section, const char* pKey);
	double		getDoubleForKey(const char* section, const char* pKey, double defaultValue);
	std::string	getStringForKey(const char* section, const char* pKey);
	std::string	getStringForKey(const char* section, const char* pKey, const std::string & defaultValue);
	
	void		setBoolForKey(const char* section, const char* pKey, bool value);
	void		setIntegerForKey(const char* section, const char* pKey, int value);
	void		setFloatForKey(const char* section, const char* pKey, float value);
	void		setDoubleForKey(const char* section, const char* pKey, double value);
	void		setStringForKey(const char* section, const char* pKey, const std::string & value);
	
	void		setValueForKey(const char* section, const char* pKey, const char* value);
	static JZUserDefault*	getInstance();
	static void destroyInstance();
	
private:
	JZUserDefault();
	~JZUserDefault();
	bool _decode(const char* section, const char* pKey, void* rtn, TYPE type);
};

#endif // _CCUserDefault_H_
