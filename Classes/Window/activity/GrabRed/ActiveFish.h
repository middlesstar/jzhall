#ifndef  _ActiveFish_H_
#define  _ActiveFish_H_

#include "ui/CocosGUI.h"
#include "cocos2d.h"


USING_NS_CC;
using namespace ui;

class ActiveFish :
	public Node
{


protected:
	struct Liquid : public Sprite
	{
		float m_vX;
		float m_vY;
		float m_vZ;
		float m_rotate;
		static Liquid* createWithSpriteFrame(SpriteFrame *spriteFrame)
		{
			Liquid *sprite = new (std::nothrow) Liquid();
			if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}
	};

	enum enumFishState{
		NORMAL,
		JUMP_1,
		JUMP_2,
		DIVE
	};

	//液滴
	Vector<Liquid*>				m_liquidList;

	//鱼
	Sprite*						m_fish;
	bool						m_bIsActivity;				//是否活动
	bool						m_bIsLeft;					//鱼头是否朝向左边
	enumFishState				m_fishState;				//鱼的状态


	const float					m_vX;						//X轴方向速度
	float						m_vY;						//Y轴方向速度
	static float				DIVE_G;						//鱼潜水上浮加速度
	static float				_G;							//重力加速度
	static float				JUMP_MAX;					//弹跳速度


public:

	ActiveFish();
	~ActiveFish();
	virtual void OnInit();

	void StopPlay();
	void StartPlay();

	void Jump();										
									

	Rect GetRect();											//得到鱼的BOX
protected:

	void update(float t);

	void PushLiquid(float force);



private:

};
#endif // _ActiveFish_H_