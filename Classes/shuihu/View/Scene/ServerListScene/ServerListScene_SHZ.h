#ifndef SHZ_ServerListScene_H_
#define SHZ_ServerListScene_H_

#include "cocos2d.h"

class SHZServerListScene
	: public cocos2d::CCScene
{
public:
	static SHZServerListScene* create(int mode);

	//ÅÐ¶ÏÊÇ·ñµ¯³ö³äÖµ¿ò
	void judgeRecharge();

private:
	SHZServerListScene();
	~SHZServerListScene();
	bool init(int mode);

public:
	virtual void onEnterTransitionDidFinish();
public:
	void closeCallback(cocos2d::CCNode *pNode); 
private:
	int mMode;
	
};
#endif // _ServerListScene_H_ 