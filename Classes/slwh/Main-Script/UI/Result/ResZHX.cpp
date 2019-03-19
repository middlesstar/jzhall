#include "ResZHX.h"


//节点下的名称
#define _BEILV			"num"


ResZHX::ResZHX():
m_zhx(NULL),
m_beilv(NULL)
{

}



ResZHX::~ResZHX()
{

}



void ResZHX::Start()
{
	m_zhx = dynamic_cast<ImageView*>(m_node);
	CC_ASSERT(NULL != m_zhx);

	m_beilv = dynamic_cast<TextAtlas*>(m_node->getChildByName(_BEILV));
	CC_ASSERT(NULL != m_beilv);

}


void ResZHX::SetZHX(enumGameZXH e)
{
	if (NULL == m_node)
	{
		return;
	}

	std::stringstream str;
	
	switch (e)
	{
	case InvalidZHX:
		//打个酱油
		break;
	case Zhuang:
		m_zhx->loadTexture(Path_RES_PNG_ZHUANG, TextureResType::PLIST);
		str << ZHUANG_BEILV;

		break;
	case He:
		m_zhx->loadTexture(Path_RES_PNG_HE, TextureResType::PLIST);
		str << HE_BEILV;
		break;
	case Xian:
		m_zhx->loadTexture(Path_RES_PNG_XIAN, TextureResType::PLIST);
		str << XIAN_BEILV;
		break;
	case ZXHmax:
		//打个酱油
		break;
	default:
		break;
	}
	m_beilv->setString(str.str());
}
