#ifndef _Battle_H_
#define _Battle_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class Battle : public ViewBase
{
public:    

	Battle();
	virtual ~Battle();

	CREATE_FUNC(Battle);
	virtual void setContent();				                //����UI����

	//������ת
	void startPlay();
	//�ر���ת
	void stopPlay();
	//��ת��Ȧ�ӿ�
	void rotation();
	//���ù�Ȧ��ʼ��ֹͣ��λ��
	void setCirclePos(int beginPos, int endPos);
	//���ƹ�Ȧ��תֹͣ��λ��
	void setRotationPos(int index);
protected:

	//��ֵ���䶯��
	void moveRecharge();
	
	//���һ����ʾ�̻���ը��Ч��
	void lightBoast(int index);
	//������Ȧ
	void lightCircle(float t);
	//���ƹ�Ȧ������
	void controlCircle(int repeat);
	//�ƶ���Ȧ
	void moveCircle(float t);
	//���ٹ�Ȧ
	void reduceCircle(float t);
	//�ټ��ٹ�Ȧ
	void stopCircle(float t);
	//����ֹͣ��Ȧ
	void meanCircle(float t);
	/*------------�н�-----------------------*/
	//����
	void back(float);

	//��ֵ��ť
	void Recharge(Ref *pSender, TouchEventType type);

public:
	//����������Ҫ������Ԫ��
	void changeImg(int index);
	//��ԭ������ͼƬ
	void reChangeImg();
	//���òʽ����ֵ
	void setColorMoney(int money);
private:

	//�洢��ת�Ĺ�Ȧ
	cocos2d::Vector<Sprite* > circle;
	//��¼������Ȧ�ĸ���
	int  num;
	//���Ƶ�����Ȧ�ĸ���
	int limitNum;
	//��¼��ǰ��Ȧ��λ��
	int pos;
	//���ƹ�Ȧ�ܵ�Ȧ��
	int moveNum;
	//���ƹ�Ȧ���ٵ��ٶ�
	float limitSpeed;
	//��¼��Ȧ�����ƶ��ĸ�����
	int closeNum;
	//����ֹͣ��λ��
	int stopPos;
	//��Ȧ�ƶ�һȦ�ı�־λ
	int circleFlag;
	//��Ȧ��תֹͣ��λ��
	int rotationPos;

	//ȷ���õ���һ���������
	int lvIndex;
protected:
	Node*        pMainNode;

	ImageView*      topColor_shark;
	ImageView*      downColor_shark;

	ImageView*     leftTop_shark;
	ImageView*     rightTop_shark;
	ImageView*     rightDown_shark;
	ImageView*     leftDown_shark;
	ImageView*     leftMiddle_shark;
	ImageView*     rightMiddle_shark;

	Sprite*      screen;
	TextAtlas*   colorMoneyNum;

	Button*       btn_recharge;
	ImageView*    rechargeLight;

};

#endif // _Battle_H_