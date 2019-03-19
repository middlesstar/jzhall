#include "PlayerListOne.h"
#include "shark/NetWork/SharkNetworkKernel.h"


//每个按钮节点下逻辑名称
#define _HEAD				"head"
#define _HEAD_BK			"head_bk"
#define	_VIP_0				"vip_0"
#define _VIP_FRONT			"vip_front"
#define _VIP_NUM			"vip_num"
#define _NAME				"name"
#define _WIN_BK				"win_bk"
#define _WIN_NUM			"win_num"
#define _SOCRE_NUM			"score_num"

#define _Head_Path			"images/shak/head/head_%d.png"
#define _Head_Path_Frame	"images/shak/vip/frame%d.png"
#define _Bet_Win			"images/shak/bet/bet_win.png"
#define _Bet_Lose			"images/shak/bet/bet_lost.png"
#define _Bet_Points			"images/shak/bet/bet_points.png"



//顺序	除了金鲨银鲨，按照按钮对应位置
Vec2 PlayerListOne::m_targetPos[12] = {
	Vec2(314, 490.5), Vec2(431, 490.5), Vec2(373, 493), Vec2(490, 490.5),
	Vec2(567, 490.5), Vec2(624.5, 490.5), Vec2(682.5, 490.5), Vec2(740.5, 490.5),
	Vec2(817, 490.5), Vec2(876, 490.5), Vec2(934, 490.5), Vec2(993, 490.5) };
PlayerListOne* PlayerListOne::m_templateThis = nullptr;
PlayerListOne::PlayerListOne() :
m_head(nullptr),										//头像
m_headBk(nullptr),										//头像边框
m_vip0(nullptr),										//vip0
m_vip(nullptr),											//vip金色
m_vipNum(nullptr),										//vip数字
m_name(nullptr),										//昵称
m_winBk(nullptr),										//赢输分数文本背景
m_winNum(nullptr),										//赢输分数文本（单位K）
m_scoreNum(nullptr) 								    //分数数字
{
}

PlayerListOne::~PlayerListOne()
{
}

void PlayerListOne::initTemplate(ImageView* node)
{
	if (m_templateThis != nullptr)
	{
		delete m_templateThis;
		m_templateThis = nullptr;
	}

	CC_ASSERT(node != nullptr);

	//m_templateThis = new Widget();
	m_templateThis = new PlayerListOne();
	//不随父节点透明
	node->setCascadeOpacityEnabled(false);
	//隐去占位图片
	node->setOpacity(_ZERO);

	m_templateThis->m_rootNode = node;

	m_templateThis->m_head = dynamic_cast<ImageView*>(node->getChildByName(_HEAD));
	CC_ASSERT(m_templateThis->m_head != nullptr);

	m_templateThis->m_headBk = dynamic_cast<ImageView*>(node->getChildByName(_HEAD_BK));
	CC_ASSERT(m_templateThis->m_headBk != nullptr);

	m_templateThis->m_vip0 = dynamic_cast<ImageView*>(node->getChildByName(_VIP_0));
	CC_ASSERT(m_templateThis->m_vip0 != nullptr);

	m_templateThis->m_vip = dynamic_cast<ImageView*>(node->getChildByName(_VIP_FRONT));
	CC_ASSERT(m_templateThis->m_vip != nullptr);

	m_templateThis->m_vipNum = dynamic_cast<TextAtlas*>(node->getChildByName(_VIP_NUM));
	CC_ASSERT(m_templateThis->m_vipNum != nullptr);

	m_templateThis->m_name = dynamic_cast<Text*>(node->getChildByName(_NAME));
	CC_ASSERT(m_templateThis->m_name != nullptr);

	m_templateThis->m_winBk = dynamic_cast<ImageView*>(node->getChildByName(_WIN_BK));
	CC_ASSERT(m_templateThis->m_winBk != nullptr);

	m_templateThis->m_winNum = dynamic_cast<Text*>(node->getChildByName(_WIN_NUM));
	CC_ASSERT(m_templateThis->m_winNum != nullptr);

	m_templateThis->m_scoreNum = dynamic_cast<TextAtlas*>(node->getChildByName(_SOCRE_NUM));
	CC_ASSERT(m_templateThis->m_scoreNum != nullptr);


}

PlayerListOne* PlayerListOne::create()
{
	if (m_templateThis == nullptr)
	{
		return nullptr;
	}

	auto This = new PlayerListOne();

	This->m_rootNode = dynamic_cast<ImageView*>(m_templateThis->m_rootNode->clone());
	CC_ASSERT(This->m_rootNode != nullptr);

	This->m_head = dynamic_cast<ImageView*>(This->m_rootNode->getChildByName(_HEAD));
	CC_ASSERT(This->m_head != nullptr);

	This->m_headBk = dynamic_cast<ImageView*>(This->m_rootNode->getChildByName(_HEAD_BK));
	CC_ASSERT(This->m_headBk != nullptr);

	This->m_vip0 = dynamic_cast<ImageView*>(This->m_rootNode->getChildByName(_VIP_0));
	CC_ASSERT(This->m_vip0 != nullptr);

	This->m_vip = dynamic_cast<ImageView*>(This->m_rootNode->getChildByName(_VIP_FRONT));
	CC_ASSERT(This->m_vip != nullptr);

	This->m_vipNum = dynamic_cast<TextAtlas*>(This->m_rootNode->getChildByName(_VIP_NUM));
	CC_ASSERT(This->m_vipNum != nullptr);

	This->m_name = dynamic_cast<Text*>(This->m_rootNode->getChildByName(_NAME));
	CC_ASSERT(This->m_name != nullptr);

	This->m_winBk = dynamic_cast<ImageView*>(This->m_rootNode->getChildByName(_WIN_BK));
	CC_ASSERT(This->m_winBk != nullptr);

	This->m_winNum = dynamic_cast<Text*>(This->m_rootNode->getChildByName(_WIN_NUM));
	CC_ASSERT(This->m_winNum != nullptr);

	This->m_scoreNum = dynamic_cast<TextAtlas*>(This->m_rootNode->getChildByName(_SOCRE_NUM));
	CC_ASSERT(This->m_scoreNum != nullptr);
	

	return This;
}

void PlayerListOne::setName(const std::string &name)
{
	m_name->setText(name);
}

void PlayerListOne::setHead(int index)
{
	char path[256];
	sprintf(path, _Head_Path, index);
	m_head->loadTexture(path, TextureResType::PLIST);
}



void PlayerListOne::GoldFly(int index)
{
	auto gold = Sprite::create(GOLD_FLY);
	CC_ASSERT(gold != nullptr);

	auto parent = m_rootNode->getParent()->getParent()->getParent()->getParent()->getParent();
	Vec2 v2 = parent->convertToWorldSpace(m_rootNode->getPosition()) + m_rootNode->getParent()->getPosition();

	if (v2.y < 10)
	{
		v2.y = 20;
		v2.x = random(-100, 100);
	}


	parent->addChild(gold);
	gold->setScale(0.5);
	gold->setPosition(v2 + Vec2(random(-10, 10), random(-10, 10)));			//移动到金币的位置

	gold->setRotation(random(-180, 180));
	//gold->setPosition(m_pNode->getPosition() + Vec2(random(-15, 15), random(-15, 15)));
	gold->runAction(RepeatForever::create(RotateBy::create(0.2f, Vec3(random(-60, 60), random(-15, 15), random(-360, 360)))));
	gold->runAction(CCSequence::create(
		MoveTo::create(0.35f, m_targetPos[index]),									//CCEaseExponentialInOut::create(
		CCCallFuncN::create(this, callfuncN_selector(PlayerListOne::CallBack)),
		NULL));

}

void PlayerListOne::CallBack(Node* node)
{
	//销毁
	node->removeFromParent();
}

void PlayerListOne::setVipOrHradBk(int vip)
{
	//CC_ASSERT(vip >= 0 && vip <= 10);

	if (vip == _ZERO)
	{
		m_vip0->setVisible(true);
		m_vip->setVisible(false);
		m_vipNum->setVisible(false);
	}
	else
	{
		m_vip0->setVisible(false);
		m_vip->setVisible(true);
		m_vipNum->setVisible(true);
		std::stringstream str;
		str << vip;
		m_vipNum->setString(str.str());
	}

	//VIP框子图片不够10张 暂且这么设定
	int vipFrame = vip;
	if (vipFrame >= 7)
	{
		vipFrame = 7;
	}

	//加载炫酷头像框
	char path[256];
	sprintf(path, _Head_Path_Frame, vipFrame);
	m_headBk->loadTexture(path, TextureResType::PLIST);

}

void PlayerListOne::setWinScore(SCORE score, int type)
{
	if (type == 0)
	{
		m_winBk->loadTexture(_Bet_Points, TextureResType::PLIST);
	}
	else
	{
		if (score >= _ZERO)
		{
			//赢
			m_winBk->loadTexture(_Bet_Win, TextureResType::PLIST);

		}
		else
		{
			//输
			m_winBk->loadTexture(_Bet_Lose, TextureResType::PLIST);
		}
	}

	char num[30];
	SCORE userscore = abs(score);
	log("-------------userScore= %d", userscore);
	if (userscore > 1000)
	{
		auto gameInfo = GameInfo::GetInstance();
		int roomNum = gameInfo->getroomIndex();
		if (roomNum == 2)
		{
			sprintf(num, "%ldk", userscore / 1000);
		}
		else if (roomNum == 1)
		{
			sprintf(num, "%.1fK", 1.f * userscore / 1000);
		}
		else if (roomNum == 0)
		{
			sprintf(num, "%.2fK", 1.f * userscore / 1000);
		}
	}
	else
	{
		sprintf(num, "%ld", userscore);
	}
	
	m_winNum->setText(num);


}

void PlayerListOne::setScore(SCORE score)
{
	std::stringstream str;
	str << score;
	m_scoreNum->setString(str.str());
}

void PlayerListOne::setuserinfo(const UserInfo_Shark & info)
{
	memcpy(&m_userInfo, &info, sizeof(UserInfo_Shark));


	setHead(info.headimgid);
	setScore(info.gold);
	setVipOrHradBk(info.viplevel);
	setName(info.nickname);
	//压分阶段
	if (GameInfo::GetInstance()->getserverState()==STAKE)
	{
		setWinScore(info.jet, 0);
	}
	else
	{
		setWinScore(info.winscore, 1);
	}
	
}
