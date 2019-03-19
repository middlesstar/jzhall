#ifndef __GameInfo_H__
#define __GameInfo_H__

#include "cocos2d.h"
#include "shark/sharkStruct.h"
#include "shark/sharkDefine.h"
#include "shark/sharkNetStruct.h"
using namespace std;
USING_NS_CC;

class GameInfo
{
public:
	int *getRate();
	int *getResRecord(){ return m_resRecord; };
	void setResRecord(int* arr);
	

protected:
	int* m_rate0;						//第一套倍率
	int* m_rate1;						//第二套倍率
	int* m_rate2;						//第三套倍率
	int* m_rate3;						//第一套倍率
	int* m_rate4;						//第二套倍率
	int* m_rate5;						//第三套倍率
	int* m_rate6;						//第三套倍率

	int* m_resRecord;					//记录板数据
	int  sharkRate;                      //金鲨倍率
		
protected:
	CC_SYNTHESIZE(int, m_leftTime, LeftTime);							//进入第一局剩余时间
	CC_SYNTHESIZE(gameState, m_ServerState, serverState);			    //游戏当前状态
	CC_SYNTHESIZE(bool, m_bStartGameFinish, startGameFinish);
	CC_SYNTHESIZE(RoomInfo_Shark*, m_roomInfo, roomInfor);

	CC_SYNTHESIZE(SharkTableInfo, m_tableInfo, tableInfor);
	CC_SYNTHESIZE(SharkUserInfo, m_userInfo,userInfor);
	CC_SYNTHESIZE(OpenResult, m_openRlt, openRelt);						     	//开奖结果
	CC_SYNTHESIZE(SharkCJ, m_sharkCj, sharkCj);
	CC_SYNTHESIZE(int, m_circleFlag, circleFlag);                              //记录光标停的位置
	CC_SYNTHESIZE(ownStakeInfo, m_ownInfo, ownInfo);
	CC_SYNTHESIZE(bool, m_tophead, tophead);                                  //记录自己的成绩是否进入前十
	CC_SYNTHESIZE(Scene*, m_game, gameScene);                             
	CC_SYNTHESIZE(int,m_changeScore,changeScore);                             //记录玩家金币在服务端改变时的数据
	CC_SYNTHESIZE(int, m_changeYuanBao, changeYuanBao);                       //记录玩家元宝在服务端改变时的数据
	CC_SYNTHESIZE(int, m_roomIndex, roomIndex);                               //记录进入房间的索引

	CC_SYNTHESIZE(bool,m_doReturn,doReturn);                                  //记录返回提示框的状态


 
	//决定背景音乐用哪一个
	void selectMusicBack();
public:
	static GameInfo* GetInstance();
	
protected:
	GameInfo();
	~GameInfo();
	static GameInfo* m_instance;
	
};

#endif

