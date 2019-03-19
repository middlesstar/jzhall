#ifndef _GameDef_H_
#define _GameDef_H_
#include "cocos2d.h"

static cocos2d::Size pathResolutionSize = cocos2d::Size(1074, 740);

#define MAX_SMALL_PATH				35
#define MAX_BIG_PATH                130
#define MAX_HUGE_PATH               62
#define MAX_SPECIAL_PATH            24
#define MAX_SCENE1_PATH             241
#define MAX_SCENE2_PATH             215
#define MAX_SCENE3_PATH             243

#define MAX_FISHARRAY_PATH          8

#define MAX_FISHARRAY_STYLE         7

#define MAX_GOLD_CHALLENGE_TIMES    3
#define GOLD_CHALLENGE_FEE1         0
#define GOLD_CHALLENGE_FEE2         1000
#define GOLD_CHALLENGE_FEE3         5000

#define MAX_GOLD_STAGE              18

#define MAX_SCENE_PROB_STYLE        4
#define REBATE_TIME_LIMIT           2400
#define ENERGY_TIME_LIMIT           10
#define ENERGY_THRES                3000


//// ”„¬∑æ∂¿‡–Õ
enum FISH_PATH_TYPE
{
	SMALL_FISH_PATH = 0,
	BIG_FISH_PATH,
	HUGE_FISH_PATH,
	SPECIAL_FISH_PATH,
	SCENE1_FISH_PATH,
	SCENE2_FISH_PATH,
	SCENE3_FISH_PATH,

	FISH_ARRAY_PATH,

	FISH_ARRAY_PATH_EX1,
	FISH_ARRAY_PATH_EX2,
	FISH_ARRAY_PATH_EX3,
	FISH_SLASH_PATH,
	MAXTYPE_FISH_PATH,
};


enum FishType
{
	SMALL_FISH = 0,
	TINY_BOSS_FISH = 1,
	BOSS_FISH = 2,
	GROUP_FISH = 3,
	SPECIAL_FISH = 4,
	SPECIAL_BOSSS_FISH = 5
};

#endif