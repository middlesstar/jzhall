#ifndef __ResRecord_H__
#define __ResRecord_H__

#include "slwh/Base/UIBase.h"

#define RECORD_BOARD_MAX	8

class ResRecord :
	public UIBase
{
protected:
	std::map<int, std::string> m_map;

	struct Record
	{
		ImageView* animal = nullptr;
		ImageView* zhx = nullptr;
	};

	Record m_record[RECORD_BOARD_MAX];				//���鵱��ѭ������
	int m_listRear;									//ѭ������β

public:
	ResRecord();
	~ResRecord();

protected:
	void Start();

	void SetZHXTex(int index, enumGameZXH type);

public:
	//��ʼ����¼��Ϣ�ӿ�  ��λ��Ϊׯ�к�  
	void InitResList(int* a);

	//��ӵ��ڵ�
	void Add(enumSpecialRs e1, enumAnimalColor e2, enumAnimalType e3, enumGameZXH e4);
	void Add(AnimalResult e);

	void setVisible(bool b);
	//��������
protected:
	const int List_AC_MAX = 50;						//����֡
	int m_listAcIndex;								//��ǰ����֡
	AnimalResult m_animalRes;						//���������ʱ����洢����
	Vec2 m_ptEnd;									//�������
	float offsetY;									//Y�����ϵļ��
	bool m_bIsVisible;								//�Ƿ���ʾ
protected:
	void update(float t);

};

#endif


