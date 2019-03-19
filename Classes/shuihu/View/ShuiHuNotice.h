#ifndef _SHUIHU_NOTICE
#define _SHUIHU_NOTICE	

#include "df/types.h"
#include "cocos2d.h"
//#include "Shark/resourceDef.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#define      COCOS_STUDIO_SHARKNOTICE       "shark/JZNoticeLayer.csb"

USING_NS_CC;
using namespace ui;


class ShuiHuNotice:
	public Node
{
protected:
	Node*	m_rootNode;
	Text*	m_text;							
	Button* m_okbt;							//OK��ť
	std::function<void(Ref*)> m_callback;	
protected:
	ShuiHuNotice();
	~ShuiHuNotice();

	void destory(Node* n);

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	CREATE_FUNC(ShuiHuNotice);
public:
	//Ψһ�ĵ��ýӿ�,�ص��ǵ��ȷ�ϰ�ť��ִ�У������ָ����this
	static void openNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);
};

#endif