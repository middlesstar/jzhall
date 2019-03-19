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

	//Һ��
	Vector<Liquid*>				m_liquidList;

	//��
	Sprite*						m_fish;
	bool						m_bIsActivity;				//�Ƿ�
	bool						m_bIsLeft;					//��ͷ�Ƿ������
	enumFishState				m_fishState;				//���״̬


	const float					m_vX;						//X�᷽���ٶ�
	float						m_vY;						//Y�᷽���ٶ�
	static float				DIVE_G;						//��Ǳˮ�ϸ����ٶ�
	static float				_G;							//�������ٶ�
	static float				JUMP_MAX;					//�����ٶ�


public:

	ActiveFish();
	~ActiveFish();
	virtual void OnInit();

	void StopPlay();
	void StartPlay();

	void Jump();										
									

	Rect GetRect();											//�õ����BOX
protected:

	void update(float t);

	void PushLiquid(float force);



private:

};
#endif // _ActiveFish_H_