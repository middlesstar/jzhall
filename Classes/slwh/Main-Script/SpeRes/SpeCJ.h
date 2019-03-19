#ifndef __SpeCJ_H__
#define __SpeCJ_H__

#include "slwh/Base/UIBase.h"
class SpeCJ :
	public UIBase
{
protected:
	ImageView*			m_light;
	Text*				m_name[3];
	TextAtlas*			m_num[3];
	ImageView*			m_gold[3];
	Text*				m_cjlxText;


	bool				m_bIsOpened;

public:
	SpeCJ();
	~SpeCJ();

	void StartPlay(int ncj,std::string name[3], longlong num[3]);
	void StopPlay();

protected:
	void Start();

	void CloseCallBack();
};


#endif


