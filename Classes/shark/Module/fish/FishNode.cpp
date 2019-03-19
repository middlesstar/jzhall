#include "FishNode.h"
#include "FishManager.h"
#include "MathAide.h"

const int indexFish[7] = { 3, 5, 8, 9, 10, 13, 14 };
bool FishNode::init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}

//获取鱼游动的动画
void FishNode::initWithRes()
{
	//    auto map = FishManager::instance()->fishMap;
	if (FishManager::instance()->fishMap.find(fishId) == FishManager::instance()->fishMap.end())
	{
		return;
	}

	auto data = FishManager::instance()->fishMap[fishId];

	std::string resDir("game_scene/fish/small_fish_alive/afish_");
	log("------------data.type = %d-------------", data.getTypeID());
	resDir += Convert2String(indexFish[data.getTypeID()]);
	//resDir += data.getResource();
	resDir += ".csb";

	resName = resDir;

	animationNode = CSLoader::createNode(resName);
	animationNode->setAnchorPoint(Vec2(0.5, 0.5));
	animationNode->setPosition(Vec2::ZERO);
	addChild(animationNode);

	actTimeLine = CSLoader::createTimeline(resName);
	animationNode->runAction(actTimeLine);

	actTimeLine->play("alive", true);
	animationNode->setScale(0.8);


	setPosition(-2000, -2000);
}

//刷新鱼的位置
void FishNode::update(float dt)
{
	if (FishManager::instance()->fishMap.find(fishId) == FishManager::instance()->fishMap.end())
	{
		return;
	}

	auto data = FishManager::instance()->fishMap[fishId];

	if (data.getOutScreen()) {
		data.setStatus(FISH_OUT);

		FishManager::instance()->removeFish(fishId);
		removeFromParent();
		return;
	}
	setPosition(data.getPosition());
	setRotation(data.getDirection());
}
