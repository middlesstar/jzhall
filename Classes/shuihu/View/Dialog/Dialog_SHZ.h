
#ifndef SHZ_Dialog_h_
#define SHZ_Dialog_h_

#include "cocos2d.h"
#include "cocos-ext.h"

class SHZDialog: public cocos2d::CCNode
{
public:
	static SHZDialog* create(const char* file, cocos2d::CCRect rect, cocos2d::CCRect capInsets);
	static SHZDialog* create(const char* spriteFrameName, cocos2d::CCRect capInsets);
	static SHZDialog* create(cocos2d::CCSpriteFrame* spriteFrame, cocos2d::CCRect capInsets);
private:
	SHZDialog();
	~SHZDialog();
	bool initWithFile(const char* file, cocos2d::CCRect rect, cocos2d::CCRect capInsets);
	bool initWithSpriteFrameName(const char* spriteFrameName, cocos2d::CCRect capInsets);
	bool initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, cocos2d::CCRect capInsets);
    

public:
	virtual void onEnter();
	virtual void onExit();

	void setTitle(const char *title, int fontsize=20, const char* font="");
	void setContentText(const char* text, int padding = 50, int paddintTop = 100, int fontsize = 20, const char* font="");
	void setCallbackFunc(cocos2d::CCObject* target, cocos2d::SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);    
	bool addButton(cocos2d::CCNode *normal, cocos2d::CCNode *selected, const char *title, int tag = 0);

	void show(cocos2d::CCNode* parent=0);
	void doModal(cocos2d::CCNode* parent=0);

	/*--------------todosl---------------*/
	void remove();
	/*---------------------------------*/
private:
	void buttonCallback(cocos2d::CCObject* pSender);

private:
	int mContentPadding;
	int mContentPaddingTop;

	cocos2d::CCObject*		mTarget;
	cocos2d::SEL_CallFuncN	mCallback;

	cocos2d::ui::Scale9Sprite*	mBackground;
	cocos2d::CCMenu*			mMenu;
	cocos2d::CCLabelTTF*		mLbTitle;
	cocos2d::CCLabelTTF*		mLbContentText;

	class __SHZShieldLayer*			mShieldLayer;
};

#endif