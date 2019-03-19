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
	EHST_HALL	=	1,			//��������
	EHST_SLWH	=	2,			//ɭ����᳡��
	EHST_SHARK	=	3,			//���㳡��
	EHST_SHUIHU	=	4,			//ˮ䰴�����

	EHST_MAX	
};

class HallGameScene : public Layer
{
public:
	HallGameScene();
	~HallGameScene();

	static Scene* createScene();		//����scene
	static HallGameScene* getInstance();

	static WindowBase* createWindow(int nWid);				//����һ�������Ĵ��ڣ�һ���ɴ��������scene����(����������Ϸ�д򿪴�����������)
	static void setHandleKeyboard(bool bHandle);
	static void openSceneMessageTip(const char* pText);		//��ȫ�ֳ�������Ϣ��
	static void openChatSprite(Node* pParentNode);			//������������
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
	void openWindow(int nWid);			    //�򿪱��ΪnWid�Ĵ���
	void closeWindow(int nWid);			    //�رմ���
    void updateWindow(int nWid,int nCode = 0);  //���´���,���û�д��ڣ�ǿ�ƴ�
	void updateWindow(int nWid, int nCode,void * pData);  //���´���,���û�д��ڣ�ǿ�ƴ�
public:
	void exitGame();						//�˳���Ϸ
	void openMessageTip(const char* pText);	//����Ϣ��ʾ
    void openMessageTip(int nStrId);	    //����Ϣ��ʾ

	//��ȷ�Ͽ򣬿��Դ���һ��int�����ݺ�һ��void*����
	//void openOkDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate,int nData = 0,void* pData = nullptr);
	//void openOkCancelDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate, int nData = 0, void* pData = nullptr);

	void openOkDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate = nullptr, int nData = 0, void* pData = nullptr);
	void openOkCancelDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate = nullptr, int nData = 0, void* pData = nullptr);
	void openOkVipDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate = nullptr, int nData = 0, void* pData = nullptr);

	void backToLoginWindow();			//���ص�¼����
protected:
	Node* m_pWindowParentNode;	//���ڸ��ڵ�
	Node* m_pDialogParentNode;	//�Ի��򸸽ڵ�

	CommonConfirmNode* m_pCommonConfirmNode;		//����ȷ�Ͽ�Ľڵ�
};

#endif // __HallGameScene_H__
