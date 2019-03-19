#ifndef  _WindowExchange_H_
#define  _WindowExchange_H_

#include "Window/WindowBase.h"

struct CMD_ST_DxRewardList_rsp;
struct ItemGoodsDx;

/* �һ����� */
enum eWindowExchangeState
{
	EWES_NORMAL,		//�һ�����
	EWES_RECORD,		//�һ���¼
	EWES_GOODS,			//�һ���Ʒ
	EWES_TEL			//�һ�����
};

class  WindowExchange : public WindowBase , public ConfirmDialogDelegate
{
public:
	WindowExchange();
	virtual ~WindowExchange();

	CREATE_FUNC(WindowExchange);
	virtual void setContent() override;				                //����UI����
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;

	virtual void onOkClick(int nData, void* pData) override;
protected:
	void _setUIState(int nState);
	void _setScrollContent();
	void _setExchangeRecord(CMD_ST_DxRewardList_rsp* pData);
	void _setRecordItem(ImageView* pItem, ItemGoodsDx* pRecordData);
	void _adjustScrollHeight(ui::ScrollView* pRecordScroll,float fIncreaseHeight);		//��pRecordScroll��innersize����fIncreaseHeight

	void _initPhoneAndQQInput();
	void _setExchangeTelName();
	void _setExchangeGoodsName();

	void _ItemClick_Callback(Ref* pRef);
	void _ItemExchange_Callback(Ref* pRef);
	void _GoodsExchange_Callback(Ref* pRef);

	void refreshGold(Ref* ref);
protected:
	Sprite* m_pInfoSpr;
	Node* m_pNodeExchange;
	Node* m_pNodeRecord;
	Node* m_pNodeGoods;
	Node* m_pNodeTel;

	Button* m_pBtnRecord;
	Button* m_pBtnExchange;

	ui::ScrollView* m_pRecordScroll;
	ImageView* m_pRecordItem;

	int m_nSelectIndex;
	bool m_bRefreshRecord;				//�Ƿ�Ҫˢ�¶ҽ���¼

	cocos2d::ui::EditBox* m_pTelPhoneNumInput;
	cocos2d::ui::EditBox* m_pTelQQInput;

	cocos2d::ui::EditBox* m_pGoodsPhoneNumInput;
	cocos2d::ui::EditBox* m_pGoodsQQInput;
};

#endif // _WindowExchange_H_

