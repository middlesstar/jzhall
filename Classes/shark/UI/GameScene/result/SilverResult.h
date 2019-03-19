#ifndef  _SilverResult_H_
#define  _SilverResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class  SilverResult : public ViewBase
{
public:
	SilverResult();
	virtual ~SilverResult();
	CREATE_FUNC(SilverResult);

	virtual void setContent();

	//开启闪烁
	void startPlay();
	//关闭闪烁
	void stopPlay();

	//幸运时刻到来特效
	void luckTime();
	
protected:
	//根节点
	Node*              pMainNode;
	Sprite*            light;
	Sprite*            luckLight;

};

#endif // _SilverResult_H_

