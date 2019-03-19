#ifndef _GOLD_FALLEN_3D	
#define _GOLD_FALLEN_3D	

#include "cocos2d.h"
USING_NS_CC;

#define		Max_Gold	150
#define		Max_Tex		7

//#ifndef		PLIST_PATH
//#define		PLIST_PATH							""
//#endif

#ifndef		GoldFallen3D_GOLD
#define		GoldFallen3D_GOLD					"images/common/animation/gold1/gold1_"
#endif


class GoldFallen3D:
	public Node
{
private:
	Vec2				m_visibleSize;
	const int			SpeedAdd;
	int					now_index;						//当前掉落金币数量
	bool				m_bIsOpen;			

	static GoldFallen3D*	m_instance;
protected:
	std::string	m_Texture[Max_Tex];

	struct OneGold :public Sprite
	{
		float v_x;
		float v_z;
		float v_y;						//下落速度
		float time;					    //下落翻转事件计时
		bool jump;						//是否弹起

		static OneGold* create(const std::string& filename)
		{
			OneGold *sprite = new (std::nothrow) OneGold();
			if (sprite && sprite->initWithFile(filename))		// ->initWithSpriteFrameName(filename)
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}

		static OneGold* create()
		{
			OneGold *sprite = new (std::nothrow) OneGold();
			if (sprite && sprite->init())
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}
	};

	Vector<OneGold*> m_list;

protected:

	void CreatOne();

	virtual void update(float deltaTime);
	virtual bool init();
protected:

	GoldFallen3D();
	
public:
	
	~GoldFallen3D();
	static GoldFallen3D* GetInstance();
	static void StartPlay();
	static void StopPlay();
};

#endif