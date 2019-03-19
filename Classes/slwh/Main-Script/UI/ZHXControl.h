#ifndef __ZHXControl_H__
#define __ZHXControl_H__

#include "slwh/Base/UIBase.h"

class ZHXControl :
	public UIBase
{
protected:
	//ׯ���еĿ���
	enum enumZHXState
	{
		ZHXNormal = 0x00000000,										//��ֹ״̬
		ZHXSpeedAdd = 0x00000003,                                    //���ٵ�ʱ��
		ZHXSpeedMax = 0x00000004,                                    //����ٶ�ʱ��
		ZHXSpeedSub = 0x00000005,                                    //���ٵ�ʱ��
		ZHXSpeedMin = 0x0FFFFFFF,                                    //С���ٶ�����ʱ�� ֱ��ȷ�������ֹͣ
	};
	//ׯ���п���״̬����
	enumZHXState m_enumState = ZHXNormal;

	//�����������ǳ�ʼ״̬
	ImageView*				m_zhxUp;
	ImageView*				m_zhxDown;


	//��¼��ʼλ��
	Vec2					m_downPt;
	Vec2					m_upPt;
	//��������
	float PixelH;										//�ƶ��ľ���
	enumGameZXH				m_nowZHX;					//��ǰ��ʾ��ZHX
	static const float		Speed_Add;					//���ٶ�
	static const float		Speed_Sub;					//���ٶ�
	static const float		Speed_MinSub;				//С���ٶ�
	float					m_speed;					//��ǰ�ٶ�
	float					m_time;						//��ǰʱ��


	//�������
	enumGameZXH m_rsZHX = InvalidZHX;


	//�Ƿ�ת��
	bool m_bIsRunning = false;
	std::map<enumGameZXH, enumGameZXH>	m_ZHXMap;
	std::map<enumGameZXH, std::string>	m_ZHXTex;
public:
	ZHXControl();
	~ZHXControl();

	//��ʼת���ӿ�
	void StartPlay(enumGameZXH e);
	void StartPlay(int index);
protected:
	void Start();
	void update(float t);

	//����һ�ζ���
	void RoundOne(float t);
	//��ʼ��һ����Ϸ
	void OnInit();

};

#endif

