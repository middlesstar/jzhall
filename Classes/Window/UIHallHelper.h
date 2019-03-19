#ifndef  _UIHallHelper_H_
#define  _UIHallHelper_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

//����������
enum eEditBoxType
{
	EEBT_NUM			= 1,		//����
	EEBT_TEXT			= 2			//����
};

class  UIHallHelper
{
public:
	static cocos2d::ui::EditBox* createAccountEditBox(TextField* pRawInput, int nMaxLength,const std::string& strBgImg,
		int nType = EEBT_NUM, cocos2d::ui::EditBoxDelegate* pDelegate = nullptr);
};

#endif // _UIHallHelper_H_

