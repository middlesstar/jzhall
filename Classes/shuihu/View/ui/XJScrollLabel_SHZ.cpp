//
//  XJScrollLabel.cpp
//
//  Created by jason on 13-1-30.
//

#include "XJScrollLabel_SHZ.h"

SHZXJScrollLabel* SHZXJScrollLabel::create(const char *pszText, const char *fontName, float fontSize, CCTextAlignment hAlignment, cocos2d::CCSize ScrollViewSize)
{
	SHZXJScrollLabel* pScrollLabel = new SHZXJScrollLabel;
	if( pScrollLabel && pScrollLabel->init( pszText, fontName, fontSize, hAlignment, ScrollViewSize ) )
	{
		pScrollLabel->autorelease();
		return pScrollLabel;
	}
	else
	{
		CC_SAFE_DELETE( pScrollLabel );
		return NULL;
	}
}

SHZXJScrollLabel::SHZXJScrollLabel() :
	m_pScrollView( NULL ),
	m_pLabel( NULL )
{

}

SHZXJScrollLabel::~SHZXJScrollLabel()
{
	CC_SAFE_RELEASE_NULL( m_pLabel );
	CC_SAFE_RELEASE_NULL( m_pScrollView );
}

bool SHZXJScrollLabel::init(const char *pszText, const char *fontName, float fontSize, CCTextAlignment hAlignment, cocos2d::CCSize& ScrollViewSize)
{
	//object has been initialized.
	if( m_pScrollView || m_pLabel )
	{
		return false;
	}

	//constraint the label's width, but not height.
	m_pLabel = CCLabelTTF::create( pszText, fontName, fontSize, CCSizeMake( ScrollViewSize.width, 0 ), hAlignment );
	m_pLabel->retain();

	//get the texture size
	CCSize TextSize = m_pLabel->getContentSize();


	m_pScrollView = ScrollView::create( ScrollViewSize );
	m_pScrollView->retain();
	m_pScrollView->setDelegate( this );
	m_pScrollView->setDirection( ScrollView::Direction::VERTICAL );
	

	if( TextSize.height > ScrollViewSize.height )
	{
		m_pScrollView->setContentSize( TextSize );

		m_pScrollView->setContentOffset( Vec2( 0, ScrollViewSize.height - TextSize.height ) );
	}
	else
	{
		m_pScrollView->setContentSize( ScrollViewSize );
	}

	//put the label at the top of the container.
	m_pScrollView->addChild( m_pLabel );
	m_pLabel->ignoreAnchorPointForPosition( false );
	m_pLabel->setAnchorPoint( Vec2( 0, 1 ) );
	m_pLabel->setPosition( Vec2( 0, m_pScrollView->getContentSize().height ) );


	addChild( m_pScrollView );
	return true;
}

void SHZXJScrollLabel::setTouchEnabled(bool bEnabled)
{
	m_pScrollView->setTouchEnabled(bEnabled);
}