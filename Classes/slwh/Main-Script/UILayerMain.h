#ifndef __UILayerMain_H__
#define __UILayerMain_H__

#include "cocos2d.h"
#include "UI/MyJetton/MyJettonManager.h"
#include "UI/UIUserInfo.h"
#include "UI/TimeCount.h"
#include "UI/ResRecord.h"
#include "UI/JCNum.h"
#include "UI/UIMenu.h"
#include "UI/Regular.h"
#include "UI/ZHXControl.h"
#include "UI/Result/ResultManager.h"
#include "slwh/Main-Script/UI/UserList/UserListManager.h"
#include "slwh/Main-Script/UI/WaitNextJet.h"
#include "slwh/Main-Script/UI/JetAndResManager.h"
USING_NS_CC;


#ifndef _ZERO
#define _ZERO 0
#endif // !_ZERO


class UILayerMain : public cocos2d::Layer
{
protected:
	Node*					m_rootNode;					//UI���ڵ�
	Node*                   Jetroot;                    
	Node*                   listroot;
	Node*                   openroot;
	Node*                   jetbt;
	bool					m_bEnble;					//�Ƿ����Ѻע

public:
	MyJettonManager			m_myJetManager;				//Ѻע���������
	UserListManager			m_userListManager;			//�û��б������
	ResultManager			m_resManager;				//���������
	JetAndResManager		m_jetandresManager;			//ѹ�ֺͽ������(����һ����ť)

	UIUserInfo				m_uiUserInfo;				//��Ϸ����ķ���
	WaitNextJet				m_waitNextJet;				//�ȴ���һ�ο���
	ResRecord				m_resRecord;				//��¼��
	JCNum					m_jcNum;					//���ؽ����
	UIMenu					m_uiMenu;					//�˵�
	ZHXControl				m_zhxControl;				//ZHX����
	
public:
	//virtual void onEnter() override;

	//virtual void onExit() override;

	virtual bool init();
	UILayerMain();
	~UILayerMain();
	//��Ļ������Ӧ�¼�
	bool onTouchBegan(Touch* touch, Event* event);
	//��ť��Ӧ�¼�
	//void OutBetEvent(Ref *pSender, TouchEventType type);
	void SetEnble(bool b);

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;

	CREATE_FUNC(UILayerMain);


};

#endif

