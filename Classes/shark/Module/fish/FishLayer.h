#ifndef __FishLayer_H__
#define __FishLayer_H__

#include "cocos2d.h"
#include "FishNode.h"
#include "FishManager.h"
#include "Water.h"
USING_NS_CC;


class FishLayer : public Layer
{
public:
	FishLayer();
	~FishLayer();

	bool init();

	CREATE_FUNC(FishLayer);

	bool createFish();

	void update(float dt);

	//清除所有的鱼
	void removeFish();


private:
	FishManager*     fishManager;
	CWater*          sunManager;
};

#endif