#ifndef  _ColorResult_H_
#define  _ColorResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"


class  ColorResult : public ViewBase
{
public:
	ColorResult();
	virtual ~ColorResult();
	CREATE_FUNC(ColorResult);

	virtual void setContent();

	//开启闪烁
	void startPlay();
	//关闭闪烁
	void stopPlay();

	
public:
	//判断开奖类型
	void judgeResult(int index);
	//开启灯光旋转
	void rotateLight();

	//显示中奖的类型
	void showType(Node* sp);

	//停止背景光圈的旋转
	void stopRotate();
	
	//粒子特效显示彩金来袭
	void cardLight();

public:
	//打枪效果
	void startShoot();
	//开启彩色打枪效果
	void startColor();
	
protected:
	int    count;  //记录创建的光效的个数
	std::vector<Sprite*>   leftLight;
	
protected:	
	Node*        pMainNode;
	Sprite*      light;
	Sprite*      blinkLight;
	Sprite*      yuanbao;
	Sprite*      tickets;
	Node*      bg_caijin;
	Sprite*      caijin_box;
	Sprite*      caijinCome;

	bool         lightFlag;

protected:
	ImageView*			m_light;
	Text*				m_name[3];
	TextAtlas*			m_num[3];
	ImageView*			m_gold[3];
	Text*				m_cjlxText;

	//cocostudio::timeline::ActionTimeline* uiAction;


	bool				m_bIsOpened;

public:
	void ListShow();
	void ListHide();
protected:
	void CloseCallBack();
};

#endif // _ColorResult_H_

