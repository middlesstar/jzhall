#include "OneOfStake.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "shark/sharkStruct.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
#include "HallGameScene.h"

USING_NS_CC;

#define     RECHARGEOUT         "recharge_out"

#define     TEXTBIT      "text"          //����
#define     TEXTSCORE    "textScore"     //Ѻ��
#define     CIRCLEYUN    "bet_light"     //��ʾѺ�ֹ�Ȧ
#define     RING         "ring"          //���������ת��Ȧ

//˳��	���˽������裬���հ�ť��Ӧλ��
Vec2 OneOfStake::m_targetPos[12] = {
	Vec2(61, 490.5), Vec2(178, 490.5), Vec2(120, 493), Vec2(237, 490.5),
	Vec2(314, 490.5), Vec2(371.5, 490.5), Vec2(429.5, 490.5), Vec2(487.5, 490.5),
	Vec2(564, 490.5), Vec2(623, 490.5), Vec2(681, 490.5), Vec2(740, 490.5) };


//��ʼ��ѹ��
int OneOfStake::stakeAdd = _ZERO;
SCORE OneOfStake::MyAllTotel = _ZERO;
OneOfStake::OneOfStake()
:m_stake(_ZERO)
,m_beilvNum(nullptr)
,m_mystakeNum(nullptr)
, m_ring(nullptr)
, m_circle(nullptr)
, stakeFlag(true)
{

}
OneOfStake::~OneOfStake()
{

}
void OneOfStake::setContent()
{
	m_beilvNum = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode,TEXTBIT));
	CC_ASSERT(NULL != m_beilvNum);

	m_mystakeNum = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, TEXTSCORE));
	CC_ASSERT(NULL != m_mystakeNum);

	m_ring = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, RING));
	CC_ASSERT(NULL != m_ring);
	m_ring->setVisible(false);

	m_circle = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, CIRCLEYUN));	
	CC_ASSERT(NULL != m_circle);
	m_circle->setVisible(false);

	
	//��ʼ������
	SetBeiLv(_ZERO);
	SetMyStake(_ZERO);
}



void OneOfStake::BtEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		//((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		//ѹ��
		AddStake();

		_PLAY_SOUND_SHARK(PRESS);
		break;
	case TOUCH_EVENT_CANCELED:
		//((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}
}

//��ʾ��ת�Ĺ�Ȧ
void OneOfStake::showRing()
{
	m_ring->setVisible(true);
}
//��ʾ��ť�ϵĹ�Ȧ
void OneOfStake::showCircle()
{
	m_circle->setVisible(true);

	auto in = FadeOut::create(0.3);
	auto seq = Sequence::create(in, in->reverse(), NULL);
	auto rep = Repeat::create(seq, 3);
	auto call = CallFunc::create(this, callfunc_selector(OneOfStake::enlargeNum));
	auto sequ = Sequence::create(rep,CallFunc::create([this](){this->m_circle->setVisible(false); }),call, NULL);
	m_circle->runAction(sequ);

}

//�Ŵ�����
void OneOfStake::enlargeNum()
{
	//m_beilvNum
	auto big = ScaleTo::create(0.3, 1.5f);
	auto normal = ScaleTo::create(0.3, 1.0f);
	auto seq = Sequence::create(big,normal, NULL);
	auto rep = Repeat::create(seq, 3);
	m_beilvNum->runAction(rep);
}
//���ñ��ʽӿ�
void OneOfStake::SetBeiLv(int g)
{
	m_beilvNum->setString(StringUtils::format("%d",g));
}

//�����ҵ�ѹ��
void OneOfStake::SetMyStake(SCORE g)
{
	m_stake = g;
	m_mystakeNum->setString(StringUtils::format("%d",g));
}
//����Ѻ�ְ�ť��־λ
void OneOfStake::resetBtn()
{
	stakeFlag = true;
}

//ѹ��
void OneOfStake::AddStake()
{
	if (!stakeFlag) return;
	
	StakeInfo jet = GameInfo::GetInstance()->getroomInfor()->stakeinfo;
	if (m_stake >= jet.dwOneLimit) return;

	auto info = Player::getInstance();
	SCORE playerCoin = info->getLogonData().lUserScore;
	//������ѹ���ٷ�
	int dwtargetAdd = stakeAdd;
	if (m_stake + dwtargetAdd > jet.dwOneLimit)
	{
		//����ѹ�ͱ��� ����پ��϶���
		dwtargetAdd = jet.dwOneLimit - m_stake;
	}
	
	//��¼������Ѻ��ֵ
	//m_stake += dwtargetAdd;
	if (info->getLogonData().lUserScore >= dwtargetAdd)
	{
		//�������ݸ������
		SharkNetWorkKernel::getInstance()->SendJetInfo(m_tag, dwtargetAdd);
		stakeFlag = false;
	}
	else
	{
		SharkNotice::openNotice(GETSTRING(1409), [this](Ref*)
		{		
			HALL_NOTIFICATION(RECHARGEOUT, nullptr);
		});
	}

}

int OneOfStake::SetAddJet(int code)
{
	StakeInfo stake = GameInfo::GetInstance()->getroomInfor()->stakeinfo;
	//��ȡ������Ѻ������

	if (code != _ZERO)
	{
		stakeAdd = stake.dwStake[0];
	}
	else
	{
		log("-------------stake.index = %d--------------", stake.index);
		if (++stake.index >= 5)
		{
			stake.index = 0;
		}
		stakeAdd = stake.dwStake[stake.index];
	}

	GameInfo::GetInstance()->getroomInfor()->stakeinfo = stake;
	return stakeAdd;

}


//����������ѹ�ֳɹ� ˢ��ѹ��
void OneOfStake::UpdateStake(SCORE dwtargetAdd)
{
	//SetMyStake(m_stake);
	SetMyStake(m_stake += dwtargetAdd);
	MyAllTotel += dwtargetAdd;
	//��ҷ�
	GoldFly();
	//������Ч
	btnEffect();
	Player::getInstance()->addCoin(-(int)dwtargetAdd,false);
	stakeFlag = true;
}


//��ť��Ч
void OneOfStake::btnEffect()
{
	auto particle = ParticleSystemQuad::create(PATH_BTNEFFECT);
	particle->setPosition(Vec2(42 + random(-15, 15), 42 + random(-15, 15)));
	particle->setScale(0.2f);
	particle->setOpacity(0.5f);
	particle->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
	particle->setDuration(0.5f);										// ��������ϵͳ�ĳ���ʱ����  
	m_pNode->addChild(particle);
}


//������Ҷ�
void OneOfStake::GoldFly()
{
	auto gold = Sprite::create(GOLD_FLY);
	CC_ASSERT(gold != nullptr);
	int index = 0;
	if (m_tag == 1000)
	{
		index = 0;
	}
	else if (m_tag == 2000)
	{
		index = 3;
	}
	else
	{
		index = m_tag;
	}
	auto parent = m_pNode->getParent();
	parent->addChild(gold);
	gold->setScale(0.5);
	gold->setPosition(m_pNode->getPosition() + Vec2(random(-15, 15), random(-15, 15)));
	gold->runAction(RepeatForever::create(RotateBy::create(0.2f, Vec3(random(-60, 60), random(-15, 15), random(-360, 360)))));
	gold->runAction(CCSequence::create(
		MoveTo::create(0.35f, m_targetPos[index]),									//CCEaseExponentialInOut::create(
		CCCallFuncN::create(this, callfuncN_selector(OneOfStake::CallBack)),
		NULL));
}
//�ص�����
void OneOfStake::CallBack(Node* node)
{
	//����
	node->removeFromParent();
}