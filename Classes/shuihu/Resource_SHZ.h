#ifndef  _Resource_H_
#define  _Resource_H_


#include "Sound/SoundManager.h"



//音乐
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//android平台
#define     BACKGROUNDROOM3                "shuihu/sound_android/palace.ogg"
#define     DRAGON                         "shuihu/sound_android/dragon.ogg"
#define     PRESS                          "shuihu/sound_android/press.ogg"
#define     TURNCARD                       "shuihu/sound_android/right.ogg"
#define     OTHERPRESS                     "music/android/hall/ui.ogg"
#define     GOLDWIN                        "music/android/common/goldwin.ogg"

#else 
//ios平台
#define     BACKGROUNDROOM3                "shuihu/sounds/palace.mp3"
#define     DRAGON                         "shuihu/sounds/dragon.mp3"
#define     PRESS                          "shuihu/sounds/press.mp3"
#define     TURNCARD                       "shuihu/sounds/right.mp3"
#define     OTHERPRESS                     "music/ios/hall/ui.mp3"
#define     GOLDWIN                        "music/ios/common/goldwin.mp3"

#endif


#define _PLAY_SOUND_SHZ(X)	SoundManager::shared()->playEffectNormal(X)

#define _PLAY_MUSCI_SHZ(X,Y)	SoundManager::shared()->playMusicNormal(X,Y)

#define _STOP_MUSCI_SHZ		SoundManager::shared()->stopMusic()


#endif // _resouceDef_H_

