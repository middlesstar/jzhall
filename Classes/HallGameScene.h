#ifndef __HallGameScene_H__
#define __HallGameScene_H__

#include "cocos2d.h"
#include "Window/WindowDefine.h"
#include "Window/ConfirmDialog/CommonConfirmNode.h"
USING_NS_CC;

class ConfirmDialogDelegate;
class WindowBase;

enum eHallSceneType
{
	EHST_HALL	=	1,			//大厅场景
	EHST_SLWH	=	2,			//森林舞会场景
	EHST_SHARK	=	3,			//捕鱼场景
	EHST_SHUIHU	=	4,			//水浒传场景

	EHST_MAX	
};

class HallGameScene : public Layer
{
public:
	HallGameScene();
	~HallGameScene();

	static Scene* createScene();		//创建scene
	static HallGameScene* getInstance();

	static WindowBase* createWindow(int nWid);				//创建一个大厅的窗口，一般由大厅以外的scene调用(适用于子游戏中打开大厅界面的情况)
	static void setHandleKeyboard(bool bHandle);
	static void openSceneMessageTip(const char* pText);		//在全局场景打开消息框
	static void openChatSprite(Node* pParentNode);			//打开聊天悬浮框
	static int getCurRoomid();

    CREATE_FUNC(HallGameScene);
	virtual bool init() override;  

    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	virtual void onEnter() override;
	virtual void onExit() override;

public:
	static std::vector<std::string> m_gSearchPathVec;
	static void switchScene(int nSceneId);

	static int m_gCurSceneId;
protected:
	static void initSubGame(int nSceneId);

public:
	void openWindow(int nWid);			    //打开编号为nWid的窗口
	void closeWindow(int nWid);			    //关闭窗口
    void updateWindow(int nWid,int nCode = 0);  //更新窗口,如果没有窗口，强制打开
	void updateWindow(int nWid, int nCode,void * pData);  //更新窗口,如果没有窗口，强制打开
public:
	void exitGame();						//退出游戏
	void openMessageTip(const char* pText);	//打开消息提示
    void openMessageTip(int nStrId);	    //打开消息提示

	//打开确认框，可以传入一个int型数据和一个void*数据
	//void openOkDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate,int nData = 0,void* pData = nullptr);
	//void openOkCancelDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate, int nData = 0, void* pData = nullptr);

	void openOkDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate = nullptr, int nData = 0, void* pData = nullptr);
	void openOkCancelDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate = nullptr, int nData = 0, void* pData = nullptr);
	void openOkVipDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate = nullptr, int nData = 0, void* pData = nullptr);

	void backToLoginWindow();			//返回登录界面
protected:
	Node* m_pWindowParentNode;	//窗口父节点
	Node* m_pDialogParentNode;	//对话框父节点

	CommonConfirmNode* m_pCommonConfirmNode;		//公用确认框的节点
};

#endif // __HallGameScene_H__
