#ifndef __JCNum_H__
#define __JCNum_H__

#include "slwh/Base/UIBase.h"

/*
����Ϸ���� ���أ�JC����UI������
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

