#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "UIHelp_SHZ.h"
#include "ShieldLayer_SHZ.h"

USING_NS_CC;
USING_NS_CC_EXT;

//////////////////////////////////////////////////////////////////////////
#define PLIST_PATH1_SHZ		"help/help1.plist"
#define TEXTURE_PATH_SHZ1_SHZ	"help/help1.png"

#define PLIST_PATH2_SHZ		"help/help2.plist"
#define TEXTURE_PATH_SHZ2_SHZ	"help/help2.png"

#define IMG_CLOSE_SHZ		"xxx.png"


#define HELP_NUM_SHZ		4

//////////////////////////////////////////////////////////////////////////
SHZUIHelp::SHZUIHelp()
{
	//mShieldLayer = 0;
}

SHZUIHelp::~SHZUIHelp()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(PLIST_PATH1_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(TEXTURE_PATH_SHZ1_SHZ);

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(PLIST_PATH2_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(TEXTURE_PATH_SHZ2_SHZ);
}

//初始化方法
bool SHZUIHelp::init()
{
	bool bRet = false;

	do 
	{
		this->touchEvent();
		/*touch_layer_ = TouchLayer::create(-130);
		addChild(touch_layer_);
		touch_layer_->setSink(this);
		touch_layer_->setTouchEnabled(true);*/

	

		CC_BREAK_IF(!CCNode::init());
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH1_SHZ);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH2_SHZ);
	

	/*	CCScene* scene = CCDirector::getInstance()->getRunningScene();
		mShieldLayer = ShieldLayer::create(-130);
		this->addChild(mShieldLayer);*/


		//////////////////////////////////////////////////////////////////////////
		//关闭按钮
		ControlButton* btClose = createButton_SHZ(IMG_CLOSE_SHZ, 0, 0, this, cccontrol_selector(SHZUIHelp::onCloseClick));
		btClose->setTag(200);
		//btClose->setTouchPriority(-131);
		btClose->setPosition(Vec2(465, 310));
		addChild(btClose,100);  
		
		for (int i = HELP_NUM_SHZ; i >= 0; i--)
		{

			char str[32];

			sprintf(str,"help_bg%d.png",i);

			m_spHelp[i]=CCSprite::createWithSpriteFrameName(str);

			m_spHelp[i]->setPosition(Vec2(0,0));

			addChild(m_spHelp[i]);
		}
		

		m_iCount=0;

		m_bMove=false;

		bRet = true;
	} while (0);

	return bRet;
}

void SHZUIHelp::onExit()
{
	Node::onExit();
	/*------修改bysl-------------*/
	//touch_layer_->setTouchEnabled(false);
	//touch_layer_->setSink(0);
	//CCNode::onExit();

	getEventDispatcher()->removeEventListener(listener);
	listener = nullptr;

	//if (mShieldLayer)
	//	mShieldLayer->removeFromParentAndCleanup(true);
}

void SHZUIHelp::onCloseClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{ 
	this->removeFromParentAndCleanup(true);
}
void SHZUIHelp::touchEvent()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool
	{
		m_ptPos = touch->getLocation();
 		return true; 
	};
	listener->onTouchMoved = [this](Touch* touch, Event* event)	{};
	listener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		if (m_bMove)
		{
			return;
		}
		// left shift
		if (m_ptPos.x < touch->getLocation().x)
		{
			m_iCount--;

			if (m_iCount < 0)
			{
				m_iCount = 0;

				return;
			}

			SetRightAnim();

		}
		// right shift
		else if (m_ptPos.x > touch->getLocation().x)
		{
			m_iCount++;

			if (m_iCount > HELP_NUM_SHZ)
			{
				m_iCount = HELP_NUM_SHZ;

				return;
			}
			SetLeftAnim();

		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


}
void SHZUIHelp::SetLeftAnim()
{

	m_bMove=true;

	m_spHelp[m_iCount]->setPosition(Vec2(kRevolutionWidth_SHZ,0));

	m_spHelp[m_iCount]->setVisible(true);

	m_spHelp[m_iCount-1]->setPosition(Vec2(0,0));

	CCActionInterval * a1=CCMoveTo::create(0.5f,Vec2(0,0));

	CCActionInterval * a2=CCMoveTo::create(0.5f,Vec2(-kRevolutionWidth_SHZ,0));

	CCSequence* act = CCSequence::create(a1, CCCallFuncND::create(this, callfuncND_selector(SHZUIHelp::RightHelpMove), 0),0);
	m_spHelp[m_iCount-1]->runAction(a2);
	m_spHelp[m_iCount]->runAction(act);
}

void SHZUIHelp::SetRightAnim()
{
	m_bMove=true;

	m_spHelp[m_iCount]->setPosition(Vec2(-kRevolutionWidth_SHZ,0));

	m_spHelp[m_iCount]->setVisible(true);

	m_spHelp[m_iCount+1]->setPosition(Vec2(0,0));

	CCActionInterval * a1=CCMoveTo::create(0.5f,Vec2(0,0));

	CCActionInterval * a2=CCMoveTo::create(0.5f,Vec2(kRevolutionWidth_SHZ,0));

	CCSequence* act = CCSequence::create(a1, CCCallFuncND::create(this, callfuncND_selector(SHZUIHelp::RightHelpMove), 0),0);
	m_spHelp[m_iCount+1]->runAction(a2);
	m_spHelp[m_iCount]->runAction(act);
}
void SHZUIHelp::RightHelpMove(cocos2d::CCNode* node, void* data)
{
	m_bMove=false;
}
void SHZUIHelp::LeftHelpMove(cocos2d::CCNode* node, void* data)
{

}