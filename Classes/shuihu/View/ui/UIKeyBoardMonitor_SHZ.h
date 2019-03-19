#ifndef SHZ__UIKeyBoardMonitor_H_
#define SHZ__UIKeyBoardMonitor_H_

#include "cocos2d.h"
USING_NS_CC;

class SHZUIKeyBoardMonitor : public cocos2d::CCLayer
{

public:

	CREATE_FUNC(SHZUIKeyBoardMonitor);

	virtual bool init();

	//������Androidƽ̨�µķ��ؼ�����
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void onReturnCallback(cocos2d::CCNode *pNode);

	// ����title��content
	void setTitleAndContent(const char *title, const char *content);

	// ���ó���
	void setSceneName(char *sceneName);

private:
	const char *title;
	const char *content;
	char		sceneName[40];
};

#endif