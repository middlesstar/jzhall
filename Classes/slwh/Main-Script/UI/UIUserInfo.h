#ifndef __UIUserInfo_H__
#define __UIUserInfo_H__

#include "slwh/Base/UIBase.h"


/*
主要对主游戏场景的用户信息UI管理
*/


class UIUserInfo :
	public UIBase
{
protected:
	TextAtlas*	m_yuanbao;
	TextAtlas*	m_gold;

public:
	void updateInfo(float t);

	void update();

	//刷新界面显示
	void refreshPlayerCoin(Ref* ref);

	UIUserInfo();

	~UIUserInfo();
protected:


	void Start();

	void SetYunBao(int y);

	void SetGold(int g);
};

#endif

