#ifndef __BlackFish_H__
#define __BlackFish_H__

#include "GameEntity.h"
#include "GameDef.h"
#include "PathManager.h"

enum FISH_STATUS
{
	FISH_INVALID = 0,
	FISH_ALIVE,
	FISH_DIED,
	FISH_HURT,
	FISH_OUT,
	FISH_CLEAR,
	FISH_LIGHTNING
};

enum FishGroupType
{
	NONE_GROUP = 0,
	SNAKE_HEAD = 1,
	SNAKE_BODY = 2,
	SNAKE_TAIL = 3,
	THREE_GROUP = 4,
	FOUR_GROUP = 5,
	ALL_NET_GROUP = 6,
	FISH_TYPE_BOMB_GROUP = 7
};

struct CMD_S_CREATE_FISH
{
	unsigned int	nID;
	int				nType;
	unsigned int    nPathID;
	FISH_PATH_TYPE	nPathType;
	float			OffestX;
	float			OffestY;
	float			fDelay;
	unsigned int	nElaspe;
	int			    nData;
	float			fSpeed;
	int             Score;
	float           interval;
	int             randThres;
	cocos2d::Rect   collideRect;
	FishGroupType   groupType;
	int             groupId;
	FishType        fishType;
};



class BlackFish : public GameEntity
{
public:
	BlackFish();
	virtual ~BlackFish();

	void create(CMD_S_CREATE_FISH &cmd);

	virtual void OnUpdate(float dt) override;

	//���ʵ���Ƿ񳬳���Ļ��Χ
	bool InsideScreen();

	//����������·��
	void CaclutePath();

	bool EndPath();

private:
	std::vector<MOVE_POINT> m_path;
	float m_fDuration;        //����ʱ��
	float m_Elaspe;


	CC_SYNTHESIZE(FishType, fishType, FishType);
	CC_SYNTHESIZE_READONLY(bool, m_bOutScreen, OutScreen);		//�Ƿ񳬳���Ļ


	CC_SYNTHESIZE(int, m_wPathID, PathID);
	CC_SYNTHESIZE(cocos2d::Vec2, m_Offest, Offest);
	CC_SYNTHESIZE(float, m_fDelay, Delay);

	CC_SYNTHESIZE(FISH_PATH_TYPE, m_eFishType, PathType);
	CC_SYNTHESIZE(int, m_Prob, Prob);
	CC_SYNTHESIZE(FISH_STATUS, m_Status, Status);			//�㶯��״̬
	CC_SYNTHESIZE(int, m_GroupType, GroupType);
	CC_SYNTHESIZE(int, m_GroupFirstId, GroupFirstId);

	CC_SYNTHESIZE(int, baseScore, BaseScore);
	CC_SYNTHESIZE(int, catchScore, CatchScore);
	CC_SYNTHESIZE(int, catchMultiply, CatchMultiply);
	CC_SYNTHESIZE(int, catchExpMultiply, CatchExpMultiply);
	CC_SYNTHESIZE(int, catchChairId, CatchChairId);
	CC_SYNTHESIZE(int, catchRateThres, CatchRate)
		CC_SYNTHESIZE_READONLY(cocos2d::Rect, collideRect, CollideRect);
};

#endif 




