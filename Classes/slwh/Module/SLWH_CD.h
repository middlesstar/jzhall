#ifndef _SLWH_CD_
#define _SLWH_CD_
#include "cocos2d.h"
USING_NS_CC;


#define CD_MAX				50


//#ifndef		PLIST_PATH
//#define		PLIST_PATH							""
//#endif




/*
彩带
*/


class SLWH_CD :
	public Node
{
protected:
	struct SLWH_OneCD : public Sprite
	{
		int speed;					//下落速度
		int rotateSpeed;			//旋转速度 一般和下落速度成正比
		bool bIsleft;				//是否向左移动

		static SLWH_OneCD* create(const std::string& filename)
		{
			SLWH_OneCD *sprite = new (std::nothrow) SLWH_OneCD();
			if (sprite && sprite->initWithFile(filename))	// ->initWithSpriteFrameName(filename)
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}
	};


	bool m_bIsOpen;

	std::string			m_texture2D[CD_MAX];
	Vector<SLWH_OneCD*>		m_list;

	Vec2				m_visibleSize;

	static SLWH_CD*			m_instance;

protected:
	SLWH_CD();

	void CreatOne();
	void update(float deltaTime);
	virtual bool init();

public:
	~SLWH_CD();

	static SLWH_CD* GetInstance();
	static void StartPlay();
	static void StopPlay();

};

#endif