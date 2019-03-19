#pragma once


#include "slwh/Base/UIBase.h"

class XYCJRes :
	public UIBase
{
protected:
	ImageView*			m_res[3];
	ImageView*			m_yuanbaoImg;
	TextAtlas*			m_yaunbao;
	ImageView*			m_cpImg;
	TextAtlas*			m_cpNum;

public:
	void setRes(int aType, int* a ,int yuanbao = 0,int cp = 0);
	
	XYCJRes();
	~XYCJRes();
protected:
	void Start();
	//设置元宝数字UI
	void setYBui(int g);
	//设置船票数字UI
	void setCPui(int g);
};




