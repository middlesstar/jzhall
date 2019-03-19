#ifndef _PlayerListOne_H_
#define _PlayerListOne_H_

#include "shark/resourceDef.h"
#include "shark/Base/ViewBase.h"
/*
	����б�Ԫ
*/

#define _Gold3_Fly		"slwh/animation/gold3/gold3.png"

class PlayerListOne : 
	public Widget
{
protected:
	ImageView*			m_rootNode;								//���ڵ�(ImageView��������ListViewռλ)
	ImageView*			m_head;									//ͷ��
	ImageView*			m_headBk;								//ͷ��߿�
	ImageView*			m_vip0;									//vip0
	ImageView*			m_vip;									//vip��ɫ
	TextAtlas*			m_vipNum;								//vip����
	Text*				m_name;									//�ǳ�
	ImageView*			m_winBk;								//Ӯ������ı�����
	Text*				m_winNum;								//Ӯ������ı�����λK��
	TextAtlas*			m_scoreNum;								//��������

	
	static PlayerListOne*	m_templateThis;							//ģ��

	UserInfo_Shark			m_userInfo;								//�����Ϣ

	static Vec2		    	m_targetPos[12];							//��ҷ���15��Ŀ��λ��

	
protected:
	PlayerListOne();
	
	void setWinScore(SCORE score, int type);					//type = 0 ѹ�ֽ׶� type = 1��Ӯ����
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//ͷ�񱳾�����VIP���
	
	//�ص�����
	void CallBack(Node* node);

public:
	~PlayerListOne();
	UserInfo_Shark getuserinfo()const { return m_userInfo; }
	void setuserinfo(const UserInfo_Shark & info);
	
	ImageView* getRootNdoe(){ return m_rootNode; }
	void GoldFly(int tag);                                      //�ɽ��Ч��
	static void initTemplate(ImageView* node);					//��ʼ��ģ����ܴ������(ģ����cocos studio��ƴ��)
	static PlayerListOne* create();
};


#endif 
