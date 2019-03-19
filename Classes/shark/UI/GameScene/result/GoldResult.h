#ifndef  _GoldResult_H_
#define  _GoldResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"

/* 窗口基类 */
class  GoldResult : public ViewBase
{
public:
	GoldResult();
	virtual ~GoldResult();
	CREATE_FUNC(GoldResult);

	virtual void setContent();

	//启动闪烁
	void startPlay();
	//关闭闪烁
	void stopPlay();
	
	//显示爆机了特效
	void boastLight();
protected:
	Node*          pMainNode;
	Sprite*        light;
	Sprite*        blastLight;
};

#endif // _GoldResult_H_

