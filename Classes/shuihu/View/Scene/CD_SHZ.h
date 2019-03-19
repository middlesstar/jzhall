#include "cocos2d.h"
USING_NS_CC;
#define CD_MAX				5

//彩带根目录（数量50）
#ifndef		Path_PNG_LEAVES_Root
#define		Path_PNG_LEAVES_Root				"leaves_"
#endif // !Path_PNG_CD_Root

#ifndef		Path_PNG_FIRE_Root
#define		Path_PNG_FIRE_Root					"petal_"
#endif // !Path_PNG_FIRE_Root




/*
彩带
*/


class CD :
	public Node
{
protected:
	struct OneCD : public Sprite
	{
		int		speed;					//下落速度
		int		rotateSpeed;			//旋转速度 一般和下落速度成正比
		bool	bIsleft;				//是否向左移动

		static OneCD* create(const std::string& filename)
		{

			OneCD *sprite = new (std::nothrow) OneCD();
			if (sprite && sprite->initWithSpriteFrameName(filename))
			{
				//sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}
	};


	bool m_bIsOpen;

	std::string			m_texture2D_leaves[CD_MAX];
	std::string			m_texture2D_fire[CD_MAX];
	Vector<OneCD*>		m_list;
	Vec2				m_visibleSize;
	unsigned int		m_type;
	static CD*			m_instance;

protected:
	CD();

	void CreatOne();
	void update(float deltaTime);
	virtual bool init();

public:
	~CD();
	static CD* GetInstance();
	void StartPlay(unsigned int n);
	void StopPlay();

};

