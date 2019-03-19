#ifndef _GOLD_FOUNTAIN	
#define _GOLD_FOUNTAIN	

/*
	�����Ȫ
*/

#include "cocos2d.h"
USING_NS_CC;

#ifndef PLIST_PATH
#define PLIST_PATH														//PLIST��·��				
#endif


#ifndef	Path_PNG_Gold
#define	Path_PNG_Gold							"images/common/animation/gold1/gold1_0.png"
#endif


class GoldFountain :
	public Node
{
private:
	//���Ա���
	float				Gold_V_X_MIN; 					//������ٶ���Сֵ
	float				Gold_V_X_MAX;					//������ٶ����ֵ
	float				Gold_V_Y_MIN;					//������ٶ���Сֵ
	float				Gold_V_Y_MAX;					//������ٶ����ֵ
	float				Gold_V_TIME_MIN;				//����˶���Сʱ��
	float				Gold_V_TIME_MAX;				//����˶����ʱ��
	float				Gold_V_TIME_PAUSE;				//���ͣ��ʱ��
	float				Gold_V_TIME_SHOOT;				//��������ʱ��
	float				Gold_Rotate_Speed;				//Ӳ����תһȦ��ʱ��
	float				Gold_G;							//�������ٶ�(ֻ�������˶�)
	//int					Max_Gold;						//��Ҳ����ĸ���
	float				Max_Gold_Time;					//���������ʱ��
	int					Layer_Level;					//�㼶


	float				now_index;						//��ǰ���ʱ��
	bool				m_bIsOpen;			
	Vec2				m_StartPos;
	Vec2				m_EndPos;
protected:
	struct OneGold :public Sprite
	{
		float v_x;						//������ٶ�
		float v_y;						//������ٶ�
		float time;						//���˶���ʱ���˶�ʱ��
		bool  bIsShoot;					//�Ƿ�����˳�̽׶�
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
	static GoldFountain* StartPlay(Vec2 StartPos, Vec2 EndPos,int type);		//type��Ȫ����
};

#endif


