#ifndef  _CommonConfirmNode_H_
#define  _CommonConfirmNode_H_

#include "cocos2d.h"
#include "ConfirmDialogDelegate.h"

/* ȷ�Ͽ� */
enum eCommonConfirmNodeType
{
	ECCNT_DISCONNECT,				//����Ͽ���ʾ��
	ECCNT_EXIT,						//�˳���Ϸ��ʾ��
	ECCNT_BACKTOLOGIN,				//���ص�¼����

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

