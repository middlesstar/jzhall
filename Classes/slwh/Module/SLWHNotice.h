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
	Button* m_okbt;							//OK按钮
	std::function<void(Ref*)> m_callback;	
protected:
	SLWHNotice();
	~SLWHNotice();

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	CREATE_FUNC(SLWHNotice);
public:
	//唯一的调用接口,回掉是点击确认按钮后执行，传入的指针是this
	static void openNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);
};

#endif