#ifndef _GOLD_FOUNTAIN	
#define _GOLD_FOUNTAIN	

/*
	金币喷泉
*/

#include "cocos2d.h"
USING_NS_CC;

#ifndef PLIST_PATH
#define PLIST_PATH														//PLIST的路径				
#endif


#ifndef	Path_PNG_Gold
#define	Path_PNG_Gold							"images/common/animation/gold1/gold1_0.png"
#endif


class GoldFountain :
	public Node
{
private:
	//属性变量
	float				Gold_V_X_MIN; 					//横向初速度最小值
	float				Gold_V_X_MAX;					//横向初速度最大值
	float				Gold_V_Y_MIN;					//纵向初速度最小值
	float				Gold_V_Y_MAX;					//纵向初速度最大值
	float				Gold_V_TIME_MIN;				//金币运动最小时间
	float				Gold_V_TIME_MAX;				//金币运动最大时间
	float				Gold_V_TIME_PAUSE;				//金币停留时间
	float				Gold_V_TIME_SHOOT;				//金币最后冲刺时间
	float				Gold_Rotate_Speed;				//硬币自转一圈的时间
	float				Gold_G;							//重力加速度(只对抛物运动)
	//int					Max_Gold;						//金币产生的个数
	float				Max_Gold_Time;					//金币生产的时间
	int					Layer_Level;					//层级


	float				now_index;						//当前金币时间
	bool				m_bIsOpen;			
	Vec2				m_StartPos;
	Vec2				m_EndPos;
protected:
	struct OneGold :public Sprite
	{
		float v_x;						//纵向初速度
		float v_y;						//横向初速度
		float time;						//能运动的时间运动时间
		bool  bIsShoot;					//是否进入了冲刺阶段
		static OneGold* create(const std::string& filename)
		{
			OneGold *sprite = new (std::nothrow) OneGold();
			if (sprite && sprite->initWithFile(filename))		// ->initWithSpriteFrameName(filename)
			{
				sprite->autorelease();
				return sprite;
			}
			//if (sprite && sprite->initWithSpriteFrameName(filename))
			//{
			//	sprite->autorelease();
			//	return sprite;
			//}

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
	void Destory(float t);
	virtual void update(float deltaTime);
	virtual bool init();
protected:
	GoldFountain();
public:
	~GoldFountain();
	static GoldFountain* StartPlay(Vec2 StartPos, Vec2 EndPos,int type);		//type喷泉类型
};

#endif


