#ifndef  _WindowBase_H_
#define  _WindowBase_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"

#include "CustomMessage.h"
#include "HallGameScene.h"
#include "Sound/SoundManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace cocos2d::ui;

/* 窗口基类 */
class  WindowBase : public Layer
{
public:
    WindowBase();
    virtual ~WindowBase();

	virtual void onEnter() override;
	virtual void onExit() override;

	CREATE_FUNC(WindowBase);
	virtual bool init() override;
	virtual void onBackKeyPressed();

	virtual void close();					                //关闭窗口
	virtual void setContent();				                //设置UI布局
	virtual void updateWindow(int nCode);	                //刷新窗口
	virtual void updateWindow(int nCode,void* pData);	    //刷新窗口,附带参数

public:
	void setTouchSwallow(bool bSwallow = true);             //设置吞噬触摸
    void addBlackLayer(GLubyte gOpacity = 100);				//添加黑色背景

	virtual void ScaleIn(float fScale = 0.0f,float fTime = 0.3f);	//缩放动画入场
	virtual void ScaleOut(float fTime = 0.15f);						//缩放动画出场

	void playButtonClickSound();							//按钮点击事件，基本每个界面都需要，因此在WindowBase里面封装一个方法
protected:
	Node* _loadCSBLayoutFile(const std::string& strLayoutFile);
	Node* _seekNodeByName(Node* pRootNode,const std::string& name);
	void _addButtonClickEvent(Node* pRootNode, const std::string& btnName, std::function<void(Ref*)> btnCallback);
	
	void close_callback(float dt);		//关闭回调

protected:
    Node* m_pNode;
    LayerColor* m_pLayerColor;

    EventListenerTouchOneByOne* m_pEventListener;

	CC_SYNTHESIZE(bool, m_bSwallow, Swallow);
	CC_SYNTHESIZE(bool, m_bScaleIn, ScaleIn);
};

#endif // _WindowBase_H_

