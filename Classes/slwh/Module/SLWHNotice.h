#ifndef _SLWH_NOTICE
#define _SLWH_NOTICE	

#include "df/types.h"
#include "cocos2d.h"
#include "slwh/ResourcePath.h"
USING_NS_CC;



class SLWHNotice :
	public Node
{
protected:
	Node*	m_rootNode;
	Text*	m_text;							
	Button* m_okbt;							//OK��ť
	std::function<void(Ref*)> m_callback;	
protected:
	SLWHNotice();
	~SLWHNotice();

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	CREATE_FUNC(SLWHNotice);
public:
	//Ψһ�ĵ��ýӿ�,�ص��ǵ��ȷ�ϰ�ť��ִ�У������ָ����this
	static void openNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);
};

#endif