#ifndef  _LogicFreeMoney_H_
#define  _LogicFreeMoney_H_

#include "Window/LogicBase.h"

class  LogicFreeMoney : public LogicBase
{
public:
	LogicFreeMoney();
	virtual ~LogicFreeMoney();

	SINGLE_INSTANCE_FUNC(LogicFreeMoney);

protected:
	void _registerMessageHandler();

public:
	void requestDialyPrize();										//����ÿ�ճ齱
	void onDialyPrizeResp(const PacketAide& packet, int nLen);		//ÿ�ճ齱����

	void requestDole();												//������ȡ�ȼý�
	void onDoleResp(const PacketAide& packet, int nLen);			//��ȡ�ȼý𷵻�

	void requestDailySin(int type);									//����ÿ��ǩ��
	void onDailySinResp(const PacketAide& packet, int nLen);		//��ȡÿ��ǩ����ҷ���

	void requestWechatPresent(const std::string& strExCode);		//����΢����Ʒ�һ�
	void onWechatPresentResp(const PacketAide& packet, int nLen);	//����΢����Ʒ�һ�

	void requestComment();											//�����������
	void onCommentResp(const PacketAide& packet, int nLen);			//���غ�������

protected:
	CC_SYNTHESIZE(CMD_GP_DailyLottery_Get, m_objDailyPrize, DailyPrize);
	CC_SYNTHESIZE(CMD_ST_SignDay_rsp, m_objSignDay, SignDay);
};

#endif // _LogicFreeMoney_H_

