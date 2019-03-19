#ifndef  _ViewBase_H_
#define  _ViewBase_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"

#include "shark/resourceDef.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

/* ´°¿Ú»ùÀà */
class  ViewBase : public Node
{
public:
	ViewBase();
	virtual ~ViewBase();

	virtual bool dependNode(Node* const &spr);

	virtual void startPlay(){};

	virtual void stopPlay(){};

	
protected:
	Node* _seekNodeByName(Node* pRootNode, const std::string& name);
	virtual void setContent(){};
    Node* m_pNode;

};

#endif // _WindowBase_H_

