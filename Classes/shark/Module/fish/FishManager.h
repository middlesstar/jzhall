#ifndef __FishManager_H__
#define __FishManager_H__

#include "cocos2d.h"
#include "GameUtils_Shark.h"
#include "BlackFish.h"
USING_NS_CC;




struct GroupFirstIdSet
{
	GroupFirstIdSet() :group(0), count(0){}
	GroupFirstIdSet(int i, int j) :group(i), count(j){}
	int group;
	int count;
};

typedef struct __FishConfig
{
	int configId;
}FishConfig;

class FishManager : public Singleton<FishManager>
{
public:
	FishManager();
	~FishManager();

	void logic(float dt);

	void createAllFish();

	//�����
	void clear();

	void removeFish(int fishId);

	void createNewFish(CMD_S_CREATE_FISH &cmd);


	//������е���
	void removeAllFish();

	//�����ظ��������·��
	int createFishPath(int index, int circleFlag);

public:
	std::unordered_map<int, BlackFish> fishMap;

	std::vector<int> smallPathRand;

private:
	unsigned int IncrementID;

	CMD_S_CREATE_FISH cmdCreate;
};

#endif

