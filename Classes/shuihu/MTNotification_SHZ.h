#ifndef SHZ__MTNotification_H_
#define SHZ__MTNotification_H_

#include "cocos2d.h"

USING_NS_CC;
class SHZMTData : public cocos2d::CCObject
{
public:
	static SHZMTData* create(unsigned int data1 = 0, unsigned int data2 = 0, unsigned int data3 = 0, 
		const std::string& str1 = "", const std::string& str2 = "", const std::string& str3 = "",const bool bRemember=true)
	{
		SHZMTData* dat = new SHZMTData();
		if (dat && dat->init(data1, data2, data3, str1, str2, str3,bRemember))
		{
			dat->autorelease();
			return dat;
		}
		delete dat;
		return 0;
	}

	static SHZMTData* create(unsigned int* pData)
	{
		SHZMTData* dat = new SHZMTData();
		if (dat && dat->init(pData))
		{
			dat->autorelease();
			return dat;
		}
		delete dat;
		return 0;
	}

	unsigned int mData1;
	unsigned int mData2;
	unsigned int mData3;
	std::string  mStr1;
	std::string  mStr2;
	std::string  mStr3;

	bool mbRemember;

	unsigned int* m_ptData;			//Ö¸ÕëÖµ

private:
	SHZMTData(){}
	~SHZMTData(){}

	bool init(unsigned int data1, unsigned int data2, unsigned int data3, 
		const std::string& str1, const std::string& str2, const std::string& str3 ,const bool bRemember)
	{
		mData1 = data1;
		mData2 = data2;
		mData3 = data3;
		mStr1  = str1;
		mStr2  = str2;
		mStr3  = str3;
		mbRemember=bRemember;
		return true;
	}

	bool init(unsigned int* pData)
	{
		m_ptData = pData;

		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
class SHZMTNotificationImp;

class SHZMTNotification
	: public cocos2d::CCObject
{
public:
	static SHZMTNotification*	msInstance;
public:
	static SHZMTNotification* shared();
	static void purge();

public:
	SHZMTNotification();
	~SHZMTNotification();
	void post(float dt);

private:
	SHZMTNotificationImp* mImp;
};


#define G_NOTIFICATION_SHZ(name, obj)					CCNotificationCenter::sharedNotificationCenter()->postNotification(name, obj)
#define G_NOTIFICATION_REG_SHZ(name, callfuncO, obj)	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(callfuncO), name, obj)
#define G_NOTIFICATION_UNREG_SHZ(name)					CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, name)
#endif // _MTNotification_H_