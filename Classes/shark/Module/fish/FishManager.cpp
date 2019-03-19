#include "FishManager.h"



FishManager::FishManager() :IncrementID(0)
{
	clear();
}


FishManager::~FishManager()
{
	IncrementID = 0;
	clear();
	fishMap.clear();
}

void FishManager::clear()
{
	IncrementID = 0;

	fishMap.clear();
}

void FishManager::logic(float dt)
{
	for (auto fishItr : fishMap)
	{
		fishMap[fishItr.first].OnUpdate(dt);
	}
}

//创建所有的鱼
void FishManager::createAllFish()
{
	for (int i = 0; i < 10; i++) {
		CMD_S_CREATE_FISH cmd;
		cmd.nID = IncrementID;
		cmd.nType = abs(rand()) % 7;

		cmd.nElaspe = 0;
		cmd.fDelay = 0;
		cmd.fSpeed = 1;
		cmd.nData = 0;

		cmd.groupType = NONE_GROUP;
		cmd.groupId = -1;
		cmd.fishType = SMALL_FISH;

		int pathIndex = abs(rand()) % 30;
		cmd.nPathID = createFishPath(pathIndex, i);
		log("-------------cmd.nPathID = %d---------------",cmd.nPathID);

		
	    cmd.OffestX = 0;
		cmd.OffestY = 0;
		createNewFish(cmd);
	}
}
//避免重复创建鱼的路径
int FishManager::createFishPath(int index, int circleFlag)
{
	int pathNum = 0;
	if (index == 0)
	{
		pathNum = circleFlag + 1;
	}
	else
	{
		pathNum = index;
		if (IncrementID == 0)
		{
		}
		else
		{
			for (int i = 1; i < IncrementID + 1; i++)
			{
				if (pathNum == fishMap[i].getPathID())
				{
					int newIndex = abs(rand()) % 30;
					pathNum = createFishPath(newIndex, i);
				}
				else
				{
					if (i == IncrementID)
					{
						break;
					}
				}
			}
		}

	}

	return pathNum;
}


void FishManager::createNewFish(CMD_S_CREATE_FISH &cmd)
{
	IncrementID++;
	BlackFish bFish;
	bFish.create(cmd);

	fishMap[IncrementID] = bFish;
}

void FishManager::removeFish(int fishId)
{
	fishMap.erase(fishId);
}

//清除所有的鱼
void FishManager::removeAllFish()
{
	IncrementID = 0;

	fishMap.clear();
}
