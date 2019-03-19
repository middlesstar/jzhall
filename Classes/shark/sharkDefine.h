#ifndef  _sharkDefine_H_
#define  _sharkDefine_H_

#include "Sound/SoundManager.h"

#include "df/Module.h"
//游戏属性
#define KIND_ID_SHARK					206  								//游戏 I D
#define GAME_NAME_SHARK					TEXT("金鲨银鲨")						//游戏名字

//组件属性
#define GAME_PLAYER_SHARK			       	1									//游戏人数(暂时填1)
#define VERSION_SERVER_SHARK				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT_SHARK				PROCESS_VERSION(6,0,3)				//程序版本

#define _ZERO 0
#define Animal_MAx 10

//第一套倍率
#define _Rate1(x)			{2,x,24,2,6,8,8,12,12,8,8,6}
//第二套倍率
#define _Rate2(x)			{2,x,24,2,3,12,24,24,24,24,12,3}
//第三套倍率
#define _Rate3(x)			{2,x,24,2,4,6,24,24,24,24,6,4}
//第四套倍率
#define _Rate4(x)			{2,x,24,2,4,8,12,24,24,12,8,4}
//第五套倍率
#define _Rate5(x)			{2,x,24,2,6,8,8,24,24,8,8,6}
//第六套倍率
#define _Rate6(x)			{2,x,24,2,6,6,8,24,24,8,6,6}
//第七套倍率
#define _Rate7(x)			{2,x,24,2,6,6,12,12,12,12,6,6}


#define _PLAY_SOUND_SHARK(X)	SoundManager::shared()->playEffectNormal(X)

#define _PLAY_MUSCI_SHARK(X,Y)	SoundManager::shared()->playMusicNormal(X,Y)

#define _STOP_MUSCI_SHARK		SoundManager::shared()->stopMusic()
      



#define SET_GET_FUNC(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }
#endif // _sharkDefine_H_

