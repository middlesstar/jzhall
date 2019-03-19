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

	//������˸
	void startPlay();
	//�ر���˸
	void stopPlay();

	//����ʱ�̵�����Ч
	void luckTime();
	
protected:
	//���ڵ�
	Node*              pMainNode;
	Sprite*            light;
	Sprite*            luckLight;

};

#endif // _SilverResult_H_

