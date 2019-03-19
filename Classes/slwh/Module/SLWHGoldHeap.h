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
	int					m_maxgold;						//��߽��
	int					m_nowindex;						//��ǰ�������
	int					m_targetindex;					//Ŀ������
	bool				m_bIsRunning;					//�Ƿ���������		
	Sprite**			m_spriteArr;					//����ָ������
	Text*				m_text;							//ѹ���ܶ�
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