#ifndef FishNode_h
#define FishNode_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

class FishNode : public Node
{
public:
	bool init();
	CREATE_FUNC(FishNode);
	void initWithRes();

	void update(float dt);
private:
	Node* animationNode;
	ActionTimeline *actTimeLine;

	CC_SYNTHESIZE(int, fishId, FishId);
	CC_SYNTHESIZE(std::string, resName, ResName);
};

#endif 