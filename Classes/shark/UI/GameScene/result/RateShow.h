#ifndef _RateShow_H_
#define _RateShow_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class RateShow : public ViewBase
{
public:
	RateShow();
	~RateShow();

	//������˸
	void startPlay();
	//�ر���˸
	void stopPlay();

	void setBeiLv(int index);
protected:
	void setContent();

public:
	TextAtlas*    rateLabel;


};


#endif // _RateShow_H_