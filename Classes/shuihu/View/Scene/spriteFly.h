#ifndef __spriteFly_H__
#define __spriteFly_H__

#include "cocos2d.h"
/**  ʹ�÷���
*   1.����һ��spriteFly����
*   2.���ö���add����ǰlayer   
*   3.����setPos��������Ԫ���ɳ�����ʼ����ֹλ�ã��������1��������ҵ���ʼλ�ã��������2��Ԫ�����е���ֹλ�ã�
*   4.���ñ����е�startAni�������ɿ�ʼ���Ŷ������������1�����Ž��Ч�����������2������Ԫ��Ч����
*   5.������üӷ�Ч����ֻ���ö������addscore�������������1���ӷ�Ч����label���������2����ǰ�û����ܷ������������3����һ�ȡ�ķ�����
*/


class spriteFly : public cocos2d::Node
{

protected:

	//����Ԫ��
	void createYuanbao();

	//����Ԫ����
	void createYuanbaoFly(Node* sp, int m);

	//����Ԫ���Ŵ�Ч��
	void yuanBaoChange(Node* obj);

	//�������
	void createCoin();
	//�����Ч
	void createCoinFly(Node* sp, int m);

	//�Ƴ����
	void removeSpriteFly(Node* obj);

	//��ҷŴ�Ч��
	void coinChange(Node* obj);	

	//�ӷ�Ч��
	void scoreTimer(float t);	

public:
	spriteFly();
	~spriteFly();
	static spriteFly* create();
	virtual bool init();
	//��ʼ���Ŷ���
	void startAni(unsigned int n);
	//�ӷ�Ч���������
	void addscore(cocos2d::LabelAtlas* label, int totaleNum,int num);
	//���ý�һ�Ԫ���ɳ�����ʼ�ͽ���λ��
	void setPos(cocos2d::Vec2 pos1,cocos2d::Vec2 pos2);

	//�ɳ�λ��
	cocos2d::Vec2                 beginPos;
	//����λ��
	cocos2d::Vec2                 endPos;

protected:
	//�����Ĳ�������
	unsigned int		         type;
	//��ǰ����
	int                          score;
	//��ȡ�ķ���
	int                          getScore;
	//�����仯����
	int                           disScore;
	//��¼�����ı�ǩ       
	cocos2d::LabelAtlas*          scoreLabel;
	
    
};

#endif // __spriteFly_H__
