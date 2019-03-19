#include "GameViewBibei_SHZ.h"
#include "shuihu/Resource_SHZ.h"

USING_NS_CC;
USING_NS_CC_EXT;

//按钮
#define TAG_BT_BIG		1			//买大
#define TAG_BT_SCORE1	2			//得分
#define TAG_BT_SMALL	3			//买小
#define	TAG_BT_BIBEI1	4			//比倍
#define TAG_BT_TIE		5			//买和
#define TAG_BT_ADDBEI	6			//加倍
#define TAG_BT_CURBEI	7			//减倍

SHZCGameViewBibei::SHZCGameViewBibei(void)
{

	m_S1=1;

	m_S2=1;

	m_bibeiScore=0;

	m_bibeiWinScore=0;

	m_bibeiWinTempScore=0;

	m_CreditScore=0;

	m_CanHaveMoney=true;

	m_bStartAnim=true;	

	m_bwinLostAnim=true;

	m_ixiazhuPos=0;

	m_TempDwonscore=0;

	m_TempCCscore=0;

	m_TempTTscore=0;

	m_iDoubleCount=0;

	m_bxiazhu=false;


	
}


SHZCGameViewBibei::~SHZCGameViewBibei(void)
{
	getEventDispatcher()->removeEventListener(listener);
	listener = nullptr;

	/*touch_layer_->setTouchEnabled(false);
	touch_layer_->setSink(0);
	touch_layer_->release();*/
}



SHZCGameViewBibei* SHZCGameViewBibei::create()
{
	SHZCGameViewBibei* layer = new SHZCGameViewBibei();
	if (layer && layer->init())
	{
		layer->autorelease();

		return layer;
	}

	delete layer;

	return 0;
}



bool SHZCGameViewBibei::init()
{
	float bgX = kRevolutionWidth_SHZ/2;
	float bgY = kRevolutionHeight_SHZ/2;

	do 
	{

		m_rcdaxiao[0].setRect(44,155,310,110);
		m_rcdaxiao[1].setRect(385,150,300,100);
		m_rcdaxiao[2].setRect(720,155,310,100);


		this->touchEvent();
		if (0 == DF::shared()->GetGameType())
			m_BgImgeTexture = cocos2d::CCSprite::createWithSpriteFrameName("BG_11.png");
		else
			m_BgImgeTexture = cocos2d::CCSprite::createWithSpriteFrameName("BG_1.png");
		m_BgImgeTexture->setPosition(Vec2(bgX, bgY));
		
		addChild(m_BgImgeTexture);

		m_sai1Texture= cocos2d::CCSprite::createWithSpriteFrameName("sai1.png");
		m_sai1Texture->setPosition(Vec2(510,305));
		m_sai1Texture->setVisible(false);
		addChild(m_sai1Texture,20);

		m_sai2Texture= cocos2d::CCSprite::createWithSpriteFrameName("sai1.png");
		m_sai2Texture->setPosition(Vec2(550,305));
		m_sai2Texture->setVisible(false);
		addChild(m_sai2Texture,20);


		m_yuanbao1Texture = cocos2d::CCSprite::createWithSpriteFrameName("yuanbao2.png");
		m_yuanbao1Texture->setVisible(false);
		addChild(m_yuanbao1Texture,20);

		m_yuanbao2Texture = cocos2d::CCSprite::createWithSpriteFrameName("yuanbao1.png");
		m_yuanbao2Texture->setVisible(false);
		addChild(m_yuanbao2Texture,20);

		m_winscoreViewTexture =cocos2d::CCSprite::createWithSpriteFrameName("win.png");	
		m_winscoreViewTexture->setPosition(Vec2(kRevolutionWidth_SHZ / 2, kRevolutionHeight_SHZ / 2));//350,275));
		m_winscoreViewTexture->setVisible(false);
		addChild(m_winscoreViewTexture,50);

		m_bibeiViewTexture =cocos2d::CCSprite::createWithSpriteFrameName("bibei.png");	
		m_bibeiViewTexture->setPosition(Vec2(530,475));
		m_bibeiViewTexture->setVisible(false);
		addChild(m_bibeiViewTexture,50);

		m_defenViewTexture =cocos2d::CCSprite::createWithSpriteFrameName("defen.png");	
		m_defenViewTexture->setPosition(Vec2(530,475));
		m_defenViewTexture->setVisible(false);
		addChild(m_defenViewTexture,50);

		m_NodeBoss=CCNode::create();

		addChild(m_NodeBoss);

		//m_NodeLeft=CCNode::create();

		//addChild(m_NodeLeft);

		//m_NodeRight=CCNode::create();

		//addChild(m_NodeRight);


		m_TaiSaiViewTexture = cocos2d::CCSprite::createWithSpriteFrameName("TAISAI_view.png");
		m_TaiSaiViewTexture->setPosition(Vec2(530,205));
		addChild(m_TaiSaiViewTexture);
		m_TaiSaiViewTexture->setVisible(false);

		//添加金币和元宝的背景
		//金币数背景
		Sprite* gold = Sprite::createWithSpriteFrameName("sr_gold.png");
		gold->setPosition(Vec2(145, 695));
		this->addChild(gold, 100);
		//元宝数背景
		Sprite* ingot = Sprite::createWithSpriteFrameName("sr_-ingot.png");
		ingot->setPosition(Vec2(400, 695));
		this->addChild(ingot, 100);


		m_lbScore3=CCLabelTTF::create("34343443", "Arial", 18, CCSizeMake(120, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		m_lbScore3->setPosition(Vec2(540,113));
		m_lbScore3->setString("9");
		addChild(m_lbScore3,100);

		m_lbwinScore=CCLabelBMFont::create("0123456789","bmfonts/winScore.fnt");
		m_lbwinScore->setPosition(Vec2(kRevolutionWidth_SHZ / 2/*620*/, 295));
		m_lbwinScore->setString("0");
		m_lbwinScore->setVisible(false);
		addChild(m_lbwinScore,100);

		cocos2d::CCSprite* ImageRecord=cocos2d::CCSprite::createWithSpriteFrameName("record_big_flash.png");
		m_RecordTexture=CCSpriteBatchNode::createWithTexture(ImageRecord->getTexture());
		addChild(m_RecordTexture,50);

		ForeverAction(m_NodeBoss,"boss_3_%d.png",7,Vec2(520,450));
		//ForeverAction(m_NodeLeft,"left_3_%d.png",27,ccp(270,440));
		//ForeverAction(m_NodeRight,"right_3_%d.png",28,ccp(804,442));


		//创建按钮
		CreatedGameBt();

	} while (0);

	return true;
}

void SHZCGameViewBibei::touchEvent()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool
	{
		return true;
	};
	listener->onTouchMoved = [this](Touch* touch, Event* event)	{};
	listener->onTouchEnded = [this](Touch* touch, Event* event)	
	{
		if (!m_bxiazhu)
		{
			return;
		}

		if (m_rcdaxiao[0].containsPoint(touch->getLocation()))
		{
			m_btSmall->setEnabled(false);
			m_btBig->setEnabled(false);
			m_bttie->setEnabled(false);
			m_btGetScore->setEnabled(false);
			m_btaddbei->setEnabled(false);
			m_btcurbei->setEnabled(false);
			BiBei(0, 1);

			m_yuanbao1Texture->setPosition(Vec2(220, 195));
			m_yuanbao1Texture->setVisible(true);

			m_ixiazhuPos = 0;

			m_bxiazhu = false;

			return;
		}

		if (m_rcdaxiao[1].containsPoint(touch->getLocation()))
		{
			m_btSmall->setEnabled(false);
			m_btBig->setEnabled(false);
			m_bttie->setEnabled(false);
			m_btGetScore->setEnabled(false);
			m_btaddbei->setEnabled(false);
			m_btcurbei->setEnabled(false);
			BiBei(0, 2);
			m_ixiazhuPos = 1;
			m_yuanbao1Texture->setPosition(Vec2(535, 195));
			m_yuanbao1Texture->setVisible(true);

			m_bxiazhu = false;

			return;
		}

		if (m_rcdaxiao[2].containsPoint(touch->getLocation()))
		{

			m_btSmall->setEnabled(false);
			m_btBig->setEnabled(false);
			m_bttie->setEnabled(false);
			m_btGetScore->setEnabled(false);
			m_btaddbei->setEnabled(false);
			m_btcurbei->setEnabled(false);
			BiBei(0, 0);

			m_yuanbao1Texture->setPosition(Vec2(835, 195));
			m_yuanbao1Texture->setVisible(true);

			m_ixiazhuPos = 2;

			m_bxiazhu = false;

			return;
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


void SHZCGameViewBibei::onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	Control* ctr = (Control*)obj;

	//SoundManager::shared()->playSound("btsounds");

	//控件判断
	switch (ctr->getTag())
	{
	case TAG_BT_SCORE1:
		{
			m_btBig->setEnabled(false);
			m_btGetScore->setEnabled(false);	
			m_btSmall->setEnabled(false);
			//m_btbibei->setEnabled(false);
			m_bttie->setEnabled(false);

			m_defenViewTexture->setVisible(false);
			m_bibeiViewTexture->setVisible(false);
			unschedule(schedule_selector(SHZCGameViewBibei::OnbibeiordefenTimer));

			//金币飞出效果
			//金币飞出效果
			auto seq = Sequence::create(CallFunc::create(this, callfunc_selector(SHZCGameViewBibei::createCoin)), CallFunc::create(this, callfunc_selector(SHZCGameViewBibei::getWinScore)), NULL);
			this->runAction(seq);
			

			SHZSoundManager::shared()->stopMusic();

			char sSound[32]={0};
			//sprintf(sSound, "bibeibgsound");
			//SoundManager::shared()->stopSound(sSound);

			//sprintf(sSound, "xiongdiwushu");
			//SoundManager::shared()->playMusic(sSound);


			break;
		}

	case TAG_BT_BIBEI1:
		{
			m_btBig->setEnabled(false);
			m_btGetScore->setEnabled(false);	
			m_btSmall->setEnabled(false);
			//m_btbibei->setEnabled(false);
			m_bttie->setEnabled(false);
			m_yuanbao1Texture->setVisible(false);
			m_yuanbao2Texture->setVisible(false);
			StartAnim(m_CreditScore,m_bibeiWinScore);
			
			m_defenViewTexture->setVisible(false);
			m_bibeiViewTexture->setVisible(false);
			m_winscoreViewTexture->setVisible(false);
			m_lbwinScore->setVisible(false);
			unschedule(schedule_selector(SHZCGameViewBibei::OnbibeiordefenTimer));
		

			char strScore[32];

			m_bibeiScore=m_bibeiWinScore;

			sprintf(strScore,"%d",m_bibeiScore);

			/*m_bibeiWinScore=0;

			m_lbScore2->setString("0");*/

			m_lbScore3->setString(strScore);

			break;
		}
	case TAG_BT_BIG:
		{
			m_btSmall->setEnabled(false);
			m_btBig->setEnabled(false);
			m_bttie->setEnabled(false);
			m_btaddbei->setEnabled(false);
			m_btcurbei->setEnabled(false);
			m_btGetScore->setEnabled(false);
			BiBei(0,0);

			m_yuanbao1Texture->setPosition(Vec2(835,195));
			m_yuanbao1Texture->setVisible(true);
			m_bxiazhu=false;
			m_ixiazhuPos=2;
			break;
		}
	case TAG_BT_SMALL:
		{
			m_btSmall->setEnabled(false);
			m_btBig->setEnabled(false);
			m_bttie->setEnabled(false);
			m_btaddbei->setEnabled(false);
			m_btcurbei->setEnabled(false);
			m_btGetScore->setEnabled(false);
			BiBei(0,1);

			m_yuanbao1Texture->setPosition(Vec2(220,195));
			m_yuanbao1Texture->setVisible(true);
			m_bxiazhu=false;
			m_ixiazhuPos=0;

			break;
		}
	case TAG_BT_TIE:
		{
			m_btSmall->setEnabled(false);
			m_btBig->setEnabled(false);
			m_bttie->setEnabled(false);
			m_btaddbei->setEnabled(false);
			m_btcurbei->setEnabled(false);
			m_btGetScore->setEnabled(false);
			BiBei(0,2);
			m_ixiazhuPos=1;
			m_yuanbao1Texture->setPosition(Vec2(535,195));
			m_yuanbao1Texture->setVisible(true);
			m_bxiazhu=false;
			break;
		}
	case TAG_BT_ADDBEI:
		{

			m_iDoubleCount++;

			if (m_iDoubleCount>=1)
			{
				m_iDoubleCount=1;
				m_btaddbei->setEnabled(false);
			}

			m_CreditScore=m_CreditScore-m_bibeiScore;

			m_bibeiScore=m_bibeiScore*2;

			m_lbScore3->setString(toString(m_bibeiScore));

			m_btcurbei->setEnabled(true);

			SHZIClientKernel * kernel = SHZIClientKernel::get();

			kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_ADDBEI_SHZ);

			break;

		}
	case TAG_BT_CURBEI:
		{

			m_iDoubleCount--;

			if (m_iDoubleCount<=-1)
			{
				m_iDoubleCount=-1;
				m_btcurbei->setEnabled(false);
			}

			m_bibeiScore=m_bibeiScore/2;

			m_CreditScore=m_CreditScore+m_bibeiScore;

			m_lbScore3->setString(toString(m_bibeiScore));

			m_btaddbei->setEnabled(true);

			SHZIClientKernel * kernel = SHZIClientKernel::get();

			kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_CURBEI_SHZ);

			break;

		}
		
	}
}
/*-----------金币飞出效果--------------*/
//创建金币
void SHZCGameViewBibei::createCoin()
{
	srand(unsigned(time(NULL)));
	int m = 0;
	for (int n = 0; n < 9; n++)
	{
		auto sp = Sprite::create();
		sp->setPosition(Vec2(537, 370));
		this->createCoinFly(sp, m);
		this->addChild(sp, 1000);
		m++;
	}

	//auto act1 = CallFunc::create(this, callfunc_selector(SHZCGameViewBibei::playEff));
	//auto seq = Sequence::create(DelayTime::create(0.4f), act1, NULL);
	//this->runAction(seq);

}

//金币特效
void SHZCGameViewBibei::createCoinFly(Node* sp, int m)
{
	//创建动画
	Sprite* pSprite = (Sprite*)sp;
	int x = rand() % 200;
	int y = rand() % 200;
	if (y < 80)
		y = y + 50;
	int iCount = 6;
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int n = rand() % 3; n <= iCount; n++)
	{
		sprintf(str, "gold2_%d.png", n);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame, 1, mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}
	}
	cocos2d::CCAnimation *animation = CCAnimation::create(animFrames, 0.08f);

	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);
	auto act1 = MoveTo::create(0.3 + m*0.05, Vec2(pSprite->getPositionX() - 120 + x, pSprite->getPositionY() + y));
	auto act2 = MoveTo::create(0.3 + m*0.02, Vec2(55, 695));
	Action* call = CallFuncN::create(pSprite, callfuncN_selector(SHZCGameViewBibei::removeCoinFly));
	Action* call2 = CallFuncN::create(this, callfuncN_selector(SHZCGameViewBibei::coinChange));
	auto seq = Sequence::create(act1, act2, call, call2, NULL);
	pSprite->runAction(seq);
	pSprite->runAction(RepeatForever::create(animate));
}

//粒子特效
void SHZCGameViewBibei::playEff()
{
	_PLAY_SOUND_SHZ(GOLDWIN);
	auto particle = ParticleSystemQuad::create("images/common/Eff/baoxiangbao.plist");
	particle->setScale(1.2f);
	particle->setPosition(Vec2(956, 200));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(1.0f);									// 设置粒子系统的持续时间秒  
	this->addChild(particle, 999);
}



//移除金币
void SHZCGameViewBibei::removeCoinFly(Node* obj)
{
	Sprite* sp = (Sprite*)obj;
	sp->stopAllActions();
	sp->removeFromParent();
}

//金币放大效果
void SHZCGameViewBibei::coinChange(Node* obj)
{
	//添加金币
	auto  newSp = Sprite::createWithSpriteFrameName("sg_gold.png");
	newSp->setPosition(Vec2(50, 695));
	this->addChild(newSp, 101);
	auto seq = Sequence::create(ScaleTo::create(0.1, 1.3), CallFunc::create([newSp](){newSp->removeFromParentAndCleanup(true); }), NULL);
	newSp->runAction(seq);
}
void SHZCGameViewBibei::getWinScore()
{
	GetWinScore(1);
}

/*-------------------------------*/
//得分
void SHZCGameViewBibei::GetWinScore(int gettype)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	CMD_C_GetScore_SHZ getscore;
	getscore.m_GetType = gettype;
	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_GETSCORE_SHZ,&getscore,sizeof(CMD_C_GetScore_SHZ));

}
//获取赢分
void SHZCGameViewBibei::GetWinScore(int winscore,int gametype)
{
	mbGetScore = true;

	m_bibeiWinScore = winscore;

	char strSocre[32];
	sprintf(strSocre,"%d",m_bibeiWinScore);
	//m_lbScore2->setString(strSocre);

	SHZSoundManager::shared()->stopMusic();




	m_btGetScore->setEnabled(false);
	//m_btbibei->setEnabled(false);
	m_TempCCscore = m_CreditScore;
	m_TempDwonscore = m_bibeiWinScore / 10;
	if(m_TempDwonscore<1)m_TempDwonscore = 1;
	//CBassMusicManager::GetInstance()->Play(19,false);
	//SetTimer(DWONSCORE_SHZ,100,NULL);
	
	char sSound[32]={0};
	sprintf(sSound, "defen");
	SHZSoundManager::shared()->playSound(sSound);

	/*if (winscore > 0){
		sprintf(sSound, "ying");
		SoundManager::shared()->stopSound(sSound);
	}
	else{
		sprintf(sSound, "shu");
		SoundManager::shared()->stopSound(sSound);
	}*/

	unschedule(schedule_selector(SHZCGameViewBibei::OnbibeiordefenTimer));
	m_bibeiViewTexture->setVisible(false);
	m_defenViewTexture->setVisible(false);

	schedule(schedule_selector(SHZCGameViewBibei::OnDwonScoreTimer),0.1f);
}

//比倍
bool SHZCGameViewBibei::BiBei(int bibeitype,int bibeimax)
{
	SHZ_CMD_C_BiBeiStart_SHZ bibeistart;
	bibeistart.m_BiBeiType = bibeitype;
	bibeistart.m_MaxOrOther = bibeimax;
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_BIBEISTART_SHZ,&bibeistart,sizeof(SHZ_CMD_C_BiBeiStart_SHZ));
	return true;
}
//比倍结果
void SHZCGameViewBibei::SetBiBei(int winscore,int S1,int S2)
{
	mbGetScore = false;

	char sSound[32]={0};
	sprintf(sSound, "bibeibgsound");
	//SoundManager::shared()->stopSound(sSound);

	m_iDoubleCount=0;

	if (winscore*2>m_CreditScore)
	{
		m_iDoubleCount=-1;
		m_btaddbei->setEnabled(false);
	}


	m_S1 = S1;
	m_S2 = S2;

	m_TaiSaiViewTexture->stopAllActions();
	m_TaiSaiViewTexture->setVisible(false);

	m_bwinLostAnim=true;

	m_NodeBoss->setVisible(false);

	SequenceAction("boss_1_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowLotteryAnim),14,Vec2(520,450),1);

	char str[32];

	sprintf(str,"sai%d.png",S1);

	m_sai1Texture->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));

	sprintf(str,"sai%d.png",S2);

	m_sai2Texture->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));

	schedule(schedule_selector(SHZCGameViewBibei::OnShowSaizi),1.2f);

	//m_sai1Texture->setVisible(true);
	//m_sai2Texture->setVisible(true);

	m_bibeiWinTempScore = winscore;

	m_bibeiWinScore=winscore;
}


//比倍数组
void SHZCGameViewBibei::CopyBiBeiData(int ibibeidata,int ipos)
{
	if (ibibeidata==0)
	{
		return;
	}

	char strName[32];

	if (ibibeidata==1)
	{
		sprintf(strName,"record_big_flash.png");
	}
	if (ibibeidata==2)
	{
		sprintf(strName,"record_small_flash.png");
	}
	if (ibibeidata==3)
	{
		sprintf(strName,"record_mid_flash.png");
	}

	PLAZZ_PRINTF("XXX3333");

	CCSprite* pImage=CCSprite::createWithSpriteFrameName(strName);

	pImage->setPosition(Vec2(175+ipos*85,627));

	PLAZZ_PRINTF("XXX222");

	m_RecordTexture->addChild(pImage);
		
	
	

}
//庄家开奖动画
void SHZCGameViewBibei::ShowLotteryAnim(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions(); 

	deadSpt->removeFromParentAndCleanup(true);

	m_NodeBoss->setVisible(false);
	//m_NodeLeft->setVisible(false);
	//m_NodeRight->setVisible(false);


	if (mbGetScore) return;

	if (m_bibeiWinTempScore>0)
	{
		m_yuanbao1Texture->setVisible(false);

		if (m_ixiazhuPos==2)
		{
			m_yuanbao2Texture->setPosition(Vec2(835,195));
		}
		if (m_ixiazhuPos==0)
		{
			m_yuanbao2Texture->setPosition(Vec2(220,195));
		}

		if (m_ixiazhuPos==1)
		{
			m_yuanbao2Texture->setPosition(Vec2(535,195));
		}

		m_yuanbao2Texture->setVisible(true);

		char sSound[32]={0};
		sprintf(sSound, "ying");
		SHZSoundManager::shared()->playSound(sSound);

		SequenceAction("boss_4_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowWinLostAnim),26,Vec2(530,450),1);

		SequenceAction("left_1_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowWinLostAnim),30,Vec2(270,440),2);

		SequenceAction("right_1_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowWinLostAnim),18,Vec2(844,448),3);

		schedule(schedule_selector(SHZCGameViewBibei::OnShowSaizi),3.1f);

	}else
	{
		char sSound[32]={0};
		sprintf(sSound, "shu");
		SHZSoundManager::shared()->playSound(sSound);

		SequenceAction("boss_2_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowWinLostAnim),15,Vec2(520,450),1);

		SequenceAction("left_2_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowWinLostAnim),30,Vec2(270,440),2);

		SequenceAction("right_2_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowWinLostAnim),26,Vec2(822,436),3);
	}

}
//庄家和赌徒开奖后动画
void SHZCGameViewBibei::ShowWinLostAnim(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	deadSpt->removeFromParentAndCleanup(true);


	if (node->getTag()==2)
	{
	
		//m_NodeLeft->setVisible(true);
		
	}

	if (node->getTag()==3)
	{
		
		
		//m_NodeRight->setVisible(true);
	}

	if (node->getTag()==1)
	{

		//m_NodeLeft->setVisible(false);
		//m_NodeRight->setVisible(false);
		//m_NodeBoss->setVisible(true);
		/*if (!m_bwinLostAnim)
		{
		return;
		}

		m_bwinLostAnim=false;*/

		char str[32];

		sprintf(str,"%d",m_bibeiWinTempScore);

		//m_lbScore2->setString(str);

		sprintf(str,"%d",m_bibeiScore);

		m_lbScore3->setString(str);

		if (m_bibeiWinTempScore>0)
		{
			m_CanHaveMoney=true;

			//m_btbibei->setEnabled(true);
			m_btGetScore->setEnabled(true);

			char strScore[32];

			sprintf(strScore,"%d",m_bibeiWinTempScore);

			m_lbwinScore->setString(strScore);
			m_winscoreViewTexture->setVisible(true);
			m_lbwinScore->setVisible(true);
			schedule(schedule_selector(SHZCGameViewBibei::OnbibeiordefenTimer),0.5f);

			StartAnim(m_CreditScore,m_bibeiWinScore);

		}else
		{
			char sSound[32]={0};

			SHZSoundManager::shared()->stopMusic();

			//sprintf(sSound, "xiongdiwushu");
			//SoundManager::shared()->playMusic(sSound);

			sprintf(sSound, "bibeibgsound");
			//SoundManager::shared()->stopSound(sSound);

			ShowBibeiwin(false);

			m_GameSceneInterface->OnChangeGameView(m_CreditScore);
		}
	}

	

}
//开始动作
void SHZCGameViewBibei::StartAnim(int iCreditScore,int ibibeiscore)
{

	SHZSoundManager::shared()->stopMusic();

	//SoundManager::shared()->stopSound("bibeibgsound");

	char sSound[32]={0};
	sprintf(sSound, "bibeibgsound");
	SHZSoundManager::shared()->playMusic(sSound);

	ShowBibeiwin(false);

	PLAZZ_PRINTF("22222");

	m_bStartAnim=true;

	m_CreditScore=iCreditScore;

	m_btBig->setEnabled(false);		
	m_btGetScore->setEnabled(false);	
	m_btSmall->setEnabled(false);	
	m_bttie->setEnabled(false);		

	m_btaddbei->setEnabled(true);
	m_btcurbei->setEnabled(true);

	m_bibeiScore=ibibeiscore;

	if (ibibeiscore*2>m_CreditScore)
	{
		m_btaddbei->setEnabled(false);
	}

	m_yuanbao1Texture->setVisible(false);
	m_yuanbao2Texture->setVisible(false);

	m_sai1Texture->setVisible(false);
	m_sai2Texture->setVisible(false);

	char str[32];

	sprintf(str,"%d",m_CreditScore);

	//m_lbScore1->setString(str);

	sprintf(str,"%d",ibibeiscore);

	m_lbScore3->setString(str);

	m_NodeBoss->setVisible(false);
	//m_NodeLeft->setVisible(false);
	//m_NodeRight->setVisible(false);
	
	SequenceAction("boss_0_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowStartAnim),26,Vec2(520,450),1);

	//SequenceAction("left_0_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowStartAnim),29,ccp(270,440),2);

	//SequenceAction("right_0_%d.png",callfuncND_selector(SHZCGameViewBibei::ShowStartAnim),29,ccp(816,438),3);

}
//开始动画
void SHZCGameViewBibei::ShowStartAnim(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	if (node->getTag()==1)
	{
		m_btBig->setEnabled(true);		
		m_btGetScore->setEnabled(true);	
		m_btSmall->setEnabled(true);	
		m_bttie->setEnabled(true);	

		m_bxiazhu=true;

		m_NodeBoss->setVisible(true);

		m_TaiSaiViewTexture->setVisible(true);

		CCToggleVisibility* pVisibility=CCToggleVisibility::create();

		CCActionInterval*  action1 = CCRepeatForever::create(CCSequence::create(CCDelayTime::create(0.7f),pVisibility,NULL)); 

		m_TaiSaiViewTexture->runAction(action1);
	}

	//if (node->getTag()==2)
	//{
	//	m_NodeLeft->setVisible(true);
	//}

	//if (node->getTag()==3)
	//{
	//	m_NodeRight->setVisible(true);
	//}
	
	deadSpt->removeFromParentAndCleanup(true);

}
//创建动作(循环播放动画)
void SHZCGameViewBibei::ForeverAction(CCNode* node,const char *strName,int iCount,CCPoint ptImage,int iFirst)
{
	//创建动画
	CCSprite* pSprite = CCSprite::create();

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int i=0;i<iCount;i++)
	{
		sprintf(str,strName,i+iFirst);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);  

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame,1,mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}

	}

	cocos2d::CCAnimation *animation =CCAnimation::create(animFrames, 0.2f);

	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);  
	//添加动作，一直循环  
	pSprite->runAction(cocos2d::CCRepeatForever::create(animate)); 

	pSprite->setPosition(ptImage);

	//    设置循环的次数  
	//    fish->runAction(CCRepeat::create(animate, 10));  
	//   把精灵添加到层上，背景层参数为0,我就把此动画精灵与背景层设置不在同一个层上  
	//pSprite->setVisible(false);
	node->addChild(pSprite);  

}
//创建动作(播放动画一次)
void SHZCGameViewBibei::SequenceAction(const char *strImage,SEL_CallFuncND selector,int iCount,CCPoint ptImage,int itag)
{
	//创建动画
	CCSprite* pSprite = CCSprite::create();

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int i=0;i<iCount;i++)
	{
		sprintf(str,strImage,i+1);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );  

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame,1,mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}

	}

	PLAZZ_PRINTF("%s%f,%d",a_u8("时间:"),20.0f/iCount,iCount);

	cocos2d::CCAnimation *animation =CCAnimation::create(animFrames, 4.0f/iCount);

	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);  

	CCSequence* act = CCSequence::create(animate, CCCallFuncND::create(this, selector, 0),0);// 播放动画结束

	pSprite->runAction(act);

	pSprite->setTag(itag);

	pSprite->setPosition(ptImage);

	addChild(pSprite);

}
//创建按钮
void SHZCGameViewBibei::CreatedGameBt()
{
	m_btGetScore = createButton_SHZ("bt_bibeiscore0.png", "bt_bibeiscore1.png", "bt_bibeiscore2.png", this, cccontrol_selector(SHZCGameViewBibei::onBtnClick));
	m_btGetScore->setTag(TAG_BT_SCORE1);
	m_btGetScore->setEnabled(false);
	//m_btZidong->setEnabled(false);
	m_btGetScore->setPosition(Vec2(944,70));
	addChild(m_btGetScore,100);


	m_btaddbei = createButton_SHZ("addbei0.png", "addbei1.png", "addbei2.png", this, cccontrol_selector(SHZCGameViewBibei::onBtnClick));
	m_btaddbei->setTag(TAG_BT_ADDBEI);
	//m_btaddbei->setEnabled(false);
	//m_btAddScore->setEnabled(false);
	m_btaddbei->setPosition(Vec2(140,89));
	addChild(m_btaddbei,100);

	m_btcurbei = createButton_SHZ("curbei0.png", "curbei1.png", "curbei2.png", this, cccontrol_selector(SHZCGameViewBibei::onBtnClick));
	m_btcurbei->setTag(TAG_BT_CURBEI);
	//m_btcurbei->setEnabled(false);
	//m_btAddScore->setEnabled(false);
	m_btcurbei->setPosition(Vec2(140,41));
	addChild(m_btcurbei,100);

	m_btBig = createButton_SHZ("bt_big0.png", "bt_big1.png", "bt_big2.png", this, cccontrol_selector(SHZCGameViewBibei::onBtnClick));
	m_btBig->setTag(TAG_BT_BIG);
	m_btBig->setEnabled(false);
	//m_btAddScore->setEnabled(false);
	m_btBig->setPosition(Vec2(737,45));
	addChild(m_btBig,100);

	m_btSmall = createButton_SHZ("bt_small0.png", "bt_small1.png", "bt_small2.png", this, cccontrol_selector(SHZCGameViewBibei::onBtnClick));
	m_btSmall->setTag(TAG_BT_SMALL);
	m_btSmall->setEnabled(false);
	//m_btbibei->setEnabled(false)
	m_btSmall->setPosition(Vec2(338,45));
	addChild(m_btSmall,100);

	m_bttie = createButton_SHZ("bt_tie0.png", "bt_tie1.png", "bt_tie2.png", this, cccontrol_selector(SHZCGameViewBibei::onBtnClick));
	m_bttie->setTag(TAG_BT_TIE);
	m_bttie->setEnabled(false);
	//m_btStart->setEnabled(false);
	m_bttie->setPosition(Vec2(538,45));
	addChild(m_bttie,100);


}
//比倍得分
void SHZCGameViewBibei::OnbibeiordefenTimer(float dt)
{

	static bool b=true;

	if (b)
	{

		static bool bshow=true;

		if (bshow)
		{
			m_bibeiViewTexture->setVisible(false);
			m_defenViewTexture->setVisible(true);

			bshow=false;

		}else
		{
			m_bibeiViewTexture->setVisible(true);
			m_defenViewTexture->setVisible(false);

			bshow=true;
		}

		b=false;

	}else
	{
		m_bibeiViewTexture->setVisible(false);
		m_defenViewTexture->setVisible(false);

		b=true;
	}
}

//获取赢分
void SHZCGameViewBibei::OnDwonScoreTimer(float dt)
{
	char strSocre[32];
	//游戏下分DWONSCORE_SHZ
	m_bibeiWinScore = m_bibeiWinScore - m_TempDwonscore;
	m_CreditScore = m_CreditScore + m_TempDwonscore;
	//sprintf(strSocre,"%d",m_CreditScore);
	//m_lbScore1->setString(strSocre);
	sprintf(strSocre,"%d",m_bibeiWinScore);
	//m_lbScore2->setString(strSocre);

	m_lbwinScore->setString(strSocre);

	if(m_bibeiWinScore<0)
	{
		m_bibeiWinScore = 0;

		sprintf(strSocre,"%d",m_bibeiWinScore);
		//m_lbScore2->setString(strSocre);
		m_lbwinScore->setString(strSocre);
		m_CreditScore=m_TempCCscore+m_TempTTscore;
		//sprintf(strSocre,"%d",m_CreditScore);
		//m_lbScore1->setString(strSocre);

	}
	if(m_bibeiWinScore==0)
	{

		m_CanHaveMoney = false;  
		m_winscoreViewTexture->setVisible(false);
		m_lbwinScore->setVisible(false);
		unschedule(schedule_selector(SHZCGameViewBibei::OnDwonScoreTimer));

		m_GameSceneInterface->OnChangeGameView(m_CreditScore);
		PLAZZ_PRINTF("score2:%d\n", m_CreditScore);
	}
}
//显示塞子
void SHZCGameViewBibei::OnShowSaizi(float dt)
{
	unschedule(schedule_selector(SHZCGameViewBibei::OnShowSaizi));

	if (m_sai1Texture->isVisible())
	{
		m_sai1Texture->setVisible(false);
		m_sai2Texture->setVisible(false);

	}else
	{
		char sSound[32]={0};
		sprintf(sSound, "%ddian",m_S1+m_S2);
		SHZSoundManager::shared()->playSound(sSound);
		SHZIClientKernel * kernel = SHZIClientKernel::get();
		kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_REQUESTBEIBEI_SHZ);
		m_sai1Texture->setVisible(true);
		m_sai2Texture->setVisible(true);
	}

}
//加倍
void SHZCGameViewBibei::SetAddbei(int iaddScore)
{
	m_lbScore3->setString(toString(iaddScore));

	m_bibeiWinScore=iaddScore;
}
//比倍赢分
void SHZCGameViewBibei::ShowBibeiwin(bool bshow)
{

	if (bshow)
	{
		

	}else
	{
		m_winscoreViewTexture->setVisible(false);
		m_lbwinScore->setVisible(false);
		m_bibeiViewTexture->setVisible(false);
		m_defenViewTexture->setVisible(false);
		unschedule(schedule_selector(SHZCGameViewBibei::OnbibeiordefenTimer));
	}

}