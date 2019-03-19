#ifndef _SharkShow_H_
#define _SharkShow_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class SharkShow : public ViewBase
{
public:
	SharkShow();
	~SharkShow();

	//启动鲨鱼旋转
	void startPlay();
	//关闭鲨鱼旋转
	void stopPlay();

protected:
	void setContent();

	void runRotate(Node* node);  //光圈旋转
protected:
	Node*      mainNode;
	Sprite*    circle1;
	Sprite*    circle2;
	Sprite*    circle3;

};


#endif // _RateShow_H_