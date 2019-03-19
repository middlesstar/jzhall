#include "Battle.h"
#include "HallGameScene.h"
#include "shark/UI/GameScene/MainView.h"





#define     GENERALANIMAL          "genelBattle"
#define     TOPLEFTSHARK           "gs_silver3"
#define     TOPRIGHTSHARK          "gs_silver1"
#define     DOWNLEFTSHARK          "gs_silver2"
#define     DOWNRIGHTSHARK         "color_silver"
#define     MIDDLELEFTSHARK        "goldShark"
#define     MIDDLERIGHTSHARK       "goldShark_0"
#define     COLORSHARKTOP          "color_gold"
#define     COLORSHARKDOWN         "goldSharkDown"

#define     YUANBAO                "images/shak/game_scence/gs_yuanbao.png"
#define     TICKETS                "images/shak/game_scence/gs_tickets.png"
#define     HANDSEL                "images/shak/game_scence/gs_handsel.png"

#define     GOLDSHARK              "images/shak/game_scence/gs_gold.png"
#define     SILVERSHARK            "images/shak/game_scence/gs_silver.png"


#define     SCREEN                 "colorGold_screen"
#define     MONEYLABEL             "colorMoney_lable"


#define    RECHARGEBTN             "btn_recharge"
#define    RECHARGELIGHT           "rechargeLight"
//����ֹͣ��λ�����ü�¼Ȧ����λ��
const int outPos[28] = { 20, 21, 22, 23, 24, 25, 26, 27, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
const int outPos2[28] = { 19,20, 21, 22, 23, 24, 25, 26, 27, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
const int outPos3[28] = { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };


Battle::Battle()
: pMainNode(nullptr)
, topColor_shark(nullptr)
, downColor_shark(nullptr)
, leftTop_shark(nullptr)
, rightTop_shark(nullptr)
, rightDown_shark(nullptr)
, leftDown_shark(nullptr)
, leftMiddle_shark(nullptr)
, rightMiddle_shark(nullptr)
, screen(nullptr)
, colorMoneyNum(nullptr)
, btn_recharge(nullptr)
, rechargeLight(nullptr)
{
	num = 0;
	pos = 0;
	limitNum = 0;
	moveNum = 0;
	closeNum = 0;
	limitSpeed = 0.0f;
	stopPos = 0;
	circleFlag = 0;
	rotationPos = 0;
}
Battle::~Battle()
{

}

void Battle::setContent()
{
	pMainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, GENERALANIMAL));
	CC_ASSERT(NULL != pMainNode);

	leftTop_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, TOPLEFTSHARK));
	CC_ASSERT(NULL != leftTop_shark);
	rightTop_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, TOPRIGHTSHARK));
	CC_ASSERT(NULL != rightTop_shark);
	leftDown_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, DOWNLEFTSHARK));
	CC_ASSERT(NULL != leftDown_shark);
	rightDown_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, DOWNRIGHTSHARK));
	CC_ASSERT(NULL != rightDown_shark);
	leftMiddle_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, MIDDLELEFTSHARK));
	CC_ASSERT(NULL != leftMiddle_shark);
	rightMiddle_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, MIDDLERIGHTSHARK));
	CC_ASSERT(NULL != rightMiddle_shark);
	topColor_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, COLORSHARKTOP));
	CC_ASSERT(NULL != topColor_shark);
	downColor_shark = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, COLORSHARKDOWN));
	CC_ASSERT(NULL != downColor_shark);

	btn_recharge = dynamic_cast<Button*>(_seekNodeByName(pMainNode, RECHARGEBTN));
	CC_ASSERT(NULL != btn_recharge);
	btn_recharge->addTouchEventListener(this, toucheventselector(Battle::Recharge));

	rechargeLight = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, RECHARGELIGHT));
	CC_ASSERT(NULL!=rechargeLight);
	

	//��ʾ�ʽ��	
	screen = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, SCREEN));
	CC_ASSERT(NULL != screen);
	//�ʽ��ǩ
	colorMoneyNum = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, MONEYLABEL));
	CC_ASSERT(NULL != colorMoneyNum);
	//���òʽ𴰿���ʾ�Ĳʽ���
	colorMoneyNum->setString("0");

	char name[50];
	for (int i = 1; i < 29; i++)
	{
		sprintf(name, "circle%d", i);
		auto block = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, name));
		block->setVisible(false);
		circle.pushBack(block);
	}
	
	moveRecharge();
	
}

void Battle::Recharge(Ref *pSender, TouchEventType type)
{
	auto bt = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(180, 180, 180));
		bt->runAction(ScaleTo::create(0.05f, 1.1f));
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
							  _PLAY_SOUND_SHARK(PRESS);
							  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
							  rechargeNode->setCameraMask((int)CameraFlag::USER1);
							  rechargeNode->setPosition3D(Vec3(-537, -370, -640));
							  rechargeNode->setRotation3D(Vec3(0, 0, 0));
							  this->addChild(rechargeNode);
							  break;
	}
		
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.05f, 1.f));
		bt->setColor(Color3B(255, 255, 255));
	default:
		break;
	}

	
		
}



//��ֵ���䶯��
void Battle::moveRecharge()
{
	if (!btn_recharge || !rechargeLight)
	{
		return;
	}
	btn_recharge->stopAllActions();
	rechargeLight->stopAllActions();

	auto rotateAction = Sequence::create(RotateTo::create(0.06f, 20.0f), RotateTo::create(0.12f, -20.0f),
		RotateTo::create(0.09f, 10.0f), RotateTo::create(0.06f, -10.0f), RotateTo::create(0.03, 0), NULL);
	auto scaleAction = Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), NULL);
	btn_recharge->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.0f), EaseSineInOut::create(rotateAction), scaleAction, NULL)));

	rechargeLight->runAction(RepeatForever::create(RotateBy::create(0.1f, 1.f)));
}
//���òʽ����ֵ
void Battle::setColorMoney(int money)
{
	colorMoneyNum->setString(StringUtils::format("%d", money));
}


//���ù�Ȧ��ʼ��ֹͣ��λ��
void Battle::setCirclePos(int beginPos, int endPos)
{
	lvIndex = abs(rand()) % 3;
	//lvIndex = 1;
	log("------------------lvIndex = %d------------",lvIndex);
	
	//�˴����ù�Ȧ��ʼ�ܶ���λ��
	pos = beginPos;
	limitNum = pos;
	//�˴��жϹ�Ȧ�Ƿ�תһȦ�ı�׼(�˴�stopPos�����ּ�Ϊ������Ҫֹͣ��λ��)
	stopPos = endPos;
	if (lvIndex == 0)
	{
		circleFlag = outPos[stopPos];		
	}
	else if (lvIndex == 1)
	{
		circleFlag = outPos2[stopPos];
	}
	else if (lvIndex == 2)
	{
		circleFlag = outPos3[stopPos];
	}
	
}

//������ת
void Battle::startPlay()
{
	circle.at(pos)->setVisible(true);
	this->schedule(schedule_selector(Battle::lightCircle), 0.5f);
	
}
//�ر���ת
void Battle::stopPlay()
{
	circle.at(pos)->setVisible(false);
}
//������Ȧ
void Battle::lightCircle(float t)
{
	num++;
	if (6 == num)
	{
		num = 0;
		//�رյ�����Ȧ
		this->unschedule(schedule_selector(Battle::lightCircle));
		//�ƶ���Ȧ
		this->schedule(schedule_selector(Battle::moveCircle));
	}
	else
	{
		if (27 == pos)
		{
			pos = 0;
		}
		else
		{
			pos++;
		}
		circle.at(pos)->setVisible(true);
		int speedLi = 0.5 - 0.07*num;
		this->unschedule(schedule_selector(Battle::lightCircle));
		this->schedule(schedule_selector(Battle::lightCircle),speedLi);
	}

}

//���ƹ�Ȧ������
void Battle::controlCircle(int repeat)
{
	if (27 == pos)
	{
		pos = 0;
	}
	else
	{
		pos++;
	}
	circle.at(pos)->setVisible(true);
	for (int i = 0; i < repeat; i++)
	{
		circle.at(limitNum)->setVisible(false);
		if (27 == limitNum)
		{
			limitNum = 0;
		}
		else
		{
			limitNum++;
		}
	}

}

//�ƶ���Ȧ
void Battle::moveCircle(float t)
{
	if (circleFlag == pos)
	{
		moveNum++;		
	}

	if (moveNum == 7)
	{
		moveNum = 0;
		this->unschedule(schedule_selector(Battle::moveCircle));
		this->schedule(schedule_selector(Battle::reduceCircle));
	}
	else
	{
		this->controlCircle(1);		
	}
}

//���ٹ�Ȧ
void Battle::reduceCircle(float t)
{
	limitSpeed += 0.1;
	if (0.6f == limitSpeed)
	{
		limitSpeed = 0;
		this->unschedule(schedule_selector(Battle::reduceCircle));
		this->schedule(schedule_selector(Battle::meanCircle));
	}
	else
	{	
		this->controlCircle(2);
		_PLAY_SOUND_SHARK(LASTMOVE);
		this->unschedule(schedule_selector(Battle::reduceCircle));
		this->schedule(schedule_selector(Battle::reduceCircle), limitSpeed);

	}
	
}

//����ֹͣ��Ȧ
void Battle::meanCircle(float t)
{	
	int stopControl = 0;
	if (lvIndex == 0)
	{
		stopControl = 3;
	}
	else if (lvIndex == 1)
	{
		stopControl = 4;
	}
	else if (lvIndex == 2)
	{
		stopControl = 5;
	}
	if (stopControl == closeNum)
	{
		closeNum = 0;
		this->unschedule(schedule_selector(Battle::meanCircle));
		//���һ����Ч
		_PLAY_SOUND_SHARK(ONE_MUSIC);
		//��ըЧ��
		lightBoast(pos);
		//����Ч��
		auto seq = Sequence::create(DelayTime::create(1.0f), CallFunc::create([this](){
			auto blink = Blink::create(2, 6);
			circle.at(pos)->runAction(blink);  }),NULL);
			this->runAction(seq);

	}
	else
	{
		this->controlCircle(1);
		_PLAY_SOUND_SHARK(LASTMOVE);
		closeNum++;
		this->unschedule(schedule_selector(Battle::meanCircle));
		this->schedule(schedule_selector(Battle::meanCircle),0.6);		
	}
	
}

//��ת��Ȧ
void Battle::rotation()
{
	this->schedule(schedule_selector(Battle::back), 0.05f);
}
//���ƹ�Ȧ��תֹͣ��λ��
void Battle::setRotationPos(int index)
{
	rotationPos = index;
}
//����
void Battle::back(float t)
{
	circle.at(pos)->setVisible(false);
	if (pos == 0)
	{
		pos = 27;
	}
	else
	{
		pos--;
	}
	circle.at(pos)->setVisible(true);
	
	//���ƻ�ת��Ȧֹͣ��λ��

	if (pos == rotationPos)
	{
		this->unschedule(schedule_selector(Battle::back));

		_PLAY_SOUND_SHARK(ONE_MUSIC);
		lightBoast(pos);
		auto seq = Sequence::create(DelayTime::create(1.0f), CallFunc::create([this](){
			auto blink = Blink::create(2, 6);
			circle.at(pos)->runAction(blink);  }), NULL);
			this->runAction(seq);
	}
	
}


//���һ����ʵ�̻���ը��Ч��
void Battle::lightBoast(int index)
{
	auto particle = ParticleSystemQuad::create(PATH_BTNEFFECT);
	particle->setPosition(Vec2(42 + random(-15, 15), 42 + random(-15, 15)));
	particle->setScale(0.4f);
	particle->setOpacity(0.5f);
	particle->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
	particle->setDuration(0.5f);										// ��������ϵͳ�ĳ���ʱ����  
	circle.at(index)->getParent()->addChild(particle);
}




//����������Ҫ������Ԫ��
void Battle::changeImg(int index)
{
	leftTop_shark->loadTexture(YUANBAO, TextureResType::PLIST);
	rightTop_shark->loadTexture(YUANBAO, TextureResType::PLIST);
	leftDown_shark->loadTexture(YUANBAO, TextureResType::PLIST);
	rightDown_shark->loadTexture(YUANBAO, TextureResType::PLIST);
	topColor_shark->loadTexture(HANDSEL, TextureResType::PLIST);
	downColor_shark->loadTexture(HANDSEL, TextureResType::PLIST);
	//A����
	if (index == 1)
	{
		leftMiddle_shark->loadTexture(YUANBAO, TextureResType::PLIST);
		rightMiddle_shark->loadTexture(YUANBAO, TextureResType::PLIST);
	}
	else if (index == 2)
	{
		leftMiddle_shark->loadTexture(TICKETS, TextureResType::PLIST);
		rightMiddle_shark->loadTexture(TICKETS, TextureResType::PLIST);
	}

}

//��ԭ������ͼƬ
void Battle::reChangeImg()
{
	leftTop_shark->loadTexture(SILVERSHARK, TextureResType::PLIST);
	rightTop_shark->loadTexture(SILVERSHARK, TextureResType::PLIST);
	leftDown_shark->loadTexture(SILVERSHARK, TextureResType::PLIST);
	rightDown_shark->loadTexture(SILVERSHARK, TextureResType::PLIST);
	topColor_shark->loadTexture(GOLDSHARK, TextureResType::PLIST);
	downColor_shark->loadTexture(GOLDSHARK, TextureResType::PLIST);
	leftMiddle_shark->loadTexture(GOLDSHARK, TextureResType::PLIST);
	rightMiddle_shark->loadTexture(GOLDSHARK, TextureResType::PLIST);
}


