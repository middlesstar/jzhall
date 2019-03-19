#ifndef  _LogicPromote_H_
#define  _LogicPromote_H_

#include "Window/LogicBase.h"

class  LogicPromote : public LogicBase
{
public:
	LogicPromote();
	virtual ~LogicPromote();

	SINGLE_INSTANCE_FUNC(LogicPromote);

protected:
	void _registerMessageHandler();

public:
	void requestShareSucess(int nType);								//�������ɹ������
	void onShareSucessResp(const PacketAide& packet, int nLen);		//����󷵻�

	void requestGetSharePrize(const std::string& strCode);			//����ͨ����������ȡ����
	void onGetSharePrizeResp(const PacketAide& packet, int nLen);	//ͨ����������ȡ��������

	void requestShareInfo(int nOpType);								//������������� //1-��ѯ���� 2-��ȡ���� 3-��ѯ���а�
	void onShareInfoResp(const PacketAide& packet, int nLen);		//��������������
	void onShareRankResp(const PacketAide& packet, int nLen);		//����������з���

	void requestShareDays();										//�����������
	void requestMySharePrize();										//������ȡ���������Ľ���
	void onShareDaysResp(const PacketAide& packet, int nLen);		//���ط�������

	int getShareDayLevel(int nShareDay);
protected:
	CC_SYNTHESIZE(int, m_nGetCoinNum, GetCoinNum);					//������ȡ�Ľ��
	CC_SYNTHESIZE(int, m_nGetGoldNum, GoldNum);						//������ȡ��Ԫ��

	CC_SYNTHESIZE(int, m_nShareDay, ShareDay);						//���������
	CC_SYNTHESIZE(int, m_nGetIndex, GetIndex);						//��ȡ�ı���index��0-4
};

#endif // _LogicPromote_H_

