#ifndef __MessageTipNode_H__
#define __MessageTipNode_H__
/************************************************************************/
/* 提示信息节点                                                                     */
/************************************************************************/

#include "cocos2d.h"
USING_NS_CC;

class MessageTipNode : public Sprite
{
public:
	MessageTipNode(void);
	~MessageTipNode(void);
	static MessageTipNode* createWithMessage(const char* cMessage);
	CREATE_FUNC(MessageTipNode);
	bool init();
	void addBeginAnim();		//添加开始动画

protected:
	void adjustBg();			//调整背景框位置，每次设置内容后调用

private:
	LabelTTF* label_massage;	//信息内容

	Sprite* m_pLeftBg;		//左边背景框
	Sprite* m_pMidBg;		//中间背景框
	Sprite* m_pRightBg;		//右边背景框
};


#endif
