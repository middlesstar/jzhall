#ifndef _ReturnDialog_NOTICE
#define _ReturnDialog_NOTICE	

#include "df/types.h"
#include "cocos2d.h"
#include "Shark/resourceDef.h"
USING_NS_CC;


class ReturnDialog:
	public Node
{
protected:
	Node*	rootNode;
	Text*	text;							
	Button* m_okbt;      //OK��ť
	Button* m_cancelbt;
	std::function<void(Ref*)> m_callback;	

	static ReturnDialog* m_instance;
protected:
	ReturnDialog();

	~ReturnDialog();

	void destory(Node* n);

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	CREATE_FUNC(ReturnDialog);
public:
	
	static bool GetBOpen();
	//Ψһ�ĵ��ýӿ�,�ص��ǵ��ȷ�ϰ�ť��ִ�У������ָ����this
	static void openReturnNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);

	static void cancelDialog();

	virtual void onEnter() override;

	virtual void onExit() override;
};

#endif