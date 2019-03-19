#include "CycleScroll.h"

#define ShowScale			1.1f
#define NormalScale			0.8f
#define touchMoveDistance	8.0f

const Color3B ShowColor = Color3B(255, 255, 255);
const Color3B NormalColor = Color3B(150, 150, 150);
const float gInterval = 0.2f;

static int gLastShowIndex = 0;

CycleScroll::CycleScroll()
	: m_nShowIndex(0)
	, m_pSelectDelegate(nullptr)
	, m_bCanMove(true)
{
	memset(m_pUnit, 0, sizeof(m_pUnit));
}

CycleScroll::~CycleScroll()
{

}

CycleScroll* CycleScroll::create(const std::vector<sUnitInfo>& sourceData)
{
	CycleScroll* pScroll = new CycleScroll;

	if (pScroll && pScroll->_initWithSourceData(sourceData))
	{
		pScroll->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pScroll);
	}

	return pScroll;
}

bool CycleScroll::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_posBeginTouchPos = touch->getLocation();
	Vec2 srcPos = Vec2(-_contentSize.width / 2, -_contentSize.height / 2);
	srcPos = getParent()->convertToWorldSpace(srcPos);
	Rect rect = Rect(srcPos, _contentSize);

	m_bCanMove = rect.containsPoint(m_posBeginTouchPos);
	return rect.containsPoint(m_posBeginTouchPos);
}

void CycleScroll::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (m_bCanMove)
	{
		float xOffset = touch->getLocation().x - m_posBeginTouchPos.x;

		if (xOffset > touchMoveDistance || xOffset < -touchMoveDistance)
		{
			m_bCanMove = false;
			xOffset > touchMoveDistance ? jumpRightOneStep() : jumpLeftOneStep();
		}
	}
}

void CycleScroll::jumpLeftOneStep()
{
	_moveOneStep(EMD_LEFT);
}

void CycleScroll::jumpRightOneStep()
{
	_moveOneStep(EMD_RIGHT);
}

bool CycleScroll::_initWithSourceData(const std::vector<sUnitInfo>& sourceData)
{
	m_vecSourceData = sourceData;
	int nSize = (int)sourceData.size();
	if (nSize < 3)
	{
		return false;
	}

	m_nShowIndex = 0;

	m_pUnit[0] = CycleUnit::create(sourceData.at(gLastShowIndex));
	m_pUnit[0]->setScale(ShowScale);
	m_pUnit[0]->setColor(ShowColor);
	this->addChild(m_pUnit[0], 0, gLastShowIndex);

	int nLeftIndex = _getNextIndex(gLastShowIndex, nSize);
	m_pUnit[1] = CycleUnit::create(sourceData.at(nLeftIndex));
	m_pUnit[1]->setScale(NormalScale);
	m_pUnit[1]->setColor(NormalColor);
	m_pUnit[1]->setPositionX(-m_pUnit[1]->getContentSize().width);
	this->addChild(m_pUnit[1], 0, nLeftIndex);

	int nRightIndex = _getPreviousIndex(gLastShowIndex, nSize);
	m_pUnit[2] = CycleUnit::create(sourceData.at(nRightIndex));
	m_pUnit[2]->setScale(NormalScale);
	m_pUnit[2]->setColor(NormalColor);
	m_pUnit[2]->setPositionX(m_pUnit[2]->getContentSize().width);
	this->addChild(m_pUnit[2], 0, nRightIndex);

	m_pUnit[m_nShowIndex]->playEffect();

 	this->setContentSize(Size(Director::getInstance()->getWinSize().width, m_pUnit[0]->getContentSize().height * 1.4f));

// 	LayerColor* ply = LayerColor::create();
// 	ply->setContentSize(this->getContentSize());
// 	ply->setColor(ccc3(255, 0, 0));
// 	ply->setOpacity(255);
// 	ply->setAnchorPoint(Vec2(0.5, 0.5));
// 	ply->setPosition(Vec2(-getContentSize().width / 2, -getContentSize().height / 2));
// 	this->addChild(ply,-1);

	_initTouchListener();

	for (int i = 0; i < CYCLE_ITEM_NUM; i++)
	{
		if (m_pUnit[i])
		{
			m_pUnit[i]->setTouchEnabled(true);
			m_pUnit[i]->setSwallowTouches(false);
			m_pUnit[i]->addClickEventListener(CC_CALLBACK_1(CycleScroll::_unitClick, this));
		}
	}

	return true;
}

void CycleScroll::_initTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(CycleScroll::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CycleScroll::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

int CycleScroll::_getNextIndex(int nIndex, int nSize)
{
	return (nIndex + 1) % nSize;
}

int CycleScroll::_getPreviousIndex(int nIndex, int nSize)
{
	return (nIndex - 1 + nSize) % nSize;
}

Action* CycleScroll::_createJumpAction(float fScale, const Vec2& desPos, const Color3B& desColor, float dt)
{
	return Spawn::create(ScaleTo::create(dt, fScale),MoveTo::create(dt, desPos),TintTo::create(dt, desColor),NULL);
}

Action* CycleScroll::_createJumpEndAction()
{
	return Sequence::create(ScaleBy::create(0.05f, 0.9f), ScaleBy::create(0.08f, 1.1f), ScaleBy::create(0.05f, 1.0f), NULL);
}

void CycleScroll::_moveOneStep(eMoveDirection eDir)
{
	int nSize = (int)m_vecSourceData.size();
	if (nSize < CYCLE_ITEM_NUM || m_nShowIndex < 0 || m_nShowIndex >= CYCLE_ITEM_NUM || !m_pUnit[0])
	{
		return;
	}

	if (m_pUnit[0]->numberOfRunningActions() > 0)
	{
		return;
	}
	m_pUnit[m_nShowIndex]->stopEffect();

	int nLeftIndex = _getNextIndex(m_nShowIndex, CYCLE_ITEM_NUM);
	int nRightIndex = _getPreviousIndex(m_nShowIndex, CYCLE_ITEM_NUM);

	CycleUnit* pMoveToMid = eDir == EMD_LEFT ? m_pUnit[nRightIndex] : m_pUnit[nLeftIndex];
	CycleUnit* pMoveToAnotherSide = eDir == EMD_LEFT ? m_pUnit[nLeftIndex] : m_pUnit[nRightIndex];

	pMoveToMid->runAction(_createJumpAction(ShowScale, m_pUnit[m_nShowIndex]->getPosition(), ShowColor, gInterval));		//要移动到中间的节点动画
	pMoveToMid->runAction(Sequence::create(DelayTime::create(gInterval), 
		CallFuncN::create([this, eDir, nRightIndex, nLeftIndex](Node* pNode)
	{
		m_nShowIndex = eDir == EMD_LEFT ? nRightIndex : nLeftIndex;

		CycleUnit* pUnit = dynamic_cast<CycleUnit*>(pNode);
		if (pUnit)
		{
			pUnit->runAction(_createJumpEndAction());
			pUnit->playEffect(true);
		}
	}), 
		NULL));

	m_pUnit[m_nShowIndex]->runAction(_createJumpAction(NormalScale, pMoveToAnotherSide->getPosition(), NormalColor, gInterval));	//中间节点的动画

	//要移动到另外一边的节点动画
	Sequence* pChangeAction = Sequence::create(
		ScaleTo::create(gInterval / 2, 0),
		Place::create(pMoveToMid->getPosition()),
		ScaleTo::create(gInterval / 2, NormalScale),
		CallFuncN::create([this, eDir , pMoveToMid](Node* pNode)
		{
			CycleUnit* pUnit = dynamic_cast<CycleUnit*>(pNode);
			if (pUnit)
			{
				std::function<int(int, int)> func = eDir == EMD_LEFT ? CC_CALLBACK_2(CycleScroll::_getPreviousIndex, this) : CC_CALLBACK_2(CycleScroll::_getNextIndex, this);
				int nIndex = func(pMoveToMid->getTag(), m_vecSourceData.size());
				pUnit->load(m_vecSourceData.at(nIndex));
				pUnit->setTag(nIndex);
			}

			if (m_pSelectDelegate)
			{
				m_pSelectDelegate->onUnitMoveEnd();
			}
		}),
		NULL);
	pMoveToAnotherSide->runAction(pChangeAction);

	if (m_pSelectDelegate)
	{
		m_pSelectDelegate->onUnitMoveBegin();
	}
}

void CycleScroll::_unitClick(Ref* pRef)
{
	if (m_pUnit[0] && m_pUnit[0]->numberOfRunningActions() > 0)
	{
		return;
	}

	CycleUnit* pUnit = dynamic_cast<CycleUnit*>(pRef);

	if (pUnit)
	{
		int nTag = pUnit->getTag(); 
		int nMidTag = m_pUnit[m_nShowIndex]->getTag();
		if (nMidTag == nTag)	//选中的是正中间的
		{
			gLastShowIndex = nTag;
			if (m_pSelectDelegate)
			{
				m_pSelectDelegate->onUnitSelect((const sUnitInfo*)pUnit->getUserData());
			}
		}
		else
		{
			nTag == _getNextIndex(nMidTag, m_vecSourceData.size()) ? jumpRightOneStep() : jumpLeftOneStep();
		}
	}
}