#ifndef SHZ_BIRD_DEFINE_H_
#define SHZ_BIRD_DEFINE_H_

//////////////////////////////////////////////////////////////////////////
#define MAX_BIRD_IN_MANAGER_SHZ					2048
#define MAX_BULLET_IN_MANAGER_SHZ				2048
//#define SCENE_TIME_END						600
#define MAX_SMALL_PATH_SHZ						217
#define MAX_BIG_PATH_SHZ						130
#define MAX_HUGE_PATH_SHZ						62
#define MAX_SPECIAL_PATH_SHZ					24
#define MAX_SCENE_PATH_SHZ						0


#define MAX_BIRD_CATCH_SHZ						8
#define MAX_DIFFERENCE_SHZ						2000

#define MAX_SCENE_TYPE_SHZ						5

#define MAX_BIRD_TYPE_SHZ						28

#define MAX_LEVEL_TYPE_SHZ						60
#define MAX_BASE_TYPE_SHZ						10
#define MAX_STORAGE_TYPE_SHZ					20

#define PATH_TYPE_SMALL_SHZ						0
#define PATH_TYPE_BIG_SHZ						1
#define PATH_TYPE_HUGE_SHZ						2
#define PATH_TYPE_SPECIAL_SHZ					3
#define PATH_TYPE_SCENE_SHZ						4

//////////////////////////////////////////////////////////////////////////
// ��Ⱥ
//////////////////////////////////////////////////////////////////////////
// ������������
#define MAX_SCENE_SHAPE_SHZ						4
// С��Ⱥ����
#define MAX_SMALL_SHOAL_SHZ						4
// ���׹���Ⱥ����
#define MAX_TOR_SHOAL_SHZ						1
// ������Ⱥ����
#define MAX_EEL_SHOAL_SHZ						1
// ������Ⱥ����
#define MAX_SCENE_SHOAL_SHZ						13

//////////////////////////////////////////////////////////////////////////
// ��Ч������
//////////////////////////////////////////////////////////////////////////
#define BIRD_ITEM_NULL_SHZ						0
#define BIRD_ITEM_SPECIAL_BOMB_1_SHZ			1
#define BIRD_ITEM_SPECIAL_BOMB_2_SHZ			2
#define BIRD_ITEM_SPECIAL_BOMB_3_SHZ			3
#define BIRD_ITEM_SPECIAL_DING_SHZ				4
#define BIRD_ITEM_SPECIAL_LUCKY_SHZ				5
#define BIRD_ITEM_SPECIAL_GOLDX2_SHZ			6
#define BIRD_ITEM_SPECIAL_TOR_SHZ				7
#define BIRD_ITEM_SPECIAL_EEL_SHZ				8
#define BIRD_ITEM_SPECIAL_SMALL_PURSE_SHZ		9
#define BIRD_ITEM_SPECIAL_MIDDLE_PURSE_SHZ		10
#define BIRD_ITEM_SPECIAL_BIG_PURSE_SHZ			11

#define BIRD_ITEM_ZORDER_0_SHZ					200
#define BIRD_ITEM_ZORDER_1_SHZ					201
#define BIRD_ITEM_BOMB_EX_0_SHZ					202
#define BIRD_ITEM_BOMB_EX_1_SHZ					203


//////////////////////////////////////////////////////////////////////////
// ����Ч
//////////////////////////////////////////////////////////////////////////
#define BIRD_EFFECT_NULL_SHZ					0
#define BIRD_EFFECT_GOLDX2_SHZ					1
#define BIRD_EFFECT_LUCKY_SHZ					2
#define BIRD_EFFECT_DING_SHZ					3
#define BIRD_EFFECT_EEL_SHZ						4
#define BIRD_EFFECT_TOR_SHZ						5

//////////////////////////////////////////////////////////////////////////
// ���ζ��ٶ�
//////////////////////////////////////////////////////////////////////////
#define BIRD_FRAME_SPEED_SHZ		0.26f

#define BIRD_MOVE_NORMAL_SHZ		1
#define BIRD_MOVE_RUN_AWAY_SHZ		10



/**
 * �ӵ���С
 */
const float BULLET_RADIUS_SHZ = 10;

/**
 * ���С
 */
const SHZxSize	BIRD_SIZES_SHZ[]  = {
	SHZxSize(41, 37),	SHZxSize(23, 44),	SHZxSize(34, 55),	SHZxSize(34, 72),
	SHZxSize(52, 82),	SHZxSize(41, 86),	SHZxSize(51, 106),	SHZxSize(53, 120),
	SHZxSize(123, 114),	SHZxSize(65, 148),	SHZxSize(145, 139),	SHZxSize(82, 240),	
	SHZxSize(102, 255),	SHZxSize(107, 290),	SHZxSize(174, 300),	SHZxSize(120, 280),
	SHZxSize(200, 330),	SHZxSize(290, 300),	SHZxSize(120, 300), SHZxSize(90, 90),	
	SHZxSize(120, 120),	SHZxSize(90, 90),	SHZxSize(90, 90),	SHZxSize(90, 90),
	SHZxSize(90, 90),	SHZxSize(90, 90),	SHZxSize(90, 90),	SHZxSize(90, 90),
};

/**
 * ����С
 */
const float NET_SIZES_SHZ[] = {
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
	80, 80, 80, 80, 80,
};

//////////////////////////////////////////////////////////////////////////
// ��λ��
//////////////////////////////////////////////////////////////////////////
const int CANNON_POSITION_SHZ[][2] = {
	{200,   41}, {620,  41}, {1050,  41},
	{1050, 659}, {620, 659}, {200,  659}
};

//����ת�Ƕ�
const float CANNON_ANGLE_SHZ[]={180,180,180,0,0,0};
#endif // SHZ_BIRD_DEFINE_H_