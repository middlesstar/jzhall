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

/* ���ڻ��� */
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

	virtual void close();					                //�رմ���
	virtual void setContent();				                //����UI����
	virtual void updateWindow(int nCode);	                //ˢ�´���
	virtual void updateWindow(int nCode,void* pData);	    //ˢ�´���,��������

public:
	void setTouchSwallow(bool bSwallow = true);             //�������ɴ���
    void addBlackLayer(GLubyte gOpacity = 100);				//��Ӻ�ɫ����

	virtual void ScaleIn(float fScale = 0.0f,float fTime = 0.3f);	//���Ŷ����볡
	virtual void ScaleOut(float fTime = 0.15f);						//���Ŷ�������

	void playButtonClickSound();							//��ť����¼�������ÿ�����涼��Ҫ�������WindowBase�����װһ������
protected:
	Node* _loadCSBLayoutFile(const std::string& strLayoutFile);
	Node* _seekNodeByName(Node* pRootNode,const std::string& name);
	void _addButtonClickEvent(Node* pRootNode, const std::string& btnName, std::function<void(Ref*)> btnCallback);
	
	void close_callback(float dt);		//�رջص�

protected:
    Node* m_pNode;
    LayerColor* m_pLayerColor;

    EventListenerTouchOneByOne* m_pEventListener;

	CC_SYNTHESIZE(bool, m_bSwallow, Swallow);
	CC_SYNTHESIZE(bool, m_bScaleIn, ScaleIn);
};

#endif // _WindowBase_H_

