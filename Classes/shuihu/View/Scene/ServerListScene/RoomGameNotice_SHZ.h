#ifndef SHZ_RoomGameNotice_H__
#define SHZ_RoomGameNotice_H__

#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"


class SHZCRoomGameNotice :public cocos2d::CCLayer
	, public SHZIGPGameNoticeMissionSink
{
public:

	cocos2d::CCSprite*			m_bg;

	SHZTailorView*					m_TailorView;

public:
	CREATE_FUNC(SHZCRoomGameNotice);

public:
	SHZCRoomGameNotice(void);
	~SHZCRoomGameNotice(void);

	bool init();

	virtual void onGPGameNoticeResult(const char* szDescription);

	void RollText(cocos2d::CCNode* node, void* data);

private:
	int vipColorIndex(const char* strMsg);

private:
	SHZCGPGameNoticeMission	mGameNoticeMission;

	int						m_iCount;

private:
	bool					m_isAnnouncing;		// 是否正在通知
	cocos2d::CCArray*		m_noticeArras;		// 消息通知数组
};

#endif

