#ifndef __ResOneJet_H__
#define __ResOneJet_H__

#include "slwh/Base/UIBase.h"

class ResOneJet :
	public UIBase
{
protected:
	TextAtlas*			m_beilv;
	TextAtlas*			m_jet;
	ImageView*			m_light;
	ImageView*			m_bk;
public:
	ResOneJet();
	~ResOneJet();

	void SetBeiLv(int t);
	void SetJet(int t);
	void OpenLight(bool b);

protected:
	void Start();

};

#endif

