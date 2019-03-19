#ifndef _SLWH_GOLD_FALLEN	
#define _SLWH_GOLD_FALLEN	

#include "cocos2d.h"
USING_NS_CC;

#define		Max_Gold	150
#define		Max_Tex		7

//#ifndef		PLIST_PATH
//#define		PLIST_PATH							""
//#endif

#ifndef		SLWH_GoldFallen_Gold
#define		SLWH_GoldFallen_Gold					"images/common/animation/gold1/gold1_"
#endif


class SLWH_GoldFallen :
	public Node
{
private:
	Vec2				m_visibleSize;
	const int			SpeedAdd;
	int					now_index;						//��ǰ����������
	bool				m_bIsOpen;

	static SLWH_GoldFallen*	m_instance;
protected:
	std::string	m_Texture[Max_Tex];

	struct SLWH_OneGold :public Sprite
	{
		float v_y;						//�����ٶ�
		float time;					    //���䷭ת�¼���ʱ
		bool jump;						//�Ƿ���

		static SLWH_OneGold* create(const std::string& filename)
		{
			SLWH_OneGold *sprite = new (std::nothrow) SLWH_OneGold();
			if (sprite && sprite->initWithFile(filename))		// ->initWithSpriteFrameName(filename)
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}

		static SLWH_OneGold* create()
		{
			SLWH_OneGold *sprite = new (std::nothrow) SLWH_OneGold();
			if (sprite && sprite->init())
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}
	};

	Vector<SLWH_OneGold*> m_list;

protected:

	void CreatOne();

	virtual void update(float deltaTime);
	virtual bool init();
protected:

	SLWH_GoldFallen();

public:

	~SLWH_GoldFallen();
	static SLWH_GoldFallen* GetInstance();
	static void StartPlay();
	static void StopPlay();
};

#endif