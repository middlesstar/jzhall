#include "Dialog_SHZ.h"
#include "shuihu/View/Scene/getHouseIndex_SHZ.h"
USING_NS_CC;
USING_NS_CC_EXT;

//////////////////////////////////////////////////////////////////////////
//屏蔽层
class __SHZShieldLayer: public CCLayer
{
public:
	CREATE_FUNC(__SHZShieldLayer);
private:
	__SHZShieldLayer(){}

	~__SHZShieldLayer()
	{
		setTouchSwallow(false);
	}

	bool init()
	{
		do 
		{
			CC_BREAK_IF(!CCLayer::init());
			setTouchSwallow(true);
			return true;
		} while (0);
		return false;
	}


public:
	void setTouchSwallow(bool bSwallow = true)
	{
		if (bSwallow)
		{

			m_pEventListener = EventListenerTouchOneByOne::create();
			m_pEventListener->setSwallowTouches(true);

			m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool{return true; };
			getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
		}
		else
		{
			getEventDispatcher()->removeEventListener(m_pEventListener);
			m_pEventListener = nullptr;
		}
	}

private:
	EventListenerTouchOneByOne* m_pEventListener;
};

//////////////////////////////////////////////////////////////////////////
SHZDialog* SHZDialog::create(const char* file, cocos2d::CCRect rect, cocos2d::CCRect capInsets)
{
	SHZDialog* dlg = new SHZDialog();
	if (dlg && dlg->initWithFile(file, rect, capInsets))
	{
		dlg->autorelease();
		return dlg;
	}
	delete dlg;
	return 0;
}

SHZDialog* SHZDialog::create(const char* spriteFrameName, cocos2d::CCRect capInsets)
{
	SHZDialog* dlg = new SHZDialog();
	if (dlg && dlg->initWithSpriteFrameName(spriteFrameName, capInsets))
	{
		dlg->autorelease();
		return dlg;
	}
	delete dlg;
	return 0;
}

SHZDialog* SHZDialog::create(cocos2d::CCSpriteFrame* spriteFrame, cocos2d::CCRect capInsets)
{
	SHZDialog* dlg = new SHZDialog();
	if (dlg && dlg->initWithSpriteFrame(spriteFrame, capInsets))
	{
		dlg->autorelease();
		return dlg;
	}
	delete dlg;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
SHZDialog::SHZDialog()
	: mMenu(0)
	, mContentPadding(0)
	, mContentPaddingTop(0)
	, mTarget(0)
	, mCallback(0)
	, mBackground(0)
	, mLbContentText(0)
	, mLbTitle(0)
	, mShieldLayer(0)
{}

SHZDialog::~SHZDialog()
{
	CC_SAFE_RELEASE_NULL(mLbTitle);
	CC_SAFE_RELEASE_NULL(mLbContentText);
}

bool SHZDialog::initWithFile(const char* file, cocos2d::CCRect rect, cocos2d::CCRect capInsets)
{
	do
	{
		CC_BREAK_IF(!CCNode::init());
		setContentSize(Size(0,0));

		mBackground = ui::Scale9Sprite::create(file, rect, capInsets);
		mBackground->setContentSize(rect.size);
		addChild(mBackground, 0, 0);
		
		mMenu = CCMenu::create();
		mMenu->setPosition(Vec2(0,0));
		//mMenu->setTouchPriority(-501);
		addChild(mMenu, 0, 0);
		return true;
	}while(0);
	return false;
}

bool SHZDialog::initWithSpriteFrameName(const char* spriteFrameName, cocos2d::CCRect capInsets)
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	return initWithSpriteFrame(cache->spriteFrameByName(spriteFrameName), capInsets);	
}

bool SHZDialog::initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, cocos2d::CCRect capInsets)
{
	do
	{
		CC_BREAK_IF(!CCNode::init());
		setContentSize(Size(0,0));

		mBackground = ui::Scale9Sprite::createWithSpriteFrame(spriteFrame, capInsets);
		mBackground->setContentSize(spriteFrame->getRect().size);
		addChild(mBackground, 0, 0);

		mMenu = CCMenu::create();
		mMenu->setPosition(Vec2(0,0));
		//mMenu->setTouchPriority(-501);
		addChild(mMenu, 0, 0);
		return true;
	}while(0);
	return false;
}

void SHZDialog::onEnter()
{
	CCNode::onEnter();

	CCSize contentSize;

	if (getContentSize().equals(Size(0,0))) {
		contentSize = mBackground->getContentSize();
	} else {
		contentSize = getContentSize();
	}

	mBackground->setContentSize(contentSize);
	
	
	Vector<Node*> m_Vector = mMenu->getChildren();
	//CCArray* array = mMenu->getChildren();
	
	float btnWidth = 0;
	float btnHeight = 0;
	float btnInterval = 70;

	for (const auto& itemData : m_Vector)
	{
		CCNode* node = dynamic_cast<CCNode*>(itemData);
		btnWidth += node->getContentSize().width + btnInterval;

		if (btnHeight < node->getContentSize().height)
			btnHeight = node->getContentSize().height;
	}
// 
// 	for (unsigned int i = 0; i < array->count(); ++i)
// 	{
// 		CCNode* node = dynamic_cast<CCNode*>(array->objectAtIndex(i));
// 		btnWidth += node->getContentSize().width+btnInterval;
// 
// 		if (btnHeight < node->getContentSize().height)
// 			btnHeight = node->getContentSize().height;
// 	}
	
	float btnX = -btnWidth/2;
	for (const auto& itemData : m_Vector)
	{
		CCNode* node = dynamic_cast<CCNode*>(itemData);
		node->setPosition(Vec2( btnX + (node->getContentSize().width+btnInterval)/2,  -contentSize.height/2 + btnHeight+20));
		btnX += node->getContentSize().width+btnInterval;
	}

	// 显示对话框标题
	if (mLbTitle)
	{
		mLbTitle->setPosition(Vec2(0, contentSize.height/2 - mContentPaddingTop / 2));
		addChild(mLbTitle);
	}

	// 显示文本内容
	if (mLbContentText)
	{
		mLbContentText->setPosition(Vec2(0, btnHeight / 2 ));
		mLbContentText->setDimensions(CCSizeMake(contentSize.width - mContentPadding * 2, contentSize.height - mContentPaddingTop - btnHeight));
		mLbContentText->setHorizontalAlignment(kCCTextAlignmentCenter);
		mLbContentText->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		addChild(mLbContentText);
	}
}

void SHZDialog::onExit()
{
	CCNode::onExit();
}

void SHZDialog::setTitle(const char *title, int fontsize, const char* font)
{
	CC_SAFE_RELEASE_NULL(mLbTitle);

	if (title == 0 || strlen(title) == 0)
		return;

	mLbTitle = CCLabelTTF::create(title, font, fontsize);
	mLbTitle->setColor(ccc3(0,0,0));
	mLbTitle->retain();
}

void SHZDialog::setContentText(const char *text, int padding, int paddingTop, int fontsize, const char* font)
{
	CC_SAFE_RELEASE_NULL(mLbContentText);

	if (text == 0 || strlen(text) == 0)
		return;

	mContentPadding		= padding;
	mContentPaddingTop	= paddingTop;
	mLbContentText		= CCLabelTTF::create(text, font, fontsize);
	mLbContentText->setColor(ccc3(255,255,255));
	mLbContentText->retain();
}

void SHZDialog::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
	mTarget		= target;
	mCallback	= callfun;    
}


bool SHZDialog::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag)
{
	// 创建图片菜单按钮
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage,nullptr, this, menu_selector(SHZDialog::buttonCallback));
	menuImage->setTag(tag);
	menuImage->setPosition(Vec2(0, 0));

	if (title != 0 && strlen(title) != 0)
	{
		CCSize imenu = menuImage->getContentSize();
		CCLabelTTF* ttf = CCLabelTTF::create(title, "", 20);
		ttf->setColor(ccc3(0, 0, 0));
		ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2+30));
		menuImage->addChild(ttf);
	}

	mMenu->addChild(menuImage);
	return true;
}

bool SHZDialog::addButton(cocos2d::CCNode *normal, cocos2d::CCNode *selected, const char *title, int tag)
{
	// 创建精灵菜单按钮
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normal, selected, nullptr,this, menu_selector(SHZDialog::buttonCallback));
	menuItem->setTag(tag);
	menuItem->setPosition(Vec2(0, 0));
	if (title != 0 && strlen(title) != 0)
	{
		CCSize imenu = menuItem->getContentSize();
		CCLabelTTF* ttf = CCLabelTTF::create(title, "", 20);
		ttf->setColor(ccc3(0, 0, 0));
		ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2+20));
		menuItem->addChild(ttf);
	}
	menuItem->setScale(1.2);
	mMenu->addChild(menuItem);
	return true;
}

void SHZDialog::buttonCallback(cocos2d::CCObject *pSender)
{
	CCNode* node = dynamic_cast<CCNode*>(pSender);
	if (mCallback && mTarget)
	{
		(mTarget->*mCallback)(node);
	}

	if (mShieldLayer)
	{
		mShieldLayer->removeFromParentAndCleanup(true);
	}
	SHZGetHouseIndex::getInstance()->setFlag(false);
	removeFromParentAndCleanup(true);
}

void SHZDialog::show(cocos2d::CCNode* parent)
{
	if (parent == 0)
		parent = CCDirector::sharedDirector()->getRunningScene();

	if (parent)
	{
		parent->addChild(this);
	}
}

void SHZDialog::doModal(cocos2d::CCNode* parent)
{
	if (parent == 0)
		parent = CCDirector::getInstance()->getRunningScene();

	if (parent)
	{
		mShieldLayer = __SHZShieldLayer::create();
		parent->addChild(mShieldLayer,1000);
		parent->addChild(this,10000);
	}
	SHZGetHouseIndex::getInstance()->setFlag(true);
}

void SHZDialog::remove()
{
	if (mShieldLayer)
	{
		mShieldLayer->removeFromParentAndCleanup(true);
	}
	SHZGetHouseIndex::getInstance()->setFlag(false);
	this->removeFromParentAndCleanup(true);
}