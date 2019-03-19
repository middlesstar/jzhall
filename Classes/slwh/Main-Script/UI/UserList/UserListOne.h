#pragma once

#include "slwh/ResourcePath.h"
#include "slwh/Base/UIBase.h"
/*
	����б�Ԫ
*/

#define _Gold3_Fly		"images/common/animation/gold3/gold3.png"

class UserListOne : 
	public UIBase
{
protected:
	ImageView*			m_rootNode;								//���ڵ�(ImageVirw��������ListViewռλ)
	ImageView*			m_head;									//ͷ��
	ImageView*			m_headBk;								//ͷ��߿�
	ImageView*			m_vip0;									//vip0
	ImageView*			m_vip;									//vip��ɫ
	TextAtlas*			m_vipNum;								//vip����
	Text*				m_name;									//�ǳ�
	ImageView*			m_winBk;								//Ӯ������ı�����
	Text*				m_winNum;								//Ӯ������ı�����λK��
	TextAtlas*			m_scoreNum;								//��������
	
	static UserListOne*	m_templateThis;							//ģ��

	slwhPlayerInfo		m_userInfo;								//�����Ϣ

	static Vec2			m_sTarPos[15];							//��ҷ���15��Ŀ��λ��

protected:
	UserListOne();
	
	void setWinScore(SCORE score, int type);					//type = 0 ѹ�ֽ׶� type = 1��Ӯ����
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//ͷ�񱳾�����VIP���

public:
	~UserListOne();
	slwhPlayerInfo getuserinfo()const { return m_userInfo; }
	void setuserinfo(const slwhPlayerInfo & info);
	
	ImageView* getRootNdoe(){ return m_rootNode; }
	void GoldFlyStartPlay(int tag);
	static void initTemplate(ImageView* node);					//��ʼ��ģ����ܴ������(ģ����cocos studio��ƴ��)
	static UserListOne* create();
};



