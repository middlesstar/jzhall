#ifndef __MessageTipNode_H__
#define __MessageTipNode_H__
/************************************************************************/
/* ��ʾ��Ϣ�ڵ�                                                                     */
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
	void addBeginAnim();		//��ӿ�ʼ����

protected:
	void adjustBg();			//����������λ�ã�ÿ���������ݺ����

private:
	LabelTTF* label_massage;	//��Ϣ����

	Sprite* m_pLeftBg;		//��߱�����
	Sprite* m_pMidBg;		//�м䱳����
	Sprite* m_pRightBg;		//�ұ߱�����
};


#endif
