#include "FishLayer.h"
#include "PathManager.h"


FishLayer::FishLayer()
:fishManager(nullptr)
, sunManager(nullptr)
{
}


FishLayer::~FishLayer()
{
}


bool FishLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	//������Ч
	sunManager = CWater::create();
	this->addChild(sunManager);

	//������
	PathManager::instance()->initPath("images/shak/");
	fishManager = FishManager::instance();	
	fishManager->createAllFish();
	createFish();	

	scheduleUpdate();
	return true;
}


bool FishLayer::createFish()
{
	for (auto itr : fishManager->fishMap) {
		auto fish = FishNode::create();
		fish->setFishId(itr.first);
		fish->initWithRes();
		addChild(fish);
	}

	return true;
}

void FishLayer::update(float dt)
{
	fishManager->logic(dt);

	auto fishVec = getChildren();

	for (auto itr : fishVec)
	{
		((FishNode*)itr)->update(dt);
	}
}

//������е���
void FishLayer::removeFish()
{
	fishManager->removeAllFish();
}
