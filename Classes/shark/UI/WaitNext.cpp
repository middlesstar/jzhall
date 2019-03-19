#include "WaitNext.h"


#define WAIT_NEXT_JET	"wait_next_jet"
#define WAIT_TEXT_IMG	"wait_text_img"
#define WAIT_NOTICE		"wait_notice"


#define _WAIT_TEXT_ROOT "images/common/waiting/%d.png"

WaitNext::WaitNext() :
m_timeCount(nullptr),
m_waitnextgame(nullptr),
m_okbt(nullptr),
m_waitnode(nullptr)
{
}

WaitNext::~WaitNext()
{
}

void WaitNext::setContent()
{
	m_timeCount = dynamic_cast<TextAtlas*>(m_pNode->getChildByName(WAIT_NEXT_JET));
	CC_ASSERT(m_timeCount != nullptr);
	m_timeCount->setString("0");

	m_waitnextgame = dynamic_cast<ImageView*>(m_pNode->getChildByName(WAIT_TEXT_IMG));
	CC_ASSERT(m_waitnextgame != nullptr);

	m_waitnode = dynamic_cast<Node*>(m_pNode->getChildByName(WAIT_NOTICE));
	CC_ASSERT(m_waitnode != nullptr);

	m_okbt = dynamic_cast<Button*>(m_waitnode->getChildByName("ok"));
	CC_ASSERT(m_okbt != nullptr);
	m_okbt->addTouchEventListener(this, toucheventselector(WaitNext::BtEvent));
	
	m_waitnode->setScale(_ZERO);
}

void WaitNext::BtEvent(Ref* pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		waitnodeOutFunc();
	}
}

void WaitNext::StartPlay(int time)
{
	if (m_pNode == nullptr) return;
 
	m_nowTime = time;

	std::stringstream str;
	str << time;
	
	schedule(schedule_selector(WaitNext::updatetime), 1.f);
	schedule(schedule_selector(WaitNext::updateText), 0.08f);

	//大于5秒才有提示框
	if (time >= 5 )
	{
		m_waitnode->runAction(CCEaseBackOut::create(ScaleTo::create(0.2f, 1.f)));
		scheduleOnce(schedule_selector(WaitNext::updateNoticeOut), 3.f);
	}	
}

void WaitNext::updateText(float t)
{
	static int index = _ZERO;
	if (++index > 12) index = _ZERO;
	
	char buf[256];
	sprintf(buf, _WAIT_TEXT_ROOT,index);
	m_waitnextgame->loadTexture(buf, TextureResType::PLIST);
}

void WaitNext::updatetime(float t)
{
	if (--m_nowTime < 1)
	{
		m_nowTime = _ZERO;
		unschedule(schedule_selector(WaitNext::updatetime));
		unschedule(schedule_selector(WaitNext::updateText));
		unschedule(schedule_selector(WaitNext::updateNoticeOut));
		destory();
		return;
	}

	std::stringstream str;
	str << m_nowTime;
	m_timeCount->setString(str.str());
}

void WaitNext::updateNoticeOut(float t)
{
	waitnodeOutFunc();
}

void WaitNext::destory()
{
	if (m_pNode == nullptr) return;


	//从根节点移除（不再使用）
	m_pNode->removeFromParent();
	m_pNode = nullptr;
	m_timeCount = nullptr;
	m_waitnextgame = nullptr;
	m_okbt = nullptr;
	m_waitnode = nullptr;
}

void WaitNext::waitnodeOutFunc()
{
	m_waitnode->runAction(CCEaseBackIn::create(ScaleTo::create(0.2f,0.f)));
}

