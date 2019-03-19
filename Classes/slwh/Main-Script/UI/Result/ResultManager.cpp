#include "ResultManager.h"


#define GOLD_FLY_PATH	"images/common/Eff/gold2.plist"


//根(m_node)节点下各个子节点逻辑标签
#define	_RES			"res"

//_RES节点下
#define GET_TEXT		"get_num"							
#define RES_ZHX			"zhx"
#define	RES_ANIMAL		"Res_Animal"
#define SPE_TITLE		"special_title"
#define	MY_JET			"res_myjet"
#define XYCJRes			"XYCJres"
#define CJRes			"CJres"
#define CJNum			"cj_get_num"

//MY_JET 下的动物节点
//顺序 (红)动物1 ~ 4   (黄)动物1 ~4  (绿)动物1 ~4
#define RES_ARRAY		{"index", "index_0", "index_1", "index_2", "index_3","index_4", "index_5", "index_6", "index_7", "index_8", "index_9", "index_10"}

ResultManager::ResultManager() :
m_getNum(nullptr),
m_speTitle(nullptr),
m_res(nullptr),
m_speed(_ZERO),
m_getnum(_ZERO),
m_betGold(nullptr),
m_visible(_ZERO, _ZERO),
m_getYuanbao(_ZERO),
m_getTicket(_ZERO)
//m_layerColor(nullptr)
{
}

ResultManager::~ResultManager()
{
}

void ResultManager::Start()
{
	m_visible = Director::getInstance()->getWinSize();

	//颜色层
	//Color4B color = Color4B(_ZERO, _ZERO, _ZERO, COLOR_LAYER_TANSPARENT);
	//m_layerColor = CCLayerColor::create(color);
	//m_layerColor->setContentSize(Size(m_visible.x, m_visible.y));
	//m_layerColor->ignoreAnchorPointForPosition(false);
	//m_layerColor->setPosition(m_visible / 2);
	//m_node->addChild(m_layerColor, COLOR_LAYER_LEVEL);

	//加载金币的图片
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(GOLD_FLY_PATH);

	m_res = m_node->getChildByName(_RES);
	CC_ASSERT(nullptr != m_res);

	m_resAnimal.AttachNode(m_res->getChildByName(RES_ANIMAL));
	addChild(&m_resAnimal);

	m_resZHX.AttachNode(m_res->getChildByName(RES_ZHX));
	addChild(&m_resZHX);

	m_xycjres.AttachNode(m_res->getChildByName(XYCJRes));
	addChild(&m_xycjres);

	m_getNum = dynamic_cast<TextAtlas*>(m_res->getChildByName(GET_TEXT));
	CC_ASSERT(nullptr != m_getNum);

	m_speTitle = dynamic_cast<ImageView*>(m_res->getChildByName(SPE_TITLE));
	CC_ASSERT(nullptr != m_speTitle);
	m_speTitle->setVisible(false);

	m_speCJ = m_res->getChildByName(CJRes);
	CC_ASSERT(m_speCJ != nullptr);
	m_speCJ->setVisible(false);

	m_cjnum = dynamic_cast<TextAtlas*>(m_speCJ->getChildByName(CJNum));
	CC_ASSERT(nullptr != m_cjnum);


	std::string x[] = RES_ARRAY;
	auto jet = m_res->getChildByName(MY_JET);
	for (size_t i = _ZERO; i < RES_ANINAL_NUM; i++)
	{
		m_resArray[i].AttachNode(jet->getChildByName(x[i]));
		addChild(&m_resArray[i]);
	}


	//m_node->setVisible(false);
	//m_node->setOpacity(_ZERO);


	//m_node->setPosition(Vec2(_ZERO, m_visible.y /2));
	//m_particle->setOpacity(0);

	//以下是测试
	/*AnimalResult e(enumSpecialRs::Normal,enumAnimalColor::Green,enumAnimalType::Animal2,enumGameZXH::He);
	int jetbeilv[] =
	{
	11, 2, 3, 2,
	1, 4, 5, 3,
	2, 1, 54,4
	};

	int jet1[] =
	{
	123, 666, 111, 444,
	11123, 456456, 123, 4566,
	153615, 12313, 44, 88888
	};



	StartPlay(e, jetbeilv, jet1);*/
	//scheduleUpdate();
	//StartPlay();
}

//void ResultManager::update(float t)
//{
//	
//	//Move(m_node, Vec2(0, -1));
//
//	m_getNum->setPosition(m_getNum->getPosition());
//	log("-------------m_node:%f,%f------------", m_node->getPosition().x, m_node->getPosition().y);
//	log("-------------m_getNum:%f,%f------------", m_getNum->getPosition().x, m_getNum->getPosition().y);
//}

int ResultManager::SetRes(AnimalResult e, int* jetbeilv, int* jetNum, AnimalResult* e1 /*= nullptrptr*/, SCORE cj /*= 0*/, int aType/* = _ZERO*/, int* xycjarr /*= nullptr*/, float scorebl /*= _ZERO*/, float yuanbaobl /*= _ZERO*/, float cpbl/* = _ZERO*/)
{
	if (nullptr == m_node)
	{
		return -1;
	}

	//设置倍率 压分 和 中奖灯
	for (size_t i = _ZERO; i < 15; i++)
	{
		if (i < RES_ANINAL_NUM)
		{
			m_resArray[i].SetBeiLv(jetbeilv[i]);
			m_resArray[i].SetJet(jetNum[i]);
			m_resArray[i].OpenLight(false);
		}
	}


	//计算得分
	int getnum = _ZERO;
	
	//设置庄和闲
	m_resZHX.SetZHX(e.e4);

	m_speCJ->setVisible(false);
	m_xycjres.setVisible(false);
	//设置动物结果
	m_speTitle->setVisible(true);
	int index = -1;

	switch (e.e1)
	{
	case enumSpecialRs::InvalidSpecial:
		break;
	case enumSpecialRs::Normal:
		index = static_cast<int>(e.e2) * 4 + static_cast<int>(e.e3);

		m_speTitle->setVisible(false);
		m_resAnimal.SetAnimal(e, jetbeilv[index]);
		m_resArray[index].OpenLight(true);
		//计算得分
		getnum += jetbeilv[index] * jetNum[index];
		//加上ZHX
		index = 12 + static_cast<int>(e.e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::CJ:
		index = static_cast<int>(e.e2) * 4 + static_cast<int>(e.e3);

		m_speTitle->loadTexture(Path_Spe_Res_CJ, TextureResType::PLIST);
		m_resAnimal.SetAnimal(e, jetbeilv[index]);
		m_resArray[index].OpenLight(true);
		m_speCJ->setVisible(true);
		{
			std::stringstream str;
			str << cj;
			m_cjnum->setString(str.str());
		}
		//计算得分
		getnum += jetbeilv[index] * jetNum[index];
		//加上CJ
		getnum += cj;
		//加上ZHX
		index = 12 + static_cast<int>(e.e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::D3Y:
		m_speTitle->loadTexture(Path_Spe_Res_D3Y, TextureResType::PLIST);
		m_resAnimal.SetAnimal(e, -1);
		index = static_cast<int>(e.e3);
		//计算第一个得分
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);
		index += static_cast<int>(enumAnimalType::AnimalMax);
		//计算第二个得分
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);
		index += static_cast<int>(enumAnimalType::AnimalMax);
		//计算第三个得分
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);

		//加上ZHX
		index = 12 + static_cast<int>(e.e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::D4X:
		m_speTitle->loadTexture(Path_Spe_Res_D4X, TextureResType::PLIST);
		m_resAnimal.SetAnimal(e, -1);
		index = static_cast<int>(e.e2) * 4;
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);
		index++;
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);
		index++;
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);
		index++;
		getnum += jetbeilv[index] * jetNum[index];
		m_resArray[index].OpenLight(true);

		//加上ZHX
		index = 12 + static_cast<int>(e.e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::DMG:
		m_speTitle->loadTexture(Path_Spe_Res_DMG, TextureResType::PLIST);
		m_resAnimal.SetAnimal(e, -1);
		for (size_t i = _ZERO; i < RES_ANINAL_NUM; i++)
		{
			getnum += jetbeilv[i] * jetNum[i];
			m_resArray[i].OpenLight(true);
		}

		//加上ZHX
		index = 12 + static_cast<int>(e.e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::XYCJ:

		index = static_cast<int>(e.e2) * 4 + static_cast<int>(e.e3);

		m_speTitle->loadTexture(Path_Spe_Res_XYCJ, TextureResType::PLIST);
		m_resAnimal.SetAnimal(e, jetbeilv[index]);
		m_resArray[index].OpenLight(true);
	
		//计算得分
		getnum += jetbeilv[index] * jetNum[index];
		//计算幸运抽奖结果
		{
			//元宝数量
			m_getYuanbao = _ZERO;
			//船票数量
			m_getTicket = _ZERO;
			//计算幸运抽奖得分 乘以动物倍率
	
			m_getYuanbao = (int)(jetNum[index] * yuanbaobl);
			m_getTicket = (int)(jetNum[index] * cpbl);
			//中了动物 且 中了元宝却分数不够 至少得一个
			if ((jetNum[index] > _ZERO) && (m_getYuanbao == _ZERO) && (yuanbaobl > 0.00001f))  m_getYuanbao = 1;
			//中了动物 且 中了船票却分数不够 至少得一张
			if ((jetNum[index] > _ZERO) && (m_getTicket == _ZERO) && (cpbl > 0.00001f))  m_getTicket = 1;
			//计算得分
			if (scorebl > 0.00001f)
			{
				getnum += (int)(jetNum[index] * scorebl);
			}
			
			//幸运抽奖特殊逻辑
			m_xycjres.setVisible(true);
			m_xycjres.setRes(aType, xycjarr, m_getYuanbao, m_getTicket);
		}
		//加上ZHX
		index = 12 + static_cast<int>(e.e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::_4LF:
		m_speTitle->loadTexture(Path_Spe_Res_4LF, TextureResType::PLIST);
		//找到4个倍率
		{
			int a[4] = {_ZERO};
			for (size_t i = _ZERO; i < 4; i++)
			{
				index = static_cast<int>(e1[i].e2) * 4 + static_cast<int>(e1[i].e3);
				a[i] = jetbeilv[index];
				m_resArray[index].OpenLight(true);
				//计算动物得分
				getnum += jetbeilv[index] * jetNum[index];
			}
			m_resAnimal.SetAnimal(e1, a);
		}
		
		//加上ZHX
		index = 12 + static_cast<int>(e1[_ZERO].e4);
		getnum += jetbeilv[index] * jetNum[index];
		break;
	case enumSpecialRs::SpecialRsMax:
	default:
		break;
	}

	m_getnum = getnum;
	setGetnum(getnum);
	return getnum;
}

void ResultManager::setGetnum(SCORE l)
{
	std::stringstream str;
	str << l;
	m_getNum->setString(str.str());

}

void ResultManager::playYanhua(Vec2 v2)
{
	auto particle = ParticleSystemQuad::create("images/common/Eff/baoxiangbao.plist");
	particle->setScale(1.2f);
	particle->setPosition(v2);
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(2.f);									// 设置粒子系统的持续时间秒  
	m_res->addChild(particle, -100);

}
//盖章
void ResultManager::PlayGaizhang()
{

	Vec2 pos = m_speTitle->getPosition();
	m_speTitle->setScale(2.f);
	m_speTitle->setPosition(pos + Vec2(150, 100));
	m_speTitle->runAction(Sequence::create(CCSpawn::create(
		EaseSineIn::create(MoveTo::create(0.5f, pos)),
		EaseSineIn::create(ScaleTo::create(0.5f, 1.f)), NULL),
		CallFunc::create(this, callfunc_selector(ResultManager::gaizhangCallfunc)),
		NULL));

}

void ResultManager::goldandnumrool(float t)
{
	if (m_getnum > _ZERO)
	{
		playGoldFly(m_getNum->getPosition() + Vec2(_ZERO, 20), Vec2(127, 658));

		//测试说要加上数字滚动效果 还蛋疼了会儿 现在策划说不好 暂时注释去掉
		//schedule(schedule_selector(ResultManager::numroll), 0.1f);
	}
}

//数字滚动效果
void ResultManager::numroll(float t)
{

	//几次滚动完成
	static int index = _ZERO;
	if (index < 30)
	{
		setGetnum((SCORE)(m_getnum - ((1 + index) * m_getnum) / 30));
		index++;
	}
	else
	{
		setGetnum(_ZERO);
		index = _ZERO;
		unschedule(schedule_selector(ResultManager::numroll));
	}

}

void ResultManager::gaizhangCallfunc()
{

	_PLAY_MUSCI_EFF(_KZ);
	//播放震动
	auto ac1 = MoveBy::create(0.03f ,Vec2(5,_ZERO));
	auto ac2 = MoveBy::create(0.03f ,Vec2(_ZERO,5));
	auto ac3 = MoveBy::create(0.03f ,Vec2(-5, _ZERO));
	auto ac4 = MoveBy::create(0.03f ,Vec2(_ZERO, -5));
	auto seq = Sequence::create(ac1, ac2, ac3, ac4, ac1->clone(), ac2->clone(), ac3->clone(), ac4->clone(), nullptr);
	m_res->runAction(seq);

	/*auto nodegrid = NodeGrid::create();
	m_res->retain();
	m_res->removeFromParent();
	nodegrid->addChild(m_res);
	m_res->release();*/
	//nodegrid->runAction(
	//	Sequence::create(
	//	CCShaky3D::create(0.5f, Size(5, 10), 10, false),
	//	CallFuncN::create(this, callfuncN_selector(ResultManager::zdCallFunc)),
	//	NULL));
	//m_node->addChild(nodegrid);

	//播放金币喷泉
	if (m_getnum > _ZERO)
	{
		//延迟1.5秒播动画
		scheduleOnce(schedule_selector(ResultManager::goldandnumrool), 1.5f);
	}
}

//void ResultManager::zdCallFunc(Node* p)
//{
//	m_res->retain();
//	m_res->removeFromParent();
//	m_node->addChild(m_res);
//	m_res->release();
//	p->removeFromParent();
//}

void ResultManager::StartPlay()
{
	if (nullptr == m_node)
	{
		return;
	}
	m_node->setVisible(true);

	//下面开始播放各种特效

	if (m_getnum != 0)
	{
		playYanhua(Vec2(660, 643));
	}

	//如果是特殊开奖	,金币喷泉在盖章结束时候播放
	if (m_speTitle->isVisible())
	{
		PlayGaizhang();
	}
	else
	{
		//延迟2秒播动画
		if (m_getnum > _ZERO)
		{
			scheduleOnce(schedule_selector(ResultManager::goldandnumrool), 2.f);
		}
	}
}

bool ResultManager::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

int ResultManager::StartPlay(AnimalResult e, int* jetbeilv, int* jetNum, SCORE cj)
{
	int gold = SetRes(e, jetbeilv, jetNum, nullptr, cj);
	StartPlay();
	return gold;
}

//幸运抽奖
int ResultManager::StartPlay(AnimalResult e, int* jetbeilv, int* jetNum,int aType, int *a, float scorebl, float yuanbaobl, float cpbl)
{
	int gold = SetRes(e, jetbeilv, jetNum, nullptr, _ZERO, aType, a, scorebl, yuanbaobl, cpbl);
	StartPlay();
	return gold;
}

//动物结果，倍率数组，压分数组，得分，4LF结果(默认空)
int ResultManager::StartPlay(AnimalResult e, int* jetbeilv, int* jetNum, AnimalResult* e1/* = nullptrptr*/)
{

	int gold = SetRes(e, jetbeilv, jetNum, e1);
	StartPlay();
	return gold;
}

void ResultManager::StopPlay()
{
	if (nullptr == m_node)
	{
		return;
	}

	//m_node->runAction(Sequence::create(
	//	FadeOut::create(RES_ACTION_TIME),
	//	CallFunc::create(this, callfunc_selector(ResultManager::CallBackFadeOut)),
	//	nullptr
	//	));
	//m_node->runAction(MoveTo::create(RES_ACTION_TIME, Vec2(_ZERO, m_visible.y)));
}


//void ResultManager::attachBetgold(Sprite* n)
//{
//	if (n != nullptr)
//	{
//		m_betGold = n;
//	}
//
//}
//金币飞行

void ResultManager::playGoldFly(Vec2 startPos, Vec2 endPos)
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (size_t i = _ZERO; i < 20; i++)
	{
		//创建动画
		Sprite* pSprite = Sprite::create();
		m_node->getParent()->addChild(pSprite);
		pSprite->setScale(0.35f);
		pSprite->setPosition(startPos);
		pSprite->setVisible(false);
		char str[256];
		// 创建不同帧数的金币动画
		ValueMap mapTmp;
		Vector<AnimationFrame*> animFrames;
		int arr[] = { 0, 1, 2, 3, 4, 5, 6 };
		//打乱帧动画顺序4次打乱
		for (size_t i = _ZERO; i < 4; i++)
		{
			int swapIndex = random(0, 6);
			if (i == swapIndex) continue;

			int swap = arr[swapIndex];
			arr[swapIndex] = arr[i];
			arr[i] = swap;
		}

		for (size_t i = _ZERO; i < 7; i++)
		{
			sprintf(str, "gold2_%d.png", arr[i]);
			CCSpriteFrame* frame = cache->spriteFrameByName(str);

			if (frame)
			{
				CCAnimationFrame* aniFrame = CCAnimationFrame::create(frame, 1, mapTmp);
				animFrames.pushBack(aniFrame);
			}
		}

		CCAnimation *animation = CCAnimation::create(animFrames, random(0.05f, 0.1f));

		CCAnimate *animate = CCAnimate::create(animation);
		pSprite->runAction(RepeatForever::create(animate));

		float time0 = random(1.f, 2.f);
		auto ac0 = DelayTime::create(i * 0.05f);
		auto ac1 = Spawn::create(
			MoveTo::create(time0, startPos + Vec2(random(-150, 50), random(0, 200))),
			CCEaseExponentialInOut::create(ScaleTo::create(time0, random(0.55f, 0.77f))),
			NULL);
		auto ac2 = DelayTime::create(0.3f);
		float time1 = random(0.5f, 1.5f);
		auto ac3 = Spawn::create(
			CCEaseExponentialIn::create(MoveTo::create(time1, endPos)),
			//CCEaseExponentialIn::create((FadeOut::create(time1))),
			NULL);
		auto func0 = CallFuncN::create([this](Node* n){	n->removeFromParent(); });
		auto func1 = CallFuncN::create([this](Node* n){ n->setVisible(true); });
		auto fun2 = CallFunc::create([this](){ });

		auto seq = Sequence::create(ac0, func1, ac1, ac2, ac3, func0, NULL);
		pSprite->runAction(seq);
	}
}


