#ifndef  _resouceDef_H_
#define  _resouceDef_H_

#include "GameInfo.h"
#include "df/DF.h"
#include "libs/PacketAide.h"
#include "Player/Player.h"
#include "SimpleAudioEngine.h"
#include "sharkDefine.h"
#include "sharkNetStruct.h"
#include "sharkStruct.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace CocosDenshion;
#define      COCOS_STUDIO_BATTLSCENE        "shark/JZBattleLayer.csb"
#define      COCOS_STUDIO_STAKE             "shark/JZBetLayer.csb"
#define      COCOS_STUDIO_BOARD             "shark/JZLeftBoard.csb"
#define      COCOS_STUDIO_RESULT            "shark/JZResultLayer.csb"
#define      COCOS_STUDIO_SHARKNOTICE       "shark/JZNoticeLayer.csb"
   
#define     COCOS_STUDIO_SELECTSCENE        "shark/SelectRoom.csb"
//菜单界面
#define     COCOS_STUDIO_MENU               "shark/JZMenuLayer.csb"

//等待界面
#define     COCOS_STUDIO_WAIT               "shark/JZWaitLayer.csb"

//加载进度条界面 
#define     COCOS_STUDIO_LOADING            "shark/SharkLoading.csb"

//返回上一层提示框
#define    COCOS_STUDIO_RETURNNOTICE        "shark/JZReturnLayer.csb"


//音乐
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//android平台
#define     BACKGROUDMUSIC1                "ogg/backgroundmusic1.ogg"
#define     BACKGROUDMUSIC2                "ogg/backgroundmusic2.ogg"
#define     DOVE_MUSIC                     "ogg/dove.ogg"
#define		EAGLE_MUSIC                    "ogg/eagle.ogg"
#define     GOLDSHARK_MUSIC                "ogg/goldshark.ogg"
#define     LASTTIME                       "ogg/lasttime.ogg"
#define     LASTMOVE                       "ogg/lastmove.ogg"
#define     LION_MUSIC                     "ogg/lion.ogg"
#define     LOSER                          "ogg/loser.ogg"
#define     MONKEY_MUSIC                   "ogg/monkey.ogg"
#define     PANDA_MUSIC                    "ogg/panda.ogg"
#define     PEACOCK_MUSIC                  "ogg/peacock.ogg"
#define     PRESS                          "ogg/press.ogg"
#define     RABBIT_MUSIC                   "ogg/rabbit.ogg"
#define     SHARKTIME_MUSIC                "ogg/sharktime.ogg"
#define     SILVERSHARK_MUSIC              "ogg/silvershark.ogg"
#define     SWALLOW_MUSIC                  "ogg/swallow.ogg"
#define     TIMEOVER                       "ogg/timeover.ogg"
#define     TURN_MUSIC                     "ogg/trun.ogg"
#define     WIN1_MUSIC                     "ogg/win1.ogg"
#define     WIN2_MUSIC                     "ogg/win2.ogg"
#define     WIN3_MUSIC                     "ogg/win3.ogg"
#define     WINER_MUSIC                    "ogg/winer.ogg"
#define     GO_MUSIC                       "ogg/go.ogg"
#define     LIGHT_MUSIC                    "ogg/light.ogg"
#define     ONE_MUSIC                      "ogg/one.ogg"
#define     WINGOLD_MUSIC                  "music/android/common/goldwin.ogg"
#else 
//ios平台
#define     BACKGROUDMUSIC1                "mp3/backgroundmusic1.mp3"
#define     BACKGROUDMUSIC2                "mp3/backgroundmusic2.mp3"
#define     DOVE_MUSIC                     "mp3/dove.mp3"
#define		EAGLE_MUSIC                    "mp3/eagle.mp3"
#define     GOLDSHARK_MUSIC                "mp3/goldshark.mp3"
#define     LASTTIME                       "mp3/lasttime.mp3"
#define     LASTMOVE                       "mp3/lastmove.mp3"
#define     LION_MUSIC                     "mp3/lion.mp3"
#define     LOSER                          "mp3/loser.mp3"
#define     MONKEY_MUSIC                   "mp3/monkey.mp3"
#define     PANDA_MUSIC                    "mp3/panda.mp3"
#define     PEACOCK_MUSIC                  "mp3/peacock.mp3"
#define     PRESS                          "mp3/press.mp3"
#define     RABBIT_MUSIC                   "mp3/rabbit.mp3"
#define     SHARKTIME_MUSIC                "mp3/sharktime.mp3"
#define     SILVERSHARK_MUSIC              "mp3/silvershark.mp3"
#define     SWALLOW_MUSIC                  "mp3/swallow.mp3"
#define     TIMEOVER                       "mp3/timeover.mp3"
#define     TURN_MUSIC                     "mp3/trun.mp3"
#define     WIN1_MUSIC                     "mp3/win1.mp3"
#define     WIN2_MUSIC                     "mp3/win2.mp3"
#define     WIN3_MUSIC                     "mp3/win3.mp3"
#define     WINER_MUSIC                    "mp3/winer.mp3"
#define     GO_MUSIC                       "mp3/go.mp3"
#define     LIGHT_MUSIC                    "mp3/light.mp3"
#define     ONE_MUSIC                      "mp3/one.mp3"
#define     WINGOLD_MUSIC                  "music/ios/common/goldwin.mp3"

#endif


#define       PATH_MONKEY               "images/common/animal/houzi.c3b"       
#define       PATH_LION                 "images/common/animal/shizi.c3b"
#define       PATH_RABBIT               "images/common/animal/tuzi.c3b"
#define       PATH_PANDA                "images/common/animal/xiongmao.c3b"
#define       PATH_SWALLOW              "images/shak/animal/yanzi.c3b"
#define       PATH_DOVE                 "images/shak/animal/gezi.c3b"
#define       PATH_PEACOCK              "images/shak/animal/kongque.c3b"
#define       PATH_HAWK                 "images/shak/animal/laoying.c3b"
#define       PATH_GOLD                 "images/shak/animal/shayu.c3b"
#define       PATH_SILVER               "images/shak/animal/shayu.c3b"

#define       IMAGE_COLD                "images/shak/animal/shayu_jin.png"
#define       IMAGE_SILVER              "images/shak/animal/shayu_yin.png"

//按钮烟花特效
#define       PATH_BTNEFFECT             "images/common/Eff/yanhua.plist"
//金币飞的资源
#define       GOLD_FLY                   "images/common/animation/gold3/gold3.png"

//彩金来袭的特效
#define        PATH_BLAST                "images/common/Eff/card_2.plist"

//动物的logo
#define        LOGO_GOLD                 "images/shak/new_animals/na_little_10.png"
#define        LOGO_SILVER               "images/shak/new_animals/na_little_9.png"
#define        LOGO_SWALLOW              "images/shak/new_animals/na_little_8.png"
#define        LOGO_DOVE                 "images/shak/new_animals/na_little_7.png"
#define        LOGO_PEACOCK              "images/shak/new_animals/na_little_6.png"
#define        LOGO_HAWK                 "images/shak/new_animals/na_little_5.png"
#define        LOGO_LION                 "images/shak/new_animals/na_little_1.png"
#define        LOGO_PANDA                "images/shak/new_animals/na_little_2.png"
#define        LOGO_MONKEY               "images/shak/new_animals/na_little_3.png"
#define        LOGO_RABBIT               "images/shak/new_animals/na_little_4.png"



#endif // _resouceDef_H_

