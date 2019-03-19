#ifndef _SHARK_NOTICE
#define _SHARK_NOTICE	

#include "df/types.h"
#include "cocos2d.h"
#include "Shark/resourceDef.h"
USING_NS_CC;



class SharkNotice:
	public Node
{
protected:
	Node*	m_rootNode;
	Text*	m_text;							
	Button* m_okbt;							//OK��ť
	std::function<void(Ref*)> m_callback;	
protected:
	SharkNotice();
	~SharkNotice();

	void destory(Node* n);

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	CREATE_FUNC(SharkNotice);
public:
	//Ψһ�ĵ��ýӿ�,�ص��ǵ��ȷ�ϰ�ť��ִ�У������ָ����this
	static void openNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);
};

#endif