#include "SharkMenu.h"
#include "SelectRoom/SelectRoom.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "HallGameScene.h"
#include "shark/UI/RuleShow.h"
#include "shark/Module/ReturnDialog.h"

#include "LanguageConfig.h"

//Menu节点下按钮逻辑标签
#define RECHARGE	"rechargebtn"	
#define	SETTING		"setbtn"
#define	REGULAR		"rulebtn"
#define	EXIT		"returnbtn"
#define LEFT        "leftbtn"
#define RIGHT       "rightbtn"

#define     Shark_player_out    "playergoAway"
#define     RECHARGEOUT         "recharge_out"


SharkMenu::SharkMenu() :
m_recharge(nullptr),
m_setting(nullptr),
m_regular(nullptr),
m_exit(nullptr),
m_left(nullptr),
m_right(nullptr),
menu(nullptr),
flag(false)
{
	HALL_NOTIFICATION_REG(RECHARGEOUT, SharkMenu::outRecharge, nullptr);
}

SharkMenu::~SharkMenu()
{
	HALL_NOTIFICATION_UNREG(RECHARGEOUT);
}

void SharkMenu::setContent()
{
	menu = dynamic_cast<Node*>(m_pNode->getChildByName("menu"));
	CC_ASSERT(nullptr != menu);

	m_recharge = dynamic_cast<Button*>(menu->getChildByName(RECHARGE));
	CC_ASSERT(nullptr != m_recharge);
	m_recharge->setTag(101);
	m_recharge->addTouchEventListener(this, toucheventselector(SharkMenu::BtEvent));

	m_setting = dynamic_cast<Button*>(menu->getChildByName(SETTING));
	CC_ASSERT(nullptr != m_setting);
	m_setting->setTag(102);
	m_setting->addTouchEventListener(this, toucheventselector(SharkMenu::BtEvent));

	m_regular = dynamic_cast<Button*>(menu->getChildByName(REGULAR));
	CC_ASSERT(nullptr != m_regular);
	m_regular->setTag(103);
	m_regular->addTouchEventListener(this, toucheventselector(SharkMenu::BtEvent));

	m_exit = dynamic_cast<Button*>(menu->getChildByName(EXIT));
	CC_ASSERT(nullptr != m_exit);
	m_exit->setTag(104);
	m_exit->addTouchEventListener(this, toucheventselector(SharkMenu::BtEvent));

	m_left = dynamic_cast<Button*>(menu->getChildByName(LEFT));
	CC_ASSERT(nullptr != m_left);
	m_left->setTag(105);
	m_left->setVisible(false);
	m_left->addTouchEventListener(this, toucheventselector(SharkMenu::BtEvent));

	m_right = dynamic_cast<Button*>(m_pNode->getChildByName(RIGHT));
	CC_ASSERT(nullptr != m_right);
	m_right->setTag(106);
	m_right->addTouchEventListener(this, toucheventselector(SharkMenu::BtEvent));


}


void SharkMenu::BtEvent(Ref *pSender, TouchEventType type)
{
	auto bt = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(180, 180, 180));
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		bt->setColor(Color3B(255, 255, 255));
		bt->runAction(ScaleTo::create(0.05f, 1.0f));
		_PLAY_SOUND_SHARK(PRESS);
		switch (bt->getTag())
		{
		case 101:
		{
					outRecharge(nullptr);
					break;
		}
			break;
		case 102:
		{
					auto settingNode = (Node*)HallGameScene::createWindow(EWT_Setting);
					settingNode->setCameraMask((int)CameraFlag::USER1);
					settingNode->setPosition3D(Vec3(-537,-370,-640));
					settingNode->setRotation3D(Vec3(0,0,0));
					this->addChild(settingNode);
					break;
		}
			
		case 103:
		{
					auto ruleNode = RuleShow::create();
					ruleNode->setCameraMask((int)CameraFlag::USER1);
					ruleNode->setPosition3D(Vec3(-537,-370,-640));
					ruleNode->setRotation3D(Vec3(0, 0, 0));
					this->addChild(ruleNode);
					break;
		}
			
		case 104:
		{
					ReturnDialog::openReturnNotice(GETSTRING(1412), [this](Ref*){
						goBack();
					});
					
		}
			break;
		case 105:
		{
					if (flag)
					{
						moveIn();
						break;
					}
					else break;
		}
			
		case 106:
		{
					if (!flag)
					{
						moveOut();						
						break;
					}
					else break;
		}
			
		default:
			break;
		}
		break;
	case TOUCH_EVENT_CANCELED:
		bt->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//菜单收回
void SharkMenu::moveIn()
{
	flag = false;
	auto in = MoveBy::create(0.3f, Vec2(70, 0)); 
	auto call = CallFunc::create(this,callfunc_selector(SharkMenu::setRight));
	auto seq = Sequence::create(in, call, NULL);
	menu->runAction(seq);
	
}

//菜单显示
void SharkMenu::moveOut()
{
	flag = true;
	m_right->setVisible(false);
	m_left->setVisible(true);
	auto out = MoveBy::create(0.3f, Vec2(-70, 0));
	menu->runAction(out);
	
}
//设置按钮可见
void SharkMenu::setRight()
{
	m_right->setVisible(true);
	m_left->setVisible(false);
}
//返回按钮事件
void SharkMenu::goBack(int code /*= 0*/)
{
	auto gameinfo = GameInfo::GetInstance();
	//用户退出时设置用户的金币数
	int coinScore = gameinfo->getchangeScore();
	int outScore = coinScore - Player::getInstance()->getLogonData().lUserScore;
	if (outScore > 0)
	{
		Player::getInstance()->addCoin(outScore, false);
	}


	HALL_NOTIFICATION(Shark_player_out,nullptr);


	//用户信息
	SharkUserInfo info = GameInfo::GetInstance()->getuserInfor();
	//发送起立
	SharkNetWorkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
	//断开连接
	SharkNetWorkKernel::getInstance()->relese();
	//清空信息
	

	gameinfo->setstartGameFinish(false);
	gameinfo->getroomInfor()->vec.clear();


	if (code == 0)
	{
			//返回选择房间界面
			Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, SelectRoom::createScene()));		
	}
	else if (code == -1)
	{		
			
	
	}
	else
	{
		//返回大厅
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
	}
}

//弹出充值按钮
void SharkMenu::outRecharge(Ref* ref)
{
	auto m_visibleSize = Director::getInstance()->getWinSize();
	//摆放摄像机(3d动物层)
	auto ca = Camera::createPerspective(60, (GLfloat)m_visibleSize.width / m_visibleSize.height, 1, 1000);
	ca->setDepth(102);

	ca->setCameraFlag(CameraFlag::USER2);
	ca->setPosition3D(Vec3(0, 0, 0));
	ca->lookAt(Vec3(0, 0, -1));
	this->addChild(ca,1000);

	auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
	rechargeNode->setCameraMask((int)CameraFlag::USER2);
	rechargeNode->setPosition3D(Vec3(-537, -370, -640));
	rechargeNode->setRotation3D(Vec3(0, 0, 0));
	this->addChild(rechargeNode);
}
