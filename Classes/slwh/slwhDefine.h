#ifndef _SLWH_DEFINE
#define _SLWH_DEFINE

#include "df/Module.h"
#include "Sound/SoundManager.h"

//游戏属性
#define KIND_ID_SLWH					205  								//游戏 I D
#define GAME_NAME_SLWH					TEXT("森林舞会")						//游戏名字

//组件属性
#define GAME_PLAYER_SLWH				1									//游戏人数(暂时填1)
#define VERSION_SERVER_SLWH				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT_SLWH				PROCESS_VERSION(6,0,3)				//程序版本



//设计分辨率
#define	DesignResolutionSize			(cocos2d::Size(1074, 740))

#define Animal_Max	24
#define _PI	3.14159265f
#define _ORIGINAL 255
#define _GRAY 180
#define _ZERO 0
#define TRANSPARENT_MAX		255

//颜色层
#define COLOR_LAYER_TANSPARENT	200
#define COLOR_LAYER_LEVEL	-1

//固定动物顺序
#define	_AnimalArray	{1, 0, 1, 3, 0, 1, 0, 2, 0, 2, 1, 3, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 3}

#define ZHUANG_BEILV	2
#define	HE_BEILV		8
#define	XIAN_BEILV		2


//第一套倍率 顺序(红色)兔子~狮子 （黄色）。。。
#define _BeiLv0			{8, 13, 23, 46, 7, 11, 20, 40, 4, 7, 12, 25, 2, 8, 2}
//第二套倍率
#define _BeiLv1			{10, 16, 24, 40, 8, 12, 18, 36, 6, 8, 14, 28, 2, 8, 2}
//第三套倍率
#define _BeiLv2			{6, 10, 24, 30, 5, 9, 20, 26, 4, 8, 18, 24, 2, 8, 2}
//第四套倍率
#define _BeiLv3			{8, 16, 24, 44, 6, 14, 18, 38, 4, 12, 12, 32, 2, 8, 2}
//第五套倍率
#define _BeiLv4			{10, 12, 24, 44, 9, 10, 18, 34, 8, 8, 12, 24, 2, 8, 2}





//#define _PLAY_MUSCI_EFF(X)		SimpleAudioEngine::getInstance()->playEffect(X) 
#define _PLAY_MUSCI_EFF(X)		SoundManager::shared()->playEffectNormal(X)
#define _MUSCI_VOLUME_EFF(X)	SimpleAudioEngine::getInstance()->setEffectsVolume(X)


//#define _PLAY_MUSCI_BACK(X,Y)	SimpleAudioEngine::getInstance()->playBackgroundMusic(X,Y)
#define _PLAY_MUSCI_BACK(X,Y)	SoundManager::shared()->playMusicNormal(X,Y)
#define _MUSCI_VOLUME_BACK(X)	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(X)

#define _STOP_MUSCI_BACK		SoundManager::shared()->stopMusic()

//关联纹理ID与纹理 ID = 类型*100 + 颜色*10 + 动物 
#define		_RES_KEY(a,b,c)						((int)a *100 + (int)b * 10 + (int)c)


#define SET_GET_FUNC(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }


#endif // !_SLWH_DEFINE
