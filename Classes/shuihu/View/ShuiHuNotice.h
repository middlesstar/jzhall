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
	Button* m_okbt;							//OK按钮
	std::function<void(Ref*)> m_callback;	
protected:
	ShuiHuNotice();
	~ShuiHuNotice();

	void destory(Node* n);

	bool onTouchBegan(Touch* touch, Event* event){ return true; };

	CREATE_FUNC(ShuiHuNotice);
public:
	//唯一的调用接口,回掉是点击确认按钮后执行，传入的指针是this
	static void openNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr);
};

#endif