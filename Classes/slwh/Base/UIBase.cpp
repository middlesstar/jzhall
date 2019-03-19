#include "UIBase.h"

#ifndef _END
#define _END -1
#endif


UIBase::UIBase():
m_node(nullptr)
{
}


UIBase::~UIBase()
{

}

bool UIBase::AttachNode(Node* const &spr)
{
	if (nullptr == spr || m_node == spr)
	{
		//CC_ASSERT(false,"AttachNode false nullptr == spr || m_node == spr"/*, "关联根节点有问题"*/);
		return false;
	}
	m_node = spr;
	//this->onEnter();
	//this->onEnterTransitionDidFinish();
	//正确关联资源后 开始管理这个资源
	Start();
	return true;
}


void UIBase::Move(Vec2 v2)
{
	Vec2 v = m_node->getPosition();
	v += v2;
	m_node->setPosition(v);
}

void UIBase::Move(Node* node, Vec2 v2)
{
	Vec2 v = node->getPosition();
	v += v2;
	node->setPosition(v);
}

void UIBase::setVisible(const bool &b)
{
	if (nullptr == m_node)
	{
		CC_ASSERT(false);
		return;
	}
	m_node->setVisible(b);
}

//void UIBase::FitUI(Node* rootNode, int* up_ui, int* mid_ui, int* full_ui)
//{
//
//	Vec2 visible = Director::getInstance()->getWinSize();
//	Vec2 designSize = DesignResolutionSize;
//
//	//高度偏移
//	float offsetH = visible.y - designSize.y;
//
//	Vec2 v2;
//	//对上层UI进行适配
//	if (nullptr != up_ui)
//	{
//		for (size_t i = _ZERO; up_ui[i] != _END; i++)
//		{
//			auto node = rootNode->getChildByTag(up_ui[i]);
//			if (nullptr != node)
//			{
//				v2 = node->getPosition();
//				v2.y += offsetH;
//				node->setPosition(v2);
//			}
//		}
//	}
//	
//	float scale = visible.y / designSize.y;
//	//对中层UI进行适配
//	if (nullptr != mid_ui)
//	{
//		for (size_t i = _ZERO; mid_ui[i] != _END; i++)
//		{
//			auto node = rootNode->getChildByTag(mid_ui[i]);
//			if (nullptr != node)
//			{
//				v2 = node->getPosition();
//				v2.y = v2.y * scale;
//				node->setPosition(v2);
//			}
//		}
//	}
//	
//
//	//对全屏UI经行铺满处理
//	if (nullptr != full_ui)
//	{
//		for (size_t i = _ZERO; full_ui[i] != _END; i++)
//		{
//			auto node = rootNode->getChildByTag(full_ui[i]);
//			if (nullptr != node)
//			{
//				v2 = node->getPosition();
//				v2.y = v2.y * scale;
//				node->setPosition(v2);
//				node->setScaleY(scale);
//			}
//		}
//	}
//	
//}
//
//
