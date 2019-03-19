#ifndef _StakeItem_H_
#define _StakeItem_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"
#include "shark/resourceDef.h"

class StakeItem : public Widget
{
public:
	void setRate(int rate);
	void setStakeNum(SCORE num);
	void setLogo(int index);
protected:
	StakeItem();

	//回调销毁
	void CallBack(Node* node);

	
public:    
	virtual ~StakeItem();

	Widget* getWgt();


	static void initTemplate(ImageView* node);					
	static StakeItem* create();
	ImageView* getRootNdoe(){ return m_rootNode; }

protected:
	ImageView*			m_rootNode;								
	ImageView*          m_logo; 
	Text*               m_beilV; 
	TextAtlas*          m_stakeShow;

	static StakeItem*	m_templateThis;							//模板
	//AllJetInfo			stakeInfo;								//玩家信息
	
	

};

#endif // _StakeItem_H_