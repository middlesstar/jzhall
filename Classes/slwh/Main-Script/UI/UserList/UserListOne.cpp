#include "UserListOne.h"
#include "slwh/Main-Script/UI/UIUserInfo.h"
#include "slwh/Network/NetworkKernel.h"


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



Vec2 UserListOne::m_sTarPos[15] = {
	Vec2(922, 493), Vec2(773, 493), Vec2(625, 493), Vec2(477, 493),
	Vec2(971, 493), Vec2(823, 493), Vec2(672, 493), Vec2(527, 493),
	Vec2(1021, 493), Vec2(872, 493), Vec2(724, 493), Vec2(576, 493),
	Vec2(328, 493), Vec2(378, 493), Vec2(427, 493) };
UserListOne* UserListOne::m_templateThis = nullptr;
UserListOne::UserListOne() :
m_head(nullptr),										//头像
m_headBk(nullptr),										//头像边框
m_vip0(nullptr),										//vip0
m_vip(nullptr),											//vip金色
m_vipNum(nullptr),										//vip数字
m_name(nullptr),										//昵称
m_winBk(nullptr),										//赢输分数文本背景
m_winNum(nullptr),										//赢输分数文本（单位K）
m_scoreNum(nullptr)										//分数数字
{
}

UserListOne::~UserListOne()
{
}

void UserListOne::initTemplate(ImageView* node)
{
	if (m_templateThis != nullptr)
	{
		delete m_templateThis;
		m_templateThis = nullptr;
	}

	CC_ASSERT(node != nullptr);

	m_templateThis = new UserListOne();
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

UserListOne* UserListOne::create()
{
	if (m_templateThis == nullptr)
	{
		return nullptr;
	}

	auto This = new UserListOne();

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

void UserListOne::setName(const std::string &name)
{
	m_name->setString(name);
}

void UserListOne::setHead(int index)
{
	char path[256];
	sprintf(path, _Head_Path, index);
	m_head->loadTexture(path, TextureResType::PLIST);
}

void UserListOne::GoldFlyStartPlay(int tag)
{
	auto gold = Sprite::create(_Gold3_Fly);
	CC_ASSERT(gold != nullptr);

	auto parent = m_rootNode->getParent()->getParent()->getParent()->getParent();
	Vec2 v2 = parent->convertToWorldSpace(m_rootNode->getPosition()) + m_rootNode->getParent()->getPosition();
	//在下面的 全部从最底部飞过去
	if (v2.y < 10) v2 = Vec2(random(-100,100),20);
	parent->addChild(gold);
	gold->setScale(0.5);			//这样好看
	gold->setPosition(v2 + Vec2(random(-10, 10), random(-10, 10)));			//移动到金币的位置
	gold->setRotation(random(-180, 180));

	gold->runAction(RepeatForever::create(RotateBy::create(0.2f, Vec3(random(-60, 60), random(-15, 15), random(-360, 360)))));
	gold->runAction(CCSequence::create(
		MoveTo::create(0.3f, m_sTarPos[tag]),									//CCEaseExponentialInOut::create(
		CCCallFuncN::create([this](Node* n){n->removeFromParent(); }),
		NULL));
}

void UserListOne::setVipOrHradBk(int vip)
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

void UserListOne::setWinScore(SCORE score, int type)
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
	if (userscore > 1000)
	{

		slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
		switch (info->wType)
		{
		case 0:
			sprintf(num, "%.2fK", 1.f * userscore / 1000);
			break;
		case 1:
			sprintf(num, "%.1fK", 1.f * userscore / 1000);
			break;
		case 2:
			sprintf(num, "%.0fK", 1.f * userscore / 1000);
			break;
		default:
			break;
		}

	}
	else
	{
		sprintf(num, "%lld", userscore);
	}
	m_winNum->setString(num);
}

void UserListOne::setScore(SCORE score)
{
	std::stringstream str;
	str << score;
	m_scoreNum->setString(str.str());
}

void UserListOne::setuserinfo(const slwhPlayerInfo & info)
{
	memcpy(&m_userInfo, &info, sizeof(slwhPlayerInfo));

	setHead(info.headimgid);
	setScore(info.gold);
	setVipOrHradBk(info.viplevel);
	setName(info.nickname);
	//没有进入算分阶段
	if (OneGameInfo::GetInstance()->getbCountScore() == false)
	{
		setWinScore(info.jet, 0);
	}
	else
	{
		setWinScore(info.winscore, 1);
	}

}
