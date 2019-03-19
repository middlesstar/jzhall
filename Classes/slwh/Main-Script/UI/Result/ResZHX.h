#ifndef __ResZHX_H__
#define __ResZHX_H__

#include "slwh/Base/UIBase.h"

class ResZHX :
	public UIBase
{
protected:
	ImageView*			m_zhx;
	TextAtlas*			m_beilv;
public:
	ResZHX();
	~ResZHX();

	void SetZHX(enumGameZXH e);

protected:
	void Start();

};

#endif



