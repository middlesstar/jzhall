#ifndef  _LogicActivity_H_
#define  _LogicActivity_H_

#include "Window/LogicBase.h"
class LogicActivity : public LogicBase
{
public:
	LogicActivity();
	virtual ~LogicActivity();

	SINGLE_INSTANCE_FUNC(LogicActivity);

	//�������������
	void requestGrabRed();
	//���������ͬ������
	void onGrabRed(const PacketAide& packet, int nLen);
	//�������������
	void sendGrabRedData(int score, int yuanbao);
protected:
	void _registerMessageHandler();

public:
	CMD_ST_FuDaiRsp	m_qfd;			//����������

};



#endif // _LogicActivity_H_

