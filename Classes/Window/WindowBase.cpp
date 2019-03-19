#include "WindowBase.h"

WindowBase::WindowBase()
    : m_pNode(nullptr)
	, m_pLayerColor(nullptr)
    , m_pEventListener(nullptr)
	, m_bSwallow(true)
	, m_bScaleIn(false)
{

}

WindowBase::~WindowBase() 
{
}

void WindowBase::onEnter()
{
	Layer::onEnter();

	setTouchSwallow(m_bSwallow);
}

void WindowBase::onExit()
{
	Layer::onExit();
}


bool WindowBase::init()
{
	if (!Layer::init())
	{
		return false;
	}

    m_pNode = Node::create();
	this->addChild(m_pNode);
	Size size = Director::getInstance()->getWinSize();
	m_pNode->setPosition(Vec2(size.width / 2, size.height / 2));

	setContent();

	return true;
}

void WindowBase::onBackKeyPressed()
{
	this->close();
}

void WindowBase::close()
{
	m_bScaleIn ? ScaleOut() : removeFromParent();
}

void WindowBase::setContent()
{

}

void WindowBase::updateWindow(int nCode)
{

}

void WindowBase::updateWindow(int nCode, void* pData)
{

}

void WindowBase::setTouchSwallow(bool bSwallow/* = true*/)
{
    if (bSwallow)
    {
        if (m_pEventListener)
        {
            return;
        }

        m_pEventListener = EventListenerTouchOneByOne::create();
        m_pEventListener->setSwallowTouches(true);

        m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool{return true;};
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
    }
    else
    {
        getEventDispatcher()->removeEventListener(m_pEventListener);
        m_pEventListener = nullptr;
    }
}

void WindowBase::addBlackLayer(GLubyte gOpacity/* = 100*/)
{
	Size winSize = Director::getInstance()->getWinSize();

	m_pLayerColor = LayerColor::create(Color4B(0,0,0,gOpacity),winSize.width,winSize.height);
	this->addChild(m_pLayerColor, m_pNode->getLocalZOrder() - 1);
}


void WindowBase::ScaleIn(float fScale/* = 0.0f*/ , float fTime/* = 0.3f*/)
{
	if (!m_pNode)
	{
		return;
	}

    if (m_pLayerColor)
    {
        GLubyte opacity = m_pLayerColor->getOpacity();
        m_pLayerColor->setOpacity(0);
        m_pLayerColor->runAction(FadeTo::create(fTime,opacity));
    }

	m_pNode->setScale(fScale);
	m_pNode->runAction(EaseBackOut::create(ScaleTo::create(fTime,1)));	//添加一个缩放动画

	m_bScaleIn = true;
}

void WindowBase::ScaleOut(float fTime/* = 0.3f*/)
{
	if (!m_pNode)
	{
		return;
	}

	m_pNode->runAction(ScaleTo::create(fTime,0));
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(WindowBase::close_callback),fTime);

	if (m_pLayerColor)
	{
		m_pLayerColor->runAction(FadeTo::create(fTime,0));
	}

	m_bScaleIn = false;
}

void WindowBase::playButtonClickSound()
{
	SoundManager::shared()->playEffectNormal(SoundManager::shared()->getHallMusicFileFullPath("ui").c_str());
}

Node* WindowBase::_loadCSBLayoutFile(const std::string& strLayoutFile)
{
	Node *pMainNode = CSLoader::getInstance()->createNode(strLayoutFile);

	if (pMainNode)
	{
		pMainNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	}

	return pMainNode;
}

Node* WindowBase::_seekNodeByName(Node* pRootNode, const std::string& name)
{
	if (!pRootNode)
	{
		return nullptr;
	}
	if (pRootNode->getName() == name)
	{
		return pRootNode;
	}
	const auto& arrayRootChildren = pRootNode->getChildren();
	for (auto& subNode : arrayRootChildren)
	{
		Node* child = dynamic_cast<Node*>(subNode);
		if (child)
		{
			Node* res = _seekNodeByName(child, name);
			if (res != nullptr)
			{
				return res;
			}
		}
	}
	return nullptr;
}

void WindowBase::_addButtonClickEvent(Node* pRootNode, const std::string& btnName, std::function<void(Ref*)> btnCallback)
{
	Button* pBtn = dynamic_cast<Button*>(_seekNodeByName(pRootNode, btnName));

	if (pBtn)
	{
		//pBtn->addClickEventListener(btnCallback);
		pBtn->addClickEventListener([btnCallback,this](Ref* pRef){
			btnCallback(pRef);
			playButtonClickSound();
		});
	}
}

void WindowBase::close_callback(float dt)
{
	close();
}