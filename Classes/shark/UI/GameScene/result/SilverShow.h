#ifndef _SilverShow_H_
#define _SilverShow_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class SilverShow : public ViewBase
{
public:
	SilverShow();
	~SilverShow();

	//����������ת
	void startPlay();
	//�ر�������ת
	void stopPlay();

protected:
	void setContent();

	void runRotate(Node* node);  //��Ȧ��ת
protected:
	Node*      mainNode;
	Sprite*    circle1;
	Sprite*    circle2;
	Sprite*    circle3;

};


#endif // _RateShow_H_