#ifndef _SLWHReturn_NOTICE
#define _SLWHReturn_NOTICE	

#include "df/types.h"
#include "cocos2d.h"
#include "slwh/ResourcePath.h"
USING_NS_CC;


class SLWHReturn:
	public Node
{
protected:
	Node*	rootNode;
	Text*	text;							
	Button* m_okbt;      //OK��ť
	Button* m_cancelbt;
	std::function<void(Ref*)> m_callback;	

	static SLWHReturn* m_instance;
protected:
	SLWHReturn();

	~SLWHReturn();

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	virtual void onEnter() override;

	virtual void onExit() override;

	CREATE_FUNC(SLWHReturn);
public:
	
	static bool GetBOpen();
	//Ψһ�ĵ��ýӿ�,�ص��ǵ��ȷ�ϰ�ť��ִ�У������ָ����this
	static void openReturnNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);

	static void cancelDialog();
};

#endif