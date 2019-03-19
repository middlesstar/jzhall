#include "OneOfResult.h"


#define     TEXTBIT      "text"          //����
#define     TEXTSCORE    "textScore"     //Ѻ��   
#define     LABELBLACK   "bet_black"     //��ɫ����
#define     LABELWHITE   "bet_white"     //��ɫ����
#define     WINSCORE     "winScore"      //��ɫѺ��
#define     CIRCLE       "circle"

#define     COLORCIRCLE  "images/shak/result/result_fram1.png"
#define     NORMALCIRCLE  "images/shak/result/result_fram0.png"



OneOfResult::OneOfResult()
:m_beilvNum(nullptr)
, m_mystakeNum(nullptr)
, winNum(nullptr)
, blackLabel(nullptr)
, whiteLabel(nullptr)
, blinkCircle(nullptr)
{

}
OneOfResult::~OneOfResult()
{

}
void OneOfResult::setContent()
{
	m_beilvNum = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, TEXTBIT));
	CC_ASSERT(NULL != m_beilvNum);

	m_mystakeNum = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, TEXTSCORE));
	CC_ASSERT(NULL != m_mystakeNum);

	winNum = dynamic_cast<Text*>(_seekNodeByName(m_pNode, WINSCORE));
	CC_ASSERT(NULL != winNum);

	blackLabel = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, LABELBLACK));
	CC_ASSERT(NULL != blackLabel);
	blackLabel->setVisible(false);

	whiteLabel = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, LABELWHITE));
	CC_ASSERT(NULL != whiteLabel);
	whiteLabel->setVisible(false);

	blinkCircle = dynamic_cast<ImageView*>(_seekNodeByName(m_pNode, CIRCLE));
	CC_ASSERT(NULL != blinkCircle);
	blinkCircle->setVisible(false);
	
}

//���ñ��ʽӿ�
void OneOfResult::SetBeiLv(int g)
{
	m_beilvNum->setString(StringUtils::format("%d", g));
}

//��ʾ��Ѻ��
void OneOfResult::showStake(int score)
{
	blackLabel->setVisible(true);
	m_mystakeNum->setString(StringUtils::format("%d",score));
}

void OneOfResult::showWin(int score)
{
	whiteLabel->setVisible(true);

	char num[30];
	if (score > 1000)
	{
		auto gameInfo = GameInfo::GetInstance();
		int roomNum = gameInfo->getroomIndex();
		if (roomNum == 2)
		{
			sprintf(num, "%dk", score / 1000);
		}
		else if (roomNum == 1)
		{
			sprintf(num, "%.1fK", 1.f * score / 1000);
		}
		else if (roomNum == 0)
		{
			sprintf(num, "%.2fK", 1.f * score / 1000);
		}
	}
	else
	{
		sprintf(num, "%d", score);
	}
	winNum->setString(num);
}


//��ʾ��ɫ��Ȧ
void OneOfResult::showColor()
{
	blinkCircle->loadTexture(COLORCIRCLE, TextureResType::PLIST);
}

void OneOfResult::showNormal()
{
	blinkCircle->loadTexture(NORMALCIRCLE, TextureResType::PLIST);
}

//��Ȧ��ת
void OneOfResult::runCircle()
{
	blinkCircle->setVisible(true);
	blinkCircle->setScale(2.0f);
	auto rota = RotateBy::create(1.0f,360);
	auto scale = ScaleTo::create(1.0f, 1);
	auto sound = CallFunc::create(this, callfunc_selector(OneOfResult::playCircleSong));
	auto spa = Spawn::create(rota, scale,sound, NULL);
	blinkCircle->runAction(spa);

	auto rep = RepeatForever::create(rota);
	blinkCircle->runAction(rep);
	
}

//���Ź�Ȧ��С�����Ч
void OneOfResult::playCircleSong()
{
	_PLAY_SOUND_SHARK(GO_MUSIC);
}

//ֹͣ���еĶ���
void OneOfResult::stopAll()
{
	//todosl
	showNormal();
	blackLabel->setVisible(false);
	whiteLabel->setVisible(false);
	blinkCircle->stopAllActions();
	blinkCircle->setVisible(false);
	whiteLabel->getParent()->setVisible(false);
}

void OneOfResult::BtEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:

		break;
	case TOUCH_EVENT_CANCELED:

		break;
	default:
		break;
	}
}


//���ñ��ֿ����İ�ť
void OneOfResult::showWinBtn(bool flag,int score)
{
	if (flag)
	{		
		showColor();
		showWin(score);
		whiteLabel->getParent()->setVisible(true);
	}
	else
	{
		showStake(score);
		blackLabel->getParent()->setVisible(true);
	}
	runCircle();
}