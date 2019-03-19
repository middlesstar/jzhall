#ifndef  _DragLayer_H_
#define  _DragLayer_H_

#include "cocos2d.h"
USING_NS_CC;

class DragLayer : public Layer
{
protected:
	DragLayer();

public:
	~DragLayer();
	CREATE_FUNC(DragLayer);

	virtual bool init() override;

	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;

	virtual void setContentSize(const Size& contentSize) override;
public:
	void openDebug();

protected:
	void _setTouchListener();

protected:
	Vec2 m_posOffset;		//开始接触点和锚点的偏移位置

	float m_fXMax;
	float m_fXMin;
	float m_fYMax;
	float m_fYMin;
};


#endif // _DragLayer_H_

