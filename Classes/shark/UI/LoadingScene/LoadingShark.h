#ifndef _LoadingShark_H_
#define _LoadingShark_H_

#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;

/*
	���ؽ���
	��ǰ���ظ�����Դ
*/


class LoadingShark :
	public Scene
{
protected:
	ValueVector						m_vecTex;
	ValueVector						m_vecModel;
	LoadingBar*						m_loadingBar;
	int								m_AllCount;
	int								m_texIndex;
	int								m_texCount;
	int								m_modelIndex;
	int								m_modelCount;
	int								m_state;
public:
	CREATE_FUNC(LoadingShark)
protected:
	LoadingShark();

	virtual ~LoadingShark();

	virtual bool init();

	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;
	//���ý������ٷֱ�
	void SetProgressBar(float per);
	
	bool loadTex();

	void onAyncTexCallback(Texture2D* ref);

	bool loadModel();

	void onAsyncModelCallback(Sprite3D* sprite, void* param);

public:
	void SetState(int index){ m_state = index; }
};

#endif 