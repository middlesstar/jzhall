#ifndef  _ConcludeNotice_H_
#define  _ConcludeNotice_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Window/WindowBase.h"
USING_NS_CC;
using namespace ui;


class ConcludeNotice :
	public WindowBase
{
public:
	ConcludeNotice();
	virtual ~ConcludeNotice();

	CREATE_FUNC(ConcludeNotice);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;
	virtual void updateWindow(int nCode) override;	        //刷新窗口
protected:
	int				m_yuanbao;
	int				m_score;

	TextAtlas*		m_yuanbaoNum;
	TextAtlas*		m_scoreNum;
	Text*			m_text;
protected:
	void SetYunBaoNum(int g);
	void SetScoreNum(int g);
	void SetText(const std::string &str);
};

#endif // _ConcludeNotice_H_

