#include "ColorResult.h"
#include "shark/GameInfo.h"


#define     COLORGOLD              "colorGold"
#define     COLORBG                "colorMoney"
#define     COLORLIGHT             "color_light"
#define     BLINKLIGHT             "blinkLight"
#define     TICKET                 "ticket"
#define     YUANBAOBG              "gs_yuanbao"
#define     CAIJINBOX              "caijin_box"
#define     CAIJINBG               "ColorList"

//彩金列表下的信息
#define _LIGHT			"light"
#define _NAME0			"Name0"
#define _NUM0			"Num0"
#define _GOLD0			"Gold0"
#define _NAME1			"Name1"
#define _NUM1			"Num1"
#define _GOLD1			"Gold1"
#define _NAME2			"Name2"
#define _NUM2			"Num2"
#define _GOLD2			"Gold2"
#define _CJLX_TEXT		"CJLX"


#define ROTATE_TIME		3.f
#define SCALE_TIME		1.f


ColorResult::ColorResult()
:pMainNode(nullptr)
, light(nullptr)
, blinkLight(nullptr)
, tickets(nullptr)
, yuanbao(nullptr)
, caijin_box(nullptr)
, bg_caijin(nullptr)
, caijinCome(nullptr)
, m_light(nullptr)
, m_bIsOpened(false)
, m_cjlxText(nullptr)
{
	memset(m_name, NULL, sizeof(m_name));
	memset(m_num, NULL, sizeof(m_num));
	memset(m_gold, NULL, sizeof(m_gold));
}
ColorResult::~ColorResult()
{

}
void ColorResult::setContent()
{

	pMainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, COLORGOLD));
	CC_ASSERT(NULL != pMainNode);
	
	blinkLight = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, BLINKLIGHT));
	CC_ASSERT(NULL != blinkLight);
	blinkLight->setVisible(false);

	//彩金来临
	caijinCome = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, COLORBG));
	CC_ASSERT(NULL != caijinCome);
	caijinCome->setVisible(false);



	//中船票
	tickets = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, TICKET));
	CC_ASSERT(NULL != tickets);
	tickets->setVisible(false);

	//中元宝
	yuanbao = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, YUANBAOBG));
	CC_ASSERT(NULL != yuanbao);
	yuanbao->setVisible(false);

	//彩金宝箱
	caijin_box = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, CAIJINBOX));
	CC_ASSERT(NULL != caijin_box);
	caijin_box->setVisible(false);

	//前三名显示
    bg_caijin = dynamic_cast<Node*>(_seekNodeByName(m_pNode,CAIJINBG ));
	CC_ASSERT(NULL != bg_caijin);
	bg_caijin->setVisible(false);

	//彩灯
	light = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, COLORLIGHT));
	CC_ASSERT(NULL != light);
	light->setVisible(false);

	//彩金列表
	m_light = dynamic_cast<ImageView*>(bg_caijin->getChildByName(_LIGHT));
	CC_ASSERT(nullptr != m_light);

	m_name[0] = dynamic_cast<Text*>(bg_caijin->getChildByName(_NAME0));
	CC_ASSERT(nullptr != m_name[0]);

	m_num[0] = dynamic_cast<TextAtlas*>(bg_caijin->getChildByName(_NUM0));
	CC_ASSERT(nullptr != m_num[0]);

	m_gold[0] = dynamic_cast<ImageView*>(bg_caijin->getChildByName(_GOLD0));
	CC_ASSERT(nullptr != m_gold[0]);

	m_name[1] = dynamic_cast<Text*>(bg_caijin->getChildByName(_NAME1));
	CC_ASSERT(nullptr != m_name[1]);

	m_num[1] = dynamic_cast<TextAtlas*>(bg_caijin->getChildByName(_NUM1));
	CC_ASSERT(nullptr != m_num[1]);

	m_gold[1] = dynamic_cast<ImageView*>(bg_caijin->getChildByName(_GOLD1));
	CC_ASSERT(nullptr != m_gold[1]);

	m_name[2] = dynamic_cast<Text*>(bg_caijin->getChildByName(_NAME2));
	CC_ASSERT(nullptr != m_name[2]);

	m_num[2] = dynamic_cast<TextAtlas*>(bg_caijin->getChildByName(_NUM2));
	CC_ASSERT(nullptr != m_num[2]);

	m_gold[2] = dynamic_cast<ImageView*>(bg_caijin->getChildByName(_GOLD2));
	CC_ASSERT(nullptr != m_gold[2]);

	m_cjlxText = dynamic_cast<Text*>(bg_caijin->getChildByName(_CJLX_TEXT));
	CC_ASSERT(nullptr != m_cjlxText);

	bg_caijin->setScale(_ZERO);
	
}

//开启闪烁
void ColorResult::startPlay()
{
	pMainNode->setVisible(true);

	//隐藏彩金来临几个字
	//auto call = CallFunc::create(this, callfunc_selector(ColorResult::cardLight));
	//auto seq = Sequence::create(call ,DelayTime::create(1.0f), CallFunc::create([this](){this->caijinCome->setVisible(false); }),NULL);
	//caijinCome->runAction(seq);
	
	//周边彩灯闪烁
	light->setVisible(true);
	auto out = FadeOut::create(1);
	auto sequ = Sequence::create(out, out->reverse(), NULL);
	light->runAction(RepeatForever::create(sequ));
}

//显示彩金来袭特效
void ColorResult::cardLight()
{
	//caijinCome->setVisible(true);

	auto particle = ParticleSystemQuad::create(PATH_BLAST);
	particle->setPosition(Vec2(205, 55));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(2.5f);									// 设置粒子系统的持续时间秒  
	caijinCome->addChild(particle);
}



//关闭闪烁
void ColorResult::stopPlay()
{
	light->stopAllActions();
	light->setVisible(false);
	pMainNode->setVisible(false);
	
}

//判断开奖类型
void ColorResult::judgeResult(int index)
{
	
	if (index == 1)           //元宝
	{
		showType(yuanbao);
	}
	
	else if (index == 2)     //船票
	{
		showType(tickets);
	}
	else if (index == 3)     //彩金
	{
		showType(caijin_box);
	}
}

//显示中奖的类型
void ColorResult::showType(Node* sp)
{
	auto node = (Sprite*)sp;
	auto seq = Sequence::create(CallFunc::create([node](){node->setVisible(true); })
		, DelayTime::create(2.0f)
		, CallFunc::create([node](){node->setVisible(false); }), NULL);
	node->runAction(seq);

	rotateLight();

	//显示彩金列表
	//auto call1 = CallFunc::create(this,callfunc_selector(ColorResult::ListShow));
	//auto call2 = CallFunc::create(this, callfunc_selector(ColorResult::ListHide));
	//auto sequ = Sequence::create(DelayTime::create(2.0f), call1, DelayTime::create(4.0f), call2, NULL);
	//this->runAction(sequ);
}

//开启灯光旋转
void ColorResult::rotateLight()
{
	blinkLight->setVisible(true);
	auto rota = RotateBy::create(2.0f, 360);
	blinkLight->runAction(RepeatForever::create(rota));	
}
//停止背景光圈的旋转
void ColorResult::stopRotate()
{
	blinkLight->stopAllActions();
	blinkLight->setVisible(false);
}

//彩金列表部分

void ColorResult::ListShow()
{

	auto gameInfo = GameInfo::GetInstance();
	SharkCJ cj = gameInfo->getsharkCj();
	std::string name[3] = { cj.name[0], cj.name[1], cj.name[2] };
	int ncj = cj.num;
	SCORE num[3];
	for (int i = 0; i < 3; i++)
	{
		num[i] = cj.score[i];
	}

	if (nullptr == bg_caijin)
	{
		return;
	}
	if (true == m_bIsOpened)
	{
		return;
	}
	m_cjlxText->setVisible(false);
	int index = _ZERO;
	for (size_t i = _ZERO; i < 3; i++)
	{

		if (i < ncj)
		{
			m_name[i]->setVisible(true);
			m_num[i]->setVisible(true);
			m_gold[i]->setVisible(true);
			m_name[i]->setString(name[i]);
			std::stringstream str;
			str << num[i];
			m_num[i]->setString(str.str());
		}
		else
		{
			m_name[i]->setVisible(false);
			m_num[i]->setVisible(false);
			m_gold[i]->setVisible(false);
		}
	}

	//无人中奖
	if (ncj == _ZERO)
	{
		m_cjlxText->setVisible(true);
	}

	m_bIsOpened = true;
	bg_caijin->runAction(EaseBounceOut::create(CCScaleTo::create(SCALE_TIME, 1.f)));
	m_light->runAction(CCRepeatForever::create(CCRotateBy::create(ROTATE_TIME, Vec3(_ZERO, _ZERO, 360))));
}

void ColorResult::ListHide()
{
	if (nullptr == bg_caijin)
	{
		return;
	}
	if (false == m_bIsOpened)
	{
		return;
	}
	bg_caijin->stopAllActions();
	bg_caijin->runAction(CCSequence::create(
		CCScaleTo::create(SCALE_TIME, _ZERO),
		CCCallFunc::create(this, callfunc_selector(ColorResult::CloseCallBack)),
		NULL));
}
void ColorResult::CloseCallBack()
{
	m_light->stopAllActions();
	m_bIsOpened = false;
}

//打枪效果
void ColorResult::startShoot()
{
	//动画的管理
	auto uiAction = CSLoader::createTimeline(COCOS_STUDIO_BATTLSCENE);
	auto node = pMainNode->getParent()->getParent();
	node->runAction(uiAction);
	uiAction->play("gold", false);

	_PLAY_SOUND_SHARK(LIGHT_MUSIC);

}

//开启彩色打枪效果
void ColorResult::startColor()
{
	//动画的管理
	auto uiAction = CSLoader::createTimeline(COCOS_STUDIO_BATTLSCENE);
	auto node = pMainNode->getParent()->getParent();
	node->runAction(uiAction);
	uiAction->play("handsel", false);

	_PLAY_SOUND_SHARK(LIGHT_MUSIC);
}

