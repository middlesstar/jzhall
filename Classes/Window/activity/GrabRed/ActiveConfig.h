#ifndef  _ActiveConfig_H_
#define  _ActiveConfig_H_


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LanguageConfig.h"
#include "Sound/SoundManager.h"
using namespace CocosDenshion;
USING_NS_CC;

extern int _RAND0;
extern int _RAND1;
extern int _RAND2;

extern Vec2 VISIBLE_SIZE;
//std::string WStrToUTF8(const std::wstring& str);


//运行游戏
#ifndef _RUN_FISH_ACTIVETY(_Type ,CallBack ,This)
#define _RUN_FISH_ACTIVETY(_Type ,CallBack ,This) \
	auto x = _Type::create(); \
	Director::getInstance()->getRunningScene()->addChild(x); \
	x->SetCallback(This, (SEL_CallFun)(&CallBack)); \
	x->StartPlay();
#endif


#ifndef _PLAY_MUSCI_EFF(X)
//#define _PLAY_MUSCI_EFF(X)	SimpleAudioEngine::getInstance()->playEffect(X);
#define _PLAY_MUSCI_EFF(X)		SoundManager::shared()->playEffectNormal(X)
#endif 

#ifndef _PLAY_MUSCI_BACK(X)
//#define _PLAY_MUSCI_BACK(X)	SimpleAudioEngine::getInstance()->playBackgroundMusic(X,true);
#define _PLAY_MUSCI_BACK(X)	SoundManager::shared()->playMusicNormal(X,true)
#endif 

//主管要求缩放比例(鱼.红包.水花)
#define ALL_SCALE				0.7

#ifndef _TRANSPARENT
#define _TRANSPARENT			255
#endif // !_TRANSPARENT


#ifndef _ZERO
#define _ZERO					0
#endif // !_ZERO

#ifndef _PI
#define _PI						3.1415926
#endif // !PI

#ifndef RES_PLIST_PNG
#define RES_PLIST_PNG
#define	PLIST_PATH				"images/hall/activity/red_packet.plist"
#define	_BK						"rp_blue.png"
#define _TOP					"rp_top.png"
#define _WAVE0					"rp_wave0.png"
#define _WAVE1					"rp_wave1.png"
#define	_FISH0					"rp_fish0.png"
#define	_FISH1					"rp_fish1.png"
#define	_RBOMB0					"rp_bomb_0.png"
#define _RBOMB1					"rp_bomb_1.png"
#define RED_PAKAGE0				"rp_red_0.png"		
#define RED_PAKAGE1				"rp_red_1.png"	
#define RED_PAKAGE2				"rp_red_2.png"	
#define	_TICKET0				"rp_ticket_0.png"
#define RP_DROPS				"rp_drops.png"
#define _BAO					"rp_box.png"

#endif // !COCOS_PLIST_PNG

//右上方标题
#define TEXT_CONTENT			GETSTRING(1300)
#define	TEXT_FONT				GETSTRING(1301)
#define TEXT_SIZE				24


//Info信息标签
#define	INFO_TIME				GETSTRING(1302)
#define	INFO_GOLD				GETSTRING(1303)
#define INFO_HF					GETSTRING(1304)
#define INFO_SIZE				22
#define INFO_LABEL_COLOR		Color3B(0,255,0)
#define INFO_TEXT_COLOR			Color3B(255,255,0)
#define INFO_TIME_MAX			60								//60秒
#define INFO_YOFFSET			33								//Y轴位置属性


//鱼 属性
#define FISH_MIN_HEIGHT			100								//鱼游动的Y坐标
#define	FISH_OFFSET_X			77								//鱼撞墙偏移 图片宽度的一半
#define	FISH_JUMP_SLIDER		5								//跳起角度平滑系数
#define FISH_MOVE_X_V			200								//鱼横向移动速度
#define FISH_DIVE_V				500								//上浮加速度
#define FISH_DIVE_LOSE			0.3								//进入水面能量损失
#define FISH_G					1000							//重力加速度
#define FISH_JUMP_V				1100							//鱼跳起速度
#define FISH_JUMP_FORCE1		0.7								//鱼起跳力1
#define FISH_JUMP_FORCE2		0.6								//鱼起跳力2
#define FISH_JUMP_LOSE			0.15							//2连跳能量损失

//水滴属性



//红包属性
#define PACKET_TIME				0.4								//红包密集度  0.8s出个红包
#define	PACKET_V_MIN			200								//红包速度small
#define	PACKET_V_MID			500								//红包速度mid
#define PACKET_V_MAX			800								//红包速度big
#define	PACKET_TEXT_SIZE		30								//红包分数文字大小
#define PACKET_TEXT_COLOR0		Color3B(255,0,0)			
#define PACKET_TEXT_COLOR1		Color3B(255,0,0)		
#define PACKET_LAYER_COLOR		Color4B(255,0,0,255)	
#define PACKET_TEXT_V_Y			100								//文字上浮速度
#define PACKET_TEXT_MAX_TIME	1.f								//文字上浮时间	
#define PACKET_SCREEN_CLEAR		.5f								//炸弹清屏时间	
#define PACKET_TRANSPARENT		128								//炸弹清屏透明度
#define PACKET_UP_DOWN_V		50								//红包上下浮动速度	
#define PACKET_UP_DOWN_TIME		.5f								//红包上下浮动时间
#define	PACKET_ROTATE_ANGLE		60								//红包最大旋转角度
#define	PACKET_ROTATE_TIME		.5f								//红包旋转时间
#define PACKET_HF_MAX			15								//话费得到超过后不再产生
#define PACKET_HF_FLOAT			10.f							//话费精确一位小数 （2位小数为100  以此类推）



//音乐path
#define MUSIC_BK				"music/ios/activity/pk.mp3"					//背景音乐
#define MUSIC_FISH_JUMP			"music/ios/activity/click.mp3"					//鱼跳起
#define	MUSCI_EAT_PACKET		"music/ios/activity/get_gold.mp3"				//迟到红包
#define MUSCI_EAT_BOMB_GET		"music/ios/activity/mfish_gold.mp3"			//好炸弹
#define MUSCI_EAT_BOMB_DIE		"music/ios/activity/mfish_31.mp3"				//坏炸弹

#endif // _ActiveConfig_H_


