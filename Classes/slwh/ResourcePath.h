/*
	资源路径
*/

#ifndef _RESOURCE_PATH
#define _RESOURCE_PATH

#include "OneGameInfo.h"
#include "df/DF.h"
#include "libs/PacketAide.h"
#include "Player/Player.h"
#include "SimpleAudioEngine.h"
#include "slwhDefine.h"
#include "slwhNetStruct.h"
#include "slwhStruct.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace ui;
using namespace CocosDenshion;

#define		COCOS_STUDIO_GAMESCENE				"slwh/GameScene.csb"
#define		COCOS_STUDIO_HALLSCENE				"slwh/HallScene.csb"
#define		COCOS_STUDIO_PROGRESSSCENE			"slwh/ProgressScene.csb"
#define		COCOS_STUDIO_JET_PANEL				"slwh/SLWH_JET.csb"
#define		COCOS_STUDIO_USER_LIST				"slwh/SLWH_USER_LIST.csb"
#define		COCOS_STUDIO_OPEN					"slwh/SLWH_OPEN.csb"
#define		COCOS_STUDIO_UPBT					"slwh/UP_BT.csb" 
#define		COCOS_STUDIO_CJ						"slwh/CJ.csb"
#define		COCOS_STUDIO_XYCJ					"slwh/XYCJ.csb"
#define		COCOS_STUDIO_NOTICE					"slwh/SLWH_Notice.csb"


//返回上一层提示框
#define    COCOS_STUDIO_RETURNNOTICE			"shark/JZReturnLayer.csb"
	
//主游戏场景3D模型
#define		Path_Terrian						"slwh/Model/Scene.c3b"
#define		Path_LightAll						"slwh/Model/LightAll.c3b"
#define		Path_Step							"slwh/Model/AniDown.c3b"
#define		Path_DownTo							"slwh/Model/DownTo.c3b"
#define		Path_Line							"slwh/Model/BreatheLight.c3b"
#define		Path_Sphere							"slwh/Model/Sphere.c3b"
#define		Path_Stage							"slwh/Model/Stage.c3b"
#define		Path_Point							"slwh/Model/Point.c3b"
#define		Path_S								"slwh/Model/S.c3b"

//特殊场景
#define		Path_D4X							"slwh/Model/D4X.c3b"
#define		Path_DMG							"slwh/Model/DMG.c3b"


//新版动物
#define		Path_Monkey							"images/common/animal/houzi.c3b"
#define		Path_Rabbit							"images/common/animal/tuzi.c3b"
#define		Path_Lion							"images/common/animal/shizi.c3b"
#define		Path_Panda							"images/common/animal/xiongmao.c3b"

//UI资源路径


//UI	
//#define		Path_PNG_Jetton_Up					"slwh/game/game_scene/bet_back.png"
//#define		Path_PNG_Jetton_Down				"slwh/game/game_scene/bet_back2.png"

#define		Path_PNG_Title_NORMAL1				"game/game_scene/gs_tip0.png"
//#define		Path_PNG_Title_NORMAL2				"slwh/game/game_scene/gs_tip1.png"
//#define		Path_PNG_Title_NORMAL3				"slwh/game/game_scene/gs_tip2.png"
#define		Path_PNG_Title_CJ					"game/game_scene/gs_tip3.png"
#define		Path_PNG_Title_XYCJ					"game/game_scene/gs_tip4.png"
#define		Path_PNG_Title_D3Y					"game/game_scene/gs_tip5.png"
#define		Path_PNG_Title_D4X					"game/game_scene/gs_tip6.png"
#define		Path_PNG_Title_4LF					"game/game_scene/gs_tip7.png"
#define		Path_PNG_Title_DMG					"game/game_scene/gs_tip8.png"


//彩带根目录（数量50）

#define		Path_PNG_CD_Root					"images/common/animation/ticker/"


//小动物（用于记录板 和 压分面板）
#define		Path_SMALL_Red_Animal0				"game/animals_small/as_red_rabbit.png"
#define		Path_SMALL_Red_Animal1				"game/animals_small/as_red_monkey.png"
#define		Path_SMALL_Red_Animal2				"game/animals_small/as_red_panda.png"
#define		Path_SMALL_Red_Animal3				"game/animals_small/as_red_lion.png"

#define		Path_SMALL_Green_Animal0			"game/animals_small/as_green_rabbit.png"
#define		Path_SMALL_Green_Animal1			"game/animals_small/as_green_monkey.png"
#define		Path_SMALL_Green_Animal2			"game/animals_small/as_green_panda.png"
#define		Path_SMALL_Green_Animal3			"game/animals_small/as_green_lion.png"

#define		Path_SMALL_Yellow_Animal0			"game/animals_small/as_yellow_rabbit.png"
#define		Path_SMALL_Yellow_Animal1			"game/animals_small/as_yellow_monkey.png"
#define		Path_SMALL_Yellow_Animal2			"game/animals_small/as_yellow_panda.png"
#define		Path_SMALL_Yellow_Animal3			"game/animals_small/as_yellow_lion.png"

#define		Path_SMALL_ALL_Animal0				"game/animals_small/as_three_rabbit.png"
#define		Path_SMALL_ALL_Animal1				"game/animals_small/as_three_monkey.png"
#define		Path_SMALL_ALL_Animal2				"game/animals_small/as_three_panda.png"
#define		Path_SMALL_ALL_Animal3				"game/animals_small/as_three_lion.png"

#define		Path_SMALL_Red_AnimalAll			"game/animals_small/as_four_red.png"
#define		Path_SMALL_Green_AnimalAll			"game/animals_small/as_four_green.png"
#define		Path_SMALL_Yellow_AnimalAll			"game/animals_small/as_four_yelolow.png"

#define		Path_SMALL_All_AnimalAll			"game/animals_small/as_all.png"


//开奖结果庄和闲	
#define		Path_RES_PNG_ZHUANG					"game/game_results/gr_zhuang.png"
#define		Path_RES_PNG_HE						"game/game_results/gr_he.png"
#define		Path_RES_PNG_XIAN					"game/game_results/gr_xian.png"


//庄和闲
#define		Path_PNG_ZHUANG						"game/game_scene/gs_result_zhuang.png"
#define		Path_PNG_HE							"game/game_scene/gs_result_he.png"
#define		Path_PNG_XIAN						"game/game_scene/gs_result_xian.png"

//开奖界面特殊标题
#define		Path_Spe_Res_D3Y					"game/game_results/gr_dsy.png"
#define		Path_Spe_Res_D4X					"game/game_results/gr_dsx.png"
#define		Path_Spe_Res_4LF					"game/game_results/gr_slf.png"
#define		Path_Spe_Res_CJ						"game/game_results/gr_cjll.png"
#define		Path_Spe_Res_DMG					"game/game_results/gr_dmg.png"
#define		Path_Spe_Res_XYCJ					"game/game_results/gr_xycj.png"


//音乐
#define		GREEN_RABBIT						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Green_Rabbit").c_str()
#define		GREEN_LION							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Green_Lion").c_str()
#define		GREEN_MONKEY						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Green_Monkey").c_str() 
#define		GREEN_PANDA							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Green_Panda").c_str() 
#define		RED_LION							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Red_Lion").c_str() 
#define		RED_MONKEY							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Red_Monkey").c_str() 
#define		RED_PANDA							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Red_Panda").c_str() 
#define		RED_RABBIT							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Red_Rabbit").c_str() 
#define		YELLOW_LION							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Yellow_Lion").c_str() 
#define		YELLOW_MONKEY						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Yellow_Monkey").c_str() 
#define		YELLOW_PANDA						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Yellow_Panda").c_str() 
#define		YELLOW_RABBIT						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/Yellow_Rabbit").c_str() 
#define		LOTTERYTYPE_GREEN					 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Green").c_str() 
#define		LOTTERYTYPE_RED						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Red").c_str() 
#define		LOTTERYTYPE_YELLOW					 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Yellow").c_str() 
#define		LOTTERYTYPE_LION					 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Lion").c_str() 
#define		LOTTERYTYPE_MONKEY					 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Monkey").c_str() 
#define		LOTTERYTYPE_PANDA					 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Panda").c_str() 
#define		LOTTERYTYPE_RABBIT					 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/LotteryType_Rabbit").c_str() 
#define		RABBIT_CALL							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/nobody").c_str() 						//兔子叫声
#define		PANDA_CALL							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/apple").c_str() 							//熊猫叫声
#define		LION_CALL							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/style").c_str() 						//狮子叫声
#define		MONKEY_CALL							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/dangerous").c_str() 					//猴子叫声
#define		_XIAN								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/xian").c_str() 
#define		_HE									 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/he").c_str() 
#define		_ZHUANG								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/zhuang").c_str() 

#define		HI_TIME								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/hi_time").c_str() 
#define		_TIME0								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/time0").c_str() 
#define		_TIME1								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/time1").c_str() 
#define		_DJKS								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/djks").c_str() 							//大奖开始
#define		_KJJS								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/kjjs").c_str() 							//开奖结束
#define		_JETTON								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/jet").c_str() 		

#define		RES_ADD								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/resadd").c_str() 
#define		ZP_START							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/zp_start").c_str() 
#define		ZP_RUN								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/zp_run").c_str() 
#define		_KZ									 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/kz").c_str() 
#define		XYCJ_HI_TIME2						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/hi_time2").c_str() 
#define		XYCJ_LUCK_BG						 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/luck_bg").c_str() 
#define		XYCJ_RUN							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/run").c_str() 
#define		XYCJ_STOP							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/stop").c_str() 
#define		JET_WAIT							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/jetwait").c_str() 

#define		_CLICK								 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("common/Click").c_str() 			
#define		_BETCOIN							 OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("common/gold1").c_str() 


#endif