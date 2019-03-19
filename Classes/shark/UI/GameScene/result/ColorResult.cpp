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

//�ʽ��б��µ���Ϣ
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

	//�ʽ�����
	caijinCome = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, COLORBG));
	CC_ASSERT(NULL != caijinCome);
	caijinCome->setVisible(false);



	//�д�Ʊ
	tickets = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, TICKET));
	CC_ASSERT(NULL != tickets);
	tickets->setVisible(false);

	//��Ԫ��
	yuanbao = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, YUANBAOBG));
	CC_ASSERT(NULL != yuanbao);
	yuanbao->setVisible(false);

	//�ʽ���
	caijin_box = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, CAIJINBOX));
	CC_ASSERT(NULL != caijin_box);
	caijin_box->setVisible(false);

	//ǰ������ʾ
    bg_caijin = dynamic_cast<Node*>(_seekNodeByName(m_pNode,CAIJINBG ));
	CC_ASSERT(NULL != bg_caijin);
	bg_caijin->setVisible(false);

	//�ʵ�
	light = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, COLORLIGHT));
	CC_ASSERT(NULL != light);
	light->setVisible(false);

	//�ʽ��б�
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

//������˸
void ColorResult::startPlay()
{
	pMainNode->setVisible(true);

	//���زʽ����ټ�����
	//auto call = CallFunc::create(this, callfunc_selector(ColorResult::cardLight));
	//auto seq = Sequence::create(call ,DelayTime::create(1.0f), CallFunc::create([this](){this->caijinCome->setVisible(false); }),NULL);
	//caijinCome->runAction(seq);
	
	//�ܱ߲ʵ���˸
	light->setVisible(true);
	auto out = FadeOut::create(1);
	auto sequ = Sequence::create(out, out->reverse(), NULL);
	light->runAction(RepeatForever::create(sequ));
}

//��ʾ�ʽ���Ϯ��Ч
void ColorResult::cardLight()
{
	//caijinCome->setVisible(true);

	auto particle = ParticleSystemQuad::create(PATH_BLAST);
	particle->setPosition(Vec2(205, 55));
	particle->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
	particle->setDuration(2.5f);									// ��������ϵͳ�ĳ���ʱ����  
	caijinCome->addChild(particle);
}



//�ر���˸
void ColorResult::stopPlay()
{
	light->stopAllActions();
	light->setVisible(false);
	pMainNode->setVisible(false);
	
}

//�жϿ�������
void ColorResult::judgeResult(int index)
{
	
	if (index == 1)           //Ԫ��
	{
		showType(yuanbao);
	}
	
	else if (index == 2)     //��Ʊ
	{
		showType(tickets);
	}
	else if (index == 3)     //�ʽ�
	{
		showType(caijin_box);
	}
}

//��ʾ�н�������
void ColorResult::showType(Node* sp)
{
	auto node = (Sprite*)sp;
	auto seq = Sequence::create(CallFunc::create([node](){node->setVisible(true); })
		, DelayTime::create(2.0f)
		, CallFunc::create([node](){node->setVisible(false); }), NULL);
	node->runAction(seq);

	rotateLight();

	//��ʾ�ʽ��б�
	//auto call1 = CallFunc::create(this,callfunc_selector(ColorResult::ListShow));
	//auto call2 = CallFunc::create(this, callfunc_selector(ColorResult::ListHide));
	//auto sequ = Sequence::create(DelayTime::create(2.0f), call1, DelayTime::create(4.0f), call2, NULL);
	//this->runAction(sequ);
}

//�����ƹ���ת
void ColorResult::rotateLight()
{
	blinkLight->setVisible(true);
	auto rota = RotateBy::create(2.0f, 360);
	blinkLight->runAction(RepeatForever::create(rota));	
}
//ֹͣ������Ȧ����ת
void ColorResult::stopRotate()
{
	blinkLight->stopAllActions();
	blinkLight->setVisible(false);
}

//�ʽ��б���

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

	//�����н�
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

//��ǹЧ��
void ColorResult::startShoot()
{
	//�����Ĺ���
	auto uiAction = CSLoader::createTimeline(COCOS_STUDIO_BATTLSCENE);
	auto node = pMainNode->getParent()->getParent();
	node->runAction(uiAction);
	uiAction->play("gold", false);

	_PLAY_SOUND_SHARK(LIGHT_MUSIC);

}

//������ɫ��ǹЧ��
void ColorResult::startColor()
{
	//�����Ĺ���
	auto uiAction = CSLoader::createTimeline(COCOS_STUDIO_BATTLSCENE);
	auto node = pMainNode->getParent()->getParent();
	node->runAction(uiAction);
	uiAction->play("handsel", false);

	_PLAY_SOUND_SHARK(LIGHT_MUSIC);
}

