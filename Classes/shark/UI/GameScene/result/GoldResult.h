#ifndef  _GoldResult_H_
#define  _GoldResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"

/* ���ڻ��� */
class  GoldResult : public ViewBase
{
public:
	GoldResult();
	virtual ~GoldResult();
	CREATE_FUNC(GoldResult);

	virtual void setContent();

	//������˸
	void startPlay();
	//�ر���˸
	void stopPlay();
	
	//��ʾ��������Ч
	void boastLight();
protected:
	Node*          pMainNode;
	Sprite*        light;
	Sprite*        blastLight;
};

#endif // _GoldResult_H_

