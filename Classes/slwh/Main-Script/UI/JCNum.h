#ifndef __JCNum_H__
#define __JCNum_H__

#include "slwh/Base/UIBase.h"

/*
主游戏场景 奖池（JC）的UI管理类
*/

class JCNum :
	public UIBase
{
protected:
	TextAtlas* m_num;

public:
	JCNum();
	~JCNum();
	void SetNum(int u);
	int GetNum();
protected:
	void Start();
};

#endif

