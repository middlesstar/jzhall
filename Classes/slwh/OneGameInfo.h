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
	//��ƽ̨�õ�����·��
	std::string getSLWHMusicFileFullPath(const std::string& strPath);

protected:
	int* m_rate0;						//��һ�ױ���
	int* m_rate1;						//�ڶ��ױ���
	int* m_rate2;						//�����ױ���
	int* m_rate3;						//�����ױ���
	int* m_rate4;						//�����ױ���

	int* m_resRecord;					//��¼������
		
protected:
	CC_SYNTHESIZE(uint, m_leftTime, LeftTime);							//�����һ��ʣ��ʱ��
	CC_SYNTHESIZE(enumServerState, m_ServerState, ServerState);			//��Ϸ��ǰ״̬
	CC_SYNTHESIZE(bool, m_bIsCountScore, bCountScore);					//��һ���Ƿ������ֽ׶�
	CC_SYNTHESIZE(bool, m_bStartGameFinish, StartGameFinish);			//��Ϣ�Ƿ�ͬ�����
protected:
	CC_SYNTHESIZE(slwhRoomInfo*, m_roomInfo, roomInfo);
	CC_SYNTHESIZE(SLWHTableInfo, m_tableInfo, TableInfo);
	CC_SYNTHESIZE(SLWHUserInfo, m_userInfo,UserInfo);
	CC_SYNTHESIZE(OpenRes, m_openRlt, openRlt);							//��ͨ������D3Y  D4X �������
	CC_SYNTHESIZE(OpenCJ, m_openCJRlt, openCJRlt);						//�ʽ𿪽�
	CC_SYNTHESIZE(Open4LF, m_open4LFRlt, open4LFRlt);					//4��������
	CC_SYNTHESIZE(OpenDMG, m_openDMGRlt, openDMGRlt);					//�����Ὺ��
	CC_SYNTHESIZE(OpenXYCJ, m_openXYCJRlt, openXYCJRlt);				//���˳齱

	CC_SYNTHESIZE(int, m_scoreChange, scoreChange);                                   //���ڼ�¼�����ĸı�      
public:
	static OneGameInfo* GetInstance();
	void relese();
	~OneGameInfo();
protected:
	OneGameInfo();
	static OneGameInfo* m_instance;
};

#endif

