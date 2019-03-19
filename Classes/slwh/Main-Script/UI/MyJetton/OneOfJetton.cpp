#include "OneOfJetton.h"
#include "slwh/Main-Script/UI/UIUserInfo.h"
#include "slwh/Network/NetworkKernel.h"
#include "slwh/Main-Script/UI/MyJetton/MyJettonManager.h"
#include "slwh/Module/SLWHNotice.h"
#include "LanguageConfig.h"
#include "HallGameScene.h"

#define _Gold3_Fly		"images/common/animation/gold3/gold3.png"

//每个按钮节点下逻辑名称
#define BEILV		"NumBk"
#define MYJETTON	"NumBk0"

#define _NUM		"Num"
//初始化压分
int OneOfJetton::JetAdd = _ZERO;
SCORE OneOfJetton::MyAllTotel = _ZERO;
OneOfJetton::OneOfJetton() :
m_jet(_ZERO),
m_beilvNum(NULL),											
m_myJetNum(NULL),
BIsCanJet(true)
//m_allJetNum(NULL)
{
}

//顺序	(红） 动物1~ 4  (绿)动物 1~4 ...	ZHX  金币飞行目标位置
Vec2 OneOfJetton::m_sTarPos[15] = { 
	Vec2(646, 493), Vec2(497, 493), Vec2(349, 493), Vec2(201, 493),
	Vec2(695, 493), Vec2(547, 493), Vec2(398, 493), Vec2(250, 493), 
	Vec2(745, 493), Vec2(596, 493), Vec2(448, 493), Vec2(300, 493), 
	Vec2(52, 493), Vec2(102, 493), Vec2(151, 493) };
OneOfJetton::~OneOfJetton()
{
}

void OneOfJetton::Start()
{

	m_beilvNum = dynamic_cast<TextAtlas*>(m_node->getChildByName(BEILV)->getChildByName(_NUM));
	CC_ASSERT(NULL != m_beilvNum);

	m_myJetNum = dynamic_cast<TextAtlas*>(m_node->getChildByName(MYJETTON)->getChildByName(_NUM));
	CC_ASSERT(NULL != m_myJetNum);

	/*m_allJetNum = dynamic_cast<TextAtlas*>(m_node->getChildByName(ALLJETTON));
	CC_ASSERT(NULL != m_allJetNum);*/

	//初始化数据
	SetBeiLv(_ZERO);
	SetMyJetton(_ZERO);

}

void OneOfJetton::BtEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		((Node*)pSender)->setColor(Color3B(_GRAY, _GRAY, _GRAY));

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
	
		//压分
		AddJet();
		_PLAY_MUSCI_EFF(_BETCOIN);
		break;
	case TOUCH_EVENT_CANCELED:
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}
}


//设置倍率接口
void OneOfJetton::SetBeiLv(int g)
{
	std::stringstream str;
	str << g;
	m_beilvNum->setString(str.str());
}

//设置我的压分
void OneOfJetton::SetMyJetton(SCORE g)
{
	m_jet = g;
	std::stringstream str;
	str << g;
	m_myJetNum->setString(str.str());
}

//设置总压分接口
//void OneOfJetton::SetAllJetton(int g)
//{
//	stringstream str;
//	str << g;
//	m_allJetNum->setString(str.str());
//}

void OneOfJetton::UpdateJet(SCORE dwtargetAdd)
{
	SetMyJetton((SCORE)m_jet + dwtargetAdd);
	MyAllTotel += dwtargetAdd;
	GoldFlyStartPlay();
	//播放特效
	PopStarPlay();
	Player::getInstance()->addCoin(-(int)dwtargetAdd,false);
}

//压分
void OneOfJetton::AddJet()
{
	//不可以压分
	if (!BIsCanJet) return;

	slwhJetInfo jet = OneGameInfo::GetInstance()->getroomInfo()->jetinfo;
	if (m_jet >= (int)jet.dwOneLimit) return;
	
	auto info = Player::getInstance();
	//还可以压多少分
	dword dwtargetAdd = JetAdd;
	if (m_jet + dwtargetAdd > jet.dwOneLimit)
	{
		//继续压就爆了 差多少就上多少
		dwtargetAdd = jet.dwOneLimit - m_jet;
	}

	if (info->getLogonData().lUserScore >= dwtargetAdd)
	{
		//发送数据给服务端
		NetworkKernel::getInstance()->SendJetInfo(m_tag, dwtargetAdd);
		BIsCanJet = false;
	}
	else
	{
		SLWHNotice::openNotice(GETSTRING(1409), [this](Ref*)
		{
			auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
			rechargeNode->setPosition(Vec2(0, 0));
			Director::getInstance()->getRunningScene()->addChild(rechargeNode, 100);

			//HALL_NOTIFICATION("recharge_out", nullptr);
		});
	}
}

//按钮特效
void OneOfJetton::PopStarPlay()
{
	auto particle = ParticleSystemQuad::create("images/common/Eff/yanhua.plist");
	particle->setPosition(Vec2(42 + random(-15,15),42 + random(-15,15)));
	particle->setScale(0.2f);
	particle->setOpacity(0.5f);
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(0.5f);									// 设置粒子系统的持续时间秒  
	m_node->addChild(particle);


}

int OneOfJetton::SetAddJet(int code /*= 0*/)
{
	slwhJetInfo jet = OneGameInfo::GetInstance()->getroomInfo()->jetinfo;
	if (code != _ZERO)
	{
		JetAdd = jet.dwJet[0];
	}
	else
	{
		if (++jet.index >= 5)
		{
			jet.index = 0;
		}

		JetAdd = jet.dwJet[jet.index];
	}
	OneGameInfo::GetInstance()->getroomInfo()->jetinfo = jet;
	return JetAdd;
}

void OneOfJetton::GoldFlyStartPlay()
{
	auto gold = Sprite::create(_Gold3_Fly);
	CC_ASSERT(gold != nullptr);

	
	auto parent = m_node->getParent();
	parent->addChild(gold);
	gold->setScale(0.5);
	gold->setPosition(m_node->getPosition() + Vec2(random(-15, 15), random(-15, 15)));
	gold->runAction(RepeatForever::create(RotateBy::create(0.2f, Vec3(random(-60, 60), random(-15, 15), random(-360, 360)))));
	gold->runAction(CCSequence::create(
		MoveTo::create(0.35f, m_sTarPos[m_tag]),									//CCEaseExponentialInOut::create(
		CCCallFuncN::create([this](Node* n){n->removeFromParent(); }),
		NULL));
}




