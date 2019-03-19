#include "ViewBase.h"

ViewBase::ViewBase()
    : m_pNode(nullptr)
    
{

}

ViewBase::~ViewBase()
{
}

bool ViewBase::dependNode(Node* const &node)
{
	if (nullptr == node || m_pNode == node)
	{
		CC_ASSERT(false);
		return false;
	}
	m_pNode = node;

	setContent();
	return true;
}
Node* ViewBase::_seekNodeByName(Node* pRootNode, const std::string& name)
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





