//
//  XJScrollLabel.h
//
//  Created by jason on 13-1-30.
//

#ifndef SHZ___COG__XJScrollLabel__
#define SHZ___COG__XJScrollLabel__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define POINTER_OPERATION( pointer, operation ) \
	do\
{\
	if( pointer )\
{\
	operation;\
}\
}while( 0 )

//the anchor point of XJScrollLabel is the left-bottom corner.
class SHZXJScrollLabel : public CCNode, public ScrollViewDelegate
{
public://static functions
	static SHZXJScrollLabel* create( const char* pszText, const char *fontName, float fontSize, CCTextAlignment hAlignment, CCSize ScrollViewSize );

public:
	SHZXJScrollLabel();
	~SHZXJScrollLabel();
	bool init( const char* pszText, const char *fontName, float fontSize, CCTextAlignment hAlignment, CCSize& ScrollViewSize );

	void setTextColor(const ccColor3B& color3)
	{ POINTER_OPERATION( m_pLabel, m_pLabel->setColor( color3 ) ); }

	CCLabelTTF* getLabel() { return m_pLabel; }
	void setTouchEnabled(bool bEnabled);
public://scroll view delegate
	virtual void scrollViewDidScroll(ScrollView* view){}
	virtual void scrollViewDidZoom(ScrollView* view){}

private:
	ScrollView* m_pScrollView;
	CCLabelTTF* m_pLabel;
};

#endif /* defined(__COG__CGScrollLabel__) */
