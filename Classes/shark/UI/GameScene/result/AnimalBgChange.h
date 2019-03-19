#ifndef _GeneralResult_H_
#define _GeneralResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class AnimalBgChange : public ViewBase
{
public:
	AnimalBgChange();
	~AnimalBgChange();

	//������˸
	void startPlay();
	//�ر���˸
	void stopPlay();
protected:
	void setContent();

private:
	ImageView*   animalBg;


};

#endif // _GeneralResult_H_