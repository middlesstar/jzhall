#ifndef  _CycleScroll_H_
#define  _CycleScroll_H_

#include "cocos2d.h"
#include "CycleUnit.h"

USING_NS_CC;

#define CYCLE_ITEM_NUM		3			//实际转动节点个数

class CycleUnitSelectDelegate
{
public:
	virtual void onUnitSelect(const sUnitInfo* pSourceData) {};
	virtual void onUnitMoveBegin() {};
	virtual void onUnitMoveEnd() {};
};

class  CycleScroll : public Layer
{
	enum eMoveDirection
	{
		EMD_LEFT	=	0,
		EMD_RIGHT	=	1
	};
public:
	CycleScroll();
	virtual ~CycleScroll();

	static CycleScroll* create(const std::vector<sUnitInfo>& sourceData);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;

public:
	void jumpLeftOneStep();						//向左移动一步
	void jumpRightOneStep();					//向右移动一步

protected:
	bool _initWithSourceData(const std::vector<sUnitInfo>& sourceData);
	void _initTouchListener();
	int _getNextIndex(int nIndex,int nSize);
	int _getPreviousIndex(int nIndex, int nSize);
	
	void _moveOneStep(eMoveDirection eDir);

	Action* _createJumpAction(float fScale,const Vec2& desPos,const Color3B& desColor,float dt);
	Action* _createJumpEndAction();

	void _unitClick(Ref* pRef);
protected:
	Vec2 m_posBeginTouchPos;
	CycleUnit* m_pUnit[CYCLE_ITEM_NUM];
	int m_nShowIndex;							//当前显示在最前面的节点的index
	bool m_bCanMove;

	std::vector<sUnitInfo> m_vecSourceData;
	CC_SYNTHESIZE(CycleUnitSelectDelegate*, m_pSelectDelegate, SelectDelegate);
};

#endif // _CycleScroll_H_

