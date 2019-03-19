#include "RoomGameNotice_SHZ.h"

const int iVipColorTable_SHZ[][3] =
{
	{ 255, 255, 255 },
	{  26, 231, 255 },
	{   0, 255,  66 },
	{ 255, 255,   0 },
	{ 255, 162,   0 },
	{ 255, 48,    0 },
	{ 252,  0,  255 }
};

SHZCRoomGameNotice::SHZCRoomGameNotice(void)
	: mGameNoticeMission(STATIC_DATA_STRING("address"), SERVER_PORT_SHZ)
{
	m_isAnnouncing = false;
	m_noticeArras  = CCArray::create();
	mGameNoticeMission.setMissionSink(this);
}


SHZCRoomGameNotice::~SHZCRoomGameNotice(void)
{
}

bool SHZCRoomGameNotice::init()
{
	do 
	{
		m_bg=CCSprite::createWithSpriteFrameName("GameNoticeView.png");
		m_bg->setPosition(Vec2(kRevolutionWidth_SHZ/2,kRevolutionHeight_SHZ/2+230));
		m_bg->setVisible(false);
		addChild(m_bg);

		m_TailorView = SHZTailorView::create(CCSize(900,30));
		m_TailorView->setPosition(Vec2(75, 0));
		m_bg->addChild(m_TailorView);

		m_bg->setScaleY(1.2f);

		m_iCount=0;

		if (DF::shared()->GetGameType() == 0)
			mGameNoticeMission.StartGameNotice();

		return true;

	} while (0);


	return true;
}

void SHZCRoomGameNotice::onGPGameNoticeResult(const char* szDescription)
{
	CCLabelTTF *m_lbMsg=CCLabelTTF::create("34343443", "Arial", 25, CCSizeMake(800, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	m_lbMsg->setPosition(Vec2(kRevolutionWidth_SHZ + m_lbMsg->getContentSize().width / 2, 18));
	m_lbMsg->setString(szDescription);

	int vIndex = vipColorIndex(szDescription);
	m_lbMsg->setColor(cocos2d::ccc3(iVipColorTable_SHZ[vIndex][0], iVipColorTable_SHZ[vIndex][1], iVipColorTable_SHZ[vIndex][2]));

	CCActionInterval * a1=CCMoveTo::create(10.0f,Vec2(-100,18));
	CCSequence* act = CCSequence::create(CCHide::create(), CCDelayTime::create(m_iCount * 6.0f),CCShow::create(), a1, CCCallFuncND::create(this, callfuncND_selector(SHZCRoomGameNotice::RollText), 0),0);

	m_iCount++;

	m_bg->setVisible(true);

	m_lbMsg->runAction(act);

	m_TailorView->addChild(m_lbMsg);

	PLAZZ_PRINTF("%s",szDescription);
}

void SHZCRoomGameNotice::RollText(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	m_iCount--;

	if (m_iCount<=0)
	{
		m_iCount=0;

		m_bg->setVisible(false);
	}

	deadSpt->removeFromParentAndCleanup(true);
}

int SHZCRoomGameNotice::vipColorIndex(const char* strMsg){

	const char* pVipLoc = strstr(strMsg, "VIP");

	int index = (NULL == pVipLoc ? 0 : *(pVipLoc + 3) - 48);

	log("CGameClientView::vipColorIndex: %d", index);
	return index;
}