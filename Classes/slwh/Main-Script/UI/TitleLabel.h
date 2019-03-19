#ifndef __TitleLabel_H__
#define __TitleLabel_H__

#include "slwh/Base/UIBase.h"

/*
Щ��λ����Ҫ�ı����
���õ���ģʽ
*/


class TitleLabel :
	public UIBase
{
public:
	enum enumTitleType
	{
		Invalid = -1,
		Normal1,								//�����ĻѺע	
		D3Y,									//����Ԫ
		D4X,									//����ϲ
		_4lian,									//4����
		DMG,									//������
		CJ,										//�ʽ�
		XYCJ,									//��ɫ��Ϯ ���˳齱

		TitleMax
	};

	static const float		UPDATE_TIME;		//��ʱ��ʱ��
	enumTitleType			m_nowTyoe;
protected:
	//��ǰ֡
	int m_index;
	static TitleLabel*		m_instance;

	ImageView*				m_titleBg;
	ImageView*				m_title;
protected:
	TitleLabel();
	void Start();
	void StartOpen();
	void PlayStarEff();
public:
	~TitleLabel();


	void relese();
	static TitleLabel* GetInstance();
	static void SetTitle(enumTitleType t);
};

#endif

