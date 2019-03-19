#ifndef  _CommonConfirmNode_H_
#define  _CommonConfirmNode_H_

#include "cocos2d.h"
#include "ConfirmDialogDelegate.h"

/* 确认框 */
enum eCommonConfirmNodeType
{
	ECCNT_DISCONNECT,				//网络断开提示框
	ECCNT_EXIT,						//退出游戏提示框
	ECCNT_BACKTOLOGIN,				//返回登录界面

	ECCNT_MAX
};

class  CommonConfirmNode : public cocos2d::Node, public ConfirmDialogDelegate
{
public:
	CommonConfirmNode();
	virtual ~CommonConfirmNode();

	CREATE_FUNC(CommonConfirmNode);
	virtual bool init() override;

public:
	virtual void onOkClick(int nData, void* pData) override;
	virtual void onCancelClick(int nData, void* pData) override;
};

#endif // _CommonConfirmNode_H_

