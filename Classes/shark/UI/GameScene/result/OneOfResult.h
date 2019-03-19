#ifndef _OneOfResult_H_
#define _OneOfResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class OneOfResult : public ViewBase
{
public:
	OneOfResult();
	~OneOfResult();

	CREATE_FUNC(OneOfResult);

	//��ť��Ӧ�¼�
	void BtEvent(Ref *pSender, TouchEventType type);
	
protected:
	void setContent();
public:
	//���ñ���
	void SetBeiLv(int g);

	//��ʾ��Ѻ��
	void showStake(int score);

	//��ʾӮ��
	void showWin(int score);

	//��ʾ��ɫ��Ȧ
	void showColor();

	//��Ȧ��ת
	void runCircle();

	//ֹͣ���еĶ���
	void stopAll();

	//���ñ��ֿ����İ�ť
	void showWinBtn(bool flag,int score);
   //��ʾ������ͼƬ
	void showNormal();

	//���Ź�Ȧ��С�����Ч
	void playCircleSong();

protected:
	TextAtlas*			m_beilvNum;											//������������
	TextAtlas*			m_mystakeNum;											//�ҵ�Ѻע��������
	Text*               winNum;                                             //Ӯ�ķ���
	Sprite*             blackLabel;                                        //��ɫ����
	Sprite*             whiteLabel;                                        //��ɫ����
	ImageView*          blinkCircle;                                       //��Ȧ

	Button*             btn;
};

#endif 