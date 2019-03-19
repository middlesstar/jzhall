#include "StakeItem.h"
#include "shark/NetWork/SharkNetworkKernel.h"
  
#define     LOGO             "Image_11"
#define     RATELABEL        "bei"
#define     StAKESHOW        "stakeScore"
#define     LEFTBOARD        "boardShow"

//显示动物图像的路径              
#define      Animal         "images/shak/game_scence/gs_animal%d.png"
#define      Dove           "images/shak/new_animals/na_dove.png"
#define      Eagle          "images/shak/new_animals/na_eagle.png"
#define      Peacock        "images/shak/new_animals/na_peacock.png"
#define      Swallow        "images/shak/new_animals/na_swallow.png"
#define      Birds          "images/shak/new_animals/na_birds.png"
#define      Mammalian      "images/shak/new_animals/na_mammalian.png"
#define      GoldShark      "images/shak/new_animals/na_gold.png"
#define      SilverShark    "images/shak/new_animals/na_silver.png"

StakeItem* StakeItem::m_templateThis = nullptr;
StakeItem::StakeItem()
:m_rootNode(nullptr)
, m_logo(nullptr)
, m_stakeShow(nullptr)
, m_beilV(nullptr)
{

} 
StakeItem::~StakeItem()
{

}

void StakeItem::initTemplate(ImageView* node)
{
	if (m_templateThis != nullptr)
	{
		delete m_templateThis;
		m_templateThis = nullptr;
	}

	CC_ASSERT(node != nullptr);
	m_templateThis = new StakeItem();
	//不随父节点透明
	node->setCascadeOpacityEnabled(false);
	//隐去占位图片
	node->setOpacity(_ZERO);

	m_templateThis->m_rootNode = node;

	m_templateThis->m_logo = dynamic_cast<ImageView*>(node->getChildByName(LOGO));
	CC_ASSERT(m_templateThis->m_logo != nullptr);


	m_templateThis->m_beilV = dynamic_cast<Text*>(node->getChildByName(RATELABEL));
	CC_ASSERT(m_templateThis->m_beilV != nullptr);

	m_templateThis->m_stakeShow = dynamic_cast<TextAtlas*>(node->getChildByName(StAKESHOW));
	CC_ASSERT(m_templateThis->m_stakeShow);

}

StakeItem* StakeItem::create()
{
	if (m_templateThis == nullptr)
	{
		return nullptr;
	}

	auto This = new StakeItem();

	This->m_rootNode = dynamic_cast<ImageView*>(m_templateThis->m_rootNode->clone());
	CC_ASSERT(This->m_rootNode != nullptr);

	This->m_logo = dynamic_cast<ImageView*>(This->m_rootNode->getChildByName(LOGO));
	CC_ASSERT(This->m_logo != nullptr);


	This->m_beilV = dynamic_cast<Text*>(This->m_rootNode->getChildByName(RATELABEL));
	CC_ASSERT(This->m_beilV != nullptr);

	This->m_stakeShow = dynamic_cast<TextAtlas*>(This->m_rootNode->getChildByName(StAKESHOW));
	CC_ASSERT(This->m_stakeShow);

	return This;
}

void StakeItem::setRate(int rate)
{
	std::stringstream str;
	str << rate;
	m_beilV->setString(str.str());
}
void StakeItem::setStakeNum(SCORE num)
{
	std::stringstream str;
	str << num;
	m_stakeShow->setString(str.str());
}
void StakeItem::setLogo(int index)
{
	switch (index)
	{
	case 0:
		m_logo->loadTexture(Birds, TextureResType::PLIST);
		break;
	case 1:
		m_logo->loadTexture(GoldShark, TextureResType::PLIST);
		break;
	case 2:
		m_logo->loadTexture(SilverShark, TextureResType::PLIST);
		break;
	case 3:
		m_logo->loadTexture(Mammalian, TextureResType::PLIST);
		break;
	case 4:
		m_logo->loadTexture(Swallow, TextureResType::PLIST);
		break;
	case 5:
		m_logo->loadTexture(Dove, TextureResType::PLIST);
		break;
	case 6:
		m_logo->loadTexture(Peacock, TextureResType::PLIST);
		break;
	case 7:
		m_logo->loadTexture(Eagle, TextureResType::PLIST);
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		char name[256];
		sprintf(name, Animal, index - 7);
		m_logo->loadTexture(name, TextureResType::PLIST);
		break;
	default:
		break;
	}
	
	
}

void StakeItem::CallBack(Node* node)
{
	//销毁
	node->removeFromParent();
}

//Widget* StakeItem:: getWgt()
//{
//	return dynamic_cast<ImageView*>(m_templateThis->m_rootNode->clone());
//}
