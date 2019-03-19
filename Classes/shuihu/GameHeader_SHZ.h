#ifndef SHZ__GameHeader_H_
#define SHZ__GameHeader_H_

#include "cocos2d.h"
#include "shuihu/plazz/Plazz_SHZ.h"
#include "shuihu/StaticData_SHZ.h"
#include "df/StringData.h"
#include "shuihu/Game/xPoint_SHZ.h"
#include "shuihu/Game/xSize_SHZ.h"
#include "shuihu/Game/xRect_SHZ.h"
#include "shuihu/Game/BirdDefine_SHZ.h"
#include "shuihu/Game/Bird_SHZ.h"
#include "shuihu/Game/Bullet_SHZ.h"
#include "shuihu/Game/CMD_CatchBird_SHZ.h"
#include "shuihu/Game/GameConfig_SHZ.h"
#include "libs/libs.h"
inline cocos2d::CCPoint toCCP_SHZ(const SHZxPoint& pt)
{
	return cocos2d::CCPointMake(pt.x_, kRevolutionHeight_SHZ-pt.y_);
}

inline cocos2d::CCPoint toCCP_SHZ(float x, float y)
{
	return cocos2d::CCPointMake(x, kRevolutionHeight_SHZ-y);
}

inline cocos2d::CCSize  toCCS_SHZ(const SHZxSize& si)
{
	return cocos2d::CCSizeMake(si.width_, si.height_);
}

inline cocos2d::CCRect  toCCR_SHZ(const SHZxRect& rt)
{
	return cocos2d::CCRectMake(rt.left_, kRevolutionHeight_SHZ-rt.top_, rt.get_width(), rt.get_height());
}

inline float toCCRotation_SHZ(float angle)
{
	return angle*180/M_PI;
}

inline SHZxPoint toNetPoint_SHZ(const cocos2d::CCPoint& pt)
{
	return SHZxPoint(pt.x, kRevolutionHeight_SHZ-pt.y);
}

inline SHZxPoint toNetPoint_SHZ(float x, float y)
{
	return SHZxPoint(x, kRevolutionHeight_SHZ-y);
}

inline SHZxSize toNetSize_SHZ(const cocos2d::CCSize& si)
{
	return SHZxSize(si.width, si.height);
}

inline SHZxRect toNetRect_SHZ(const cocos2d::CCRect& rt)
{
	return SHZxRect(rt.origin.x, kRevolutionHeight_SHZ-rt.origin.y, rt.size.width, rt.size.height);
}

inline float toNetRotation_SHZ(float angle)
{
	return angle*M_PI/180;
}

//static const char* SERVER_ADRRESS_SHZ	= "192.168.10.213";

#define  SERVER_PORT_SHZ SHZStaticData::shared()->getPort()

/**
 * 游戏模式
 * 0	网络版（使用用户名和密码登陆）
 * 1	单机版
 * 3	网络版（快速登录）
 */
#define TAG_MODE_NETWORK 0
#define TAG_MODE_LOCAL   1
#define TAG_MODE_QUICK	 2

#endif // _GameHeader_H_
