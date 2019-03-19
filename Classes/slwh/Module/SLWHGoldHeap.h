#ifndef _SLWH_GOLD_HEAP
#define _SLWH_GOLD_HEAP	

#include "df/types.h"
#include "cocos2d.h"
#include "slwh/ResourcePath.h"
USING_NS_CC;


#ifndef		SLWH_GoldHeap_GOLD
#define		SLWH_GoldHeap_GOLD					"images/common/animation/gold2/bet_gold2.png"		//38*7
#endif

#define		Path_PNG_Gold_Light					"images/common/animation/gold2/linght.png"


class SLWH_GoldHeap :
	public Node
{
private:
	int					m_maxgold;						//最高金币
	int					m_nowindex;						//当前金币索引
	int					m_targetindex;					//目标索引
	bool				m_bIsRunning;					//是否正在运行		
	Sprite**			m_spriteArr;					//精灵指针数组
	Text*				m_text;							//压分总额
	Sprite*				m_light;	
protected:
	virtual void Running(float deltaTime);
	virtual bool init(int maxgold);
protected:
	SLWH_GoldHeap();
	~SLWH_GoldHeap();
public:

	static SLWH_GoldHeap* create(int maxgold);
	void StartPlay(float per,longlong gold);
	void StartPlay(int index,longlong gold);
	void StopPlay();
	
};

#endif