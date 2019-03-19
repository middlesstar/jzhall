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
	int* m_rate0;						//��һ�ױ���
	int* m_rate1;						//�ڶ��ױ���
	int* m_rate2;						//�����ױ���
	int* m_rate3;						//��һ�ױ���
	int* m_rate4;						//�ڶ��ױ���
	int* m_rate5;						//�����ױ���
	int* m_rate6;						//�����ױ���

	int* m_resRecord;					//��¼������
	int  sharkRate;                      //���豶��
		
protected:
	CC_SYNTHESIZE(int, m_leftTime, LeftTime);							//�����һ��ʣ��ʱ��
	CC_SYNTHESIZE(gameState, m_ServerState, serverState);			    //��Ϸ��ǰ״̬
	CC_SYNTHESIZE(bool, m_bStartGameFinish, startGameFinish);
	CC_SYNTHESIZE(RoomInfo_Shark*, m_roomInfo, roomInfor);

	CC_SYNTHESIZE(SharkTableInfo, m_tableInfo, tableInfor);
	CC_SYNTHESIZE(SharkUserInfo, m_userInfo,userInfor);
	CC_SYNTHESIZE(OpenResult, m_openRlt, openRelt);						     	//�������
	CC_SYNTHESIZE(SharkCJ, m_sharkCj, sharkCj);
	CC_SYNTHESIZE(int, m_circleFlag, circleFlag);                              //��¼���ͣ��λ��
	CC_SYNTHESIZE(ownStakeInfo, m_ownInfo, ownInfo);
	CC_SYNTHESIZE(bool, m_tophead, tophead);                                  //��¼�Լ��ĳɼ��Ƿ����ǰʮ
	CC_SYNTHESIZE(Scene*, m_game, gameScene);                             
	CC_SYNTHESIZE(int,m_changeScore,changeScore);                             //��¼��ҽ���ڷ���˸ı�ʱ������
	CC_SYNTHESIZE(int, m_changeYuanBao, changeYuanBao);                       //��¼���Ԫ���ڷ���˸ı�ʱ������
	CC_SYNTHESIZE(int, m_roomIndex, roomIndex);                               //��¼���뷿�������

	CC_SYNTHESIZE(bool,m_doReturn,doReturn);                                  //��¼������ʾ���״̬


 
	//����������������һ��
	void selectMusicBack();
public:
	static GameInfo* GetInstance();
	
protected:
	GameInfo();
	~GameInfo();
	static GameInfo* m_instance;
	
};

#endif

