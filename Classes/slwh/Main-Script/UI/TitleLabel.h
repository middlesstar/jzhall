#ifndef __TitleLabel_H__
#define __TitleLabel_H__

#include "slwh/Base/UIBase.h"

/*
些许位置需要改变标题
采用单例模式
*/


class TitleLabel :
	public UIBase
{
public:
	enum enumTitleType
	{
		Invalid = -1,
		Normal1,								//点击屏幕押注	
		D3Y,									//大三元
		D4X,									//大四喜
		_4lian,									//4连发
		DMG,									//大满贯
		CJ,										//彩金
		XYCJ,									//金色来袭 幸运抽奖

		TitleMax
	};

	static const float		UPDATE_TIME;		//定时器时间
	enumTitleType			m_nowTyoe;
protected:
	//当前帧
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

