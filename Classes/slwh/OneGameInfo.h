#ifndef __OneGameInfo_H__
#define __OneGameInfo_H__

#include "cocos2d.h"
#include "slwh/slwhStruct.h"
#include "slwh/slwhDefine.h"
#include "slwh/slwhNetStruct.h"
using namespace std;
USING_NS_CC;

class OneGameInfo
{
public:
	int *getRate();
	int *getResRecord(){ return m_resRecord; };
	void setResRecord(int* arr);
	//分平台得到音乐路径
	std::string getSLWHMusicFileFullPath(const std::string& strPath);

protected:
	int* m_rate0;						//第一套倍率
	int* m_rate1;						//第二套倍率
	int* m_rate2;						//第三套倍率
	int* m_rate3;						//第四套倍率
	int* m_rate4;						//第五套倍率

	int* m_resRecord;					//记录板数据
		
protected:
	CC_SYNTHESIZE(uint, m_leftTime, LeftTime);							//进入第一局剩余时间
	CC_SYNTHESIZE(enumServerState, m_ServerState, ServerState);			//游戏当前状态
	CC_SYNTHESIZE(bool, m_bIsCountScore, bCountScore);					//这一轮是否进入算分阶段
	CC_SYNTHESIZE(bool, m_bStartGameFinish, StartGameFinish);			//信息是否同步完成
protected:
	CC_SYNTHESIZE(slwhRoomInfo*, m_roomInfo, roomInfo);
	CC_SYNTHESIZE(SLWHTableInfo, m_tableInfo, TableInfo);
	CC_SYNTHESIZE(SLWHUserInfo, m_userInfo,UserInfo);
	CC_SYNTHESIZE(OpenRes, m_openRlt, openRlt);							//普通开奖，D3Y  D4X 开奖结果
	CC_SYNTHESIZE(OpenCJ, m_openCJRlt, openCJRlt);						//彩金开奖
	CC_SYNTHESIZE(Open4LF, m_open4LFRlt, open4LFRlt);					//4连发开奖
	CC_SYNTHESIZE(OpenDMG, m_openDMGRlt, openDMGRlt);					//大满贯开奖
	CC_SYNTHESIZE(OpenXYCJ, m_openXYCJRlt, openXYCJRlt);				//幸运抽奖

	CC_SYNTHESIZE(int, m_scoreChange, scoreChange);                                   //用于记录分数的改变      
public:
	static OneGameInfo* GetInstance();
	void relese();
	~OneGameInfo();
protected:
	OneGameInfo();
	static OneGameInfo* m_instance;
};

#endif

