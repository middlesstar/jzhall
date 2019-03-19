#ifndef __UIUserInfo_H__
#define __UIUserInfo_H__

#include "slwh/Base/UIBase.h"


/*
��Ҫ������Ϸ�������û���ϢUI����
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

	//ˢ�½�����ʾ
	void refreshPlayerCoin(Ref* ref);

	UIUserInfo();

	~UIUserInfo();
protected:


	void Start();

	void SetYunBao(int y);

	void SetGold(int g);
};

#endif

