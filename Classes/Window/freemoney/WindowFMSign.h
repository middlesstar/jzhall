#ifndef  _WindowFMSign_H_
#define  _WindowFMSign_H_

#include "Window/WindowBase.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Network/CommandDef.h"
#include "Network/CommandStructDef.h"

/* ���ڻ��� */
class  WindowFMSign : public WindowBase
{
public:
	WindowFMSign();
	virtual ~WindowFMSign();

	CREATE_FUNC(WindowFMSign);
	virtual void setContent();				                //����UI����
	virtual void updateWindow(int nCode);	                //ˢ�´���

	virtual void onEnter() override;


//��ʾ����
public:
	void showView();
	void onQueryDailySign();
private:
	//����ǩ����ť�ɴ���
	void setDailySignButtonEnabled(int tag, bool enabled);
	void onBtnClicked(cocos2d::Ref* pSender, cocos2d::extension::Control::EventType type);
	//������ɫ�����Ƿ����ǩ��
	void setContinueSignButtonEnabled(bool enabled);
	//��ʾǩ������ת
	void addBlockAnimation(int tag);
	//��ȡǩ����ť��λ��
	cocos2d::extension::ControlButton *getDailySignButton(int tag);
	//������ʾǩ���Ķ�����ת
	Animate *createAnimate(const char *nameFormat, int startIndex, int endIndex, float delay);
	//�������ȡ��ǩ
	void addGotSign(int tag);

protected:
	Node *pMainNode;
	ui::ScrollView* m_scrollView;
	//��ʾ����ǩ��������
	LabelTTF *m_labelContinue;
	//��ɫ���䰴ť
	cocos2d::extension::ControlButton *m_continueSignBtn;
};

#endif // _WindowFMSign_H_

