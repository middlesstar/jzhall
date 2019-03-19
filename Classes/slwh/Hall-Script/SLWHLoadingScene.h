#pragma once
#include "ui/CocosGUI.h"
#include "cocos2d.h"
//#include "slwh/Main-Script/SLWHGameScene.h"

USING_NS_CC;
using namespace ui;

/*
	加载界面
	提前加载各种资源
*/


class SLWHLoadingScene :
	public Scene
{
public:
	//static SLWHGameScene*			m_pMainGameScene;

protected:
	ValueVector						m_vecTex;
	ValueVector						m_vecModel;
	LoadingBar*						m_loadingBar;
	int								m_AllCount;
	int								m_texIndex;
	int								m_texCount;
	int								m_modelIndex;
	int								m_modelCount;
public:
	CREATE_FUNC(SLWHLoadingScene)
protected:
	SLWHLoadingScene();

	virtual ~SLWHLoadingScene();

	virtual bool init();

	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;
	//设置进度条百分比
	void SetProgressBar(float per);
	
	bool loadTex();

	void onAyncTexCallback(Texture2D* ref);

	bool loadModel();

	void onAsyncModelCallback(Sprite3D* sprite, void* param);

	//加载音乐
	bool loadMusic();
public:
	//void SetState(int index){ m_state = index; }
};

