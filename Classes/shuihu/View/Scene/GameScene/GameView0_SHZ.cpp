#include "GameView0_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "df/LocalStore.h"
#include "shuihu/View/Scene/LotteryScene/DrawMain.h"
#include "shuihu/View/Scene/LotteryScene/LotteryMission_SHZ.h"
#include "shuihu/View/Scene/GetHouseIndex_SHZ.h"
#include "shuihu/View/Scene/CD_SHZ.h"
#include "shuihu/Resource_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "HallGameScene.h"
#include "Player/Player.h"
USING_NS_CC;
USING_NS_CC_EXT;

//艺术字
#define GAME_NUMNER2 "bmfonts/game_number2.png"
#define GAME_NUMBER3 "bmfonts/game_number3.png"
#define GAME_NUMBER4 "bmfonts/game_number4.png"
//宏定义
//载入动画
#define LOADSTRUCT_SHZ     111
//类型闪动
#define TYPEIMGSTRUCT_SHZ  112
//滚动停止
#define TYPESCROLLSTOP_SHZ  113
//下分游戏
#define DWONSCORE_SHZ       200

#define ROOMLIST_PLIST_SHZ      "selectroom/room.plist"
#define ROOMLIST_CZZ_SHZ        "selectroom/room.pvr.ccz"

//按钮
#define TAG_BT_ZIDONG_SHZ	3			//自动
#define TAG_BT_SCORE_SHZ	4			//得分
#define TAG_BT_ADD_SHZ		5			//加注
#define	TAG_BT_BIBEI_SHZ	6			//比倍
#define TAG_BT_START_SHZ	7			//开始
#define TAG_BT_STOP_SHZ		8			//停止
#define TAG_BT_HAND_SHZ		9			//手动

#define TAG_BT_AWARD_SHZ    14          //领奖池
#define TAG_BT_SEND_SHZ     15          //发送按钮
#define TAG_BT_SHOWUP_SHZ   16          //展开聊天
#define TAG_BT_SHOWDOWN_SHZ 17          //收回聊天



//播放声音
#define PLAY_SOUND_SHZ(x,b) {char sSound[32]={0}; sprintf(sSound,x);SHZSoundManager::shared()->playSound(sSound, b);}

//关闭声音
#define STOP_SOUND_SHZ(x) {char sSound[32]={0}; sprintf(sSound,x);SHZSoundManager::shared()->stopSound(sSound);}


//分数种类
const int AllScoreArr_SHZ[9][3] = { { 2000, 0, 0 }, { 1000, 200, 50 }, { 400, 80, 20 }, { 200, 40, 15 }, { 160, 30, 10 }, { 100, 20, 7 }, { 60, 15, 5 }, { 40, 10, 3 }, { 20, 5, 2 } };

//压线种类
const int m_TypeArray_SHZ[9][5] = { { 5, 6, 7, 8, 9 }, { 0, 1, 2, 3, 4 }, { 10, 11, 12, 13, 14 }, { 0, 6, 12, 8, 4 }, { 10, 6, 2, 8, 14 }, { 0, 1, 7, 3, 4 }, { 10, 11, 7, 13, 14 }, { 5, 11, 12, 13, 9 }, { 5, 1, 2, 3, 9 } };

const int itypeAnimCount_SHZ[] = { 53, 41, 47, 36, 35, 54, 30, 44, 42 ,53};

//全盘奖倍率
const int m_ManpanScore_SHZ[9] = { 5000, 2500, 1000, 500, 400, 250, 150, 100, 50 };

//房间抽奖限制
const int m_ExpLimit[6] = { 2000, 20000, 100000, 200000, 1000000, 2000000 };

SHZCGameView0::SHZCGameView0(void)
{
	lotteryLayer = nullptr;
	m_IsZidong = false;
	m_CanHaveMoney = false;
	m_InitScreen = false;
	memset(m_typeimgstruct, 0, sizeof(m_typeimgstruct));
	memset(m_LineType, 0, sizeof(m_LineType));
	memset(m_havequan, 0, sizeof(m_havequan));
	m_iCurLine = 0;
	m_CreditScore = 0;
	m_ScrollState = false;
	m_IsYanshi = true;
	ThisScore = 0;
	m_CellScore = 0;
	m_ThreeMaLiCount = 0;
	for (int i = 0; i<15; i++)
	{
		m_typeid[i] = 0;
		m_typeimgstruct[i].m_typeid = 0;
		m_typeimgstruct[i].m_have = true;
	}
	m_ThisScore = 0;

	m_bTwinkle = true;

	m_bPlayTypeAnim = true;

	m_bQuanpan = false;
	m_bGetScore = false;
	m_bAllStop = false;

	m_goldDownLayer = NULL;


	/*-----------修改bysl---------*/
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(ROOMLIST_PLIST_SHZ);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("mainscene/gold2.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shuihu/Game/red_dragon.plist");	
}

SHZCGameView0* SHZCGameView0::create()
{
	SHZCGameView0* layer = new SHZCGameView0();
	if (layer && layer->init())
	{
		layer->autorelease();

		return layer;
	}

	delete layer;

	return 0;
}

SHZCGameView0::~SHZCGameView0(void)
{
	/*-----------修改bysl---------*/
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(ROOMLIST_PLIST_SHZ);
	//CCTextureCache::sharedTextureCache()->removeTextureForKey(ROOMLIST_CZZ_SHZ);

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("mainscene/gold2.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("mainscene/gold2.png");


	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("shuihu/Game/red_dragon.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("shuihu/Game/red_dragon.png");
}

bool SHZCGameView0::init()
{
	//预加载音效
	//SHZSoundManager::shared()->preloadEffect("defen");
	//SHZSoundManager::shared()->preloadEffect("shangfen");

	float bgX = kRevolutionWidth_SHZ / 2;
	float bgY = kRevolutionHeight_SHZ / 2;

	do
	{
		m_ptline[0].setPoint(535, 370);
		m_ptline[1].setPoint(535, 505);
		m_ptline[2].setPoint(535, 230);
		m_ptline[3].setPoint(535, 545 - 175);
		m_ptline[4].setPoint(535, 190 + 175);
		m_ptline[5].setPoint(535, 410);
		m_ptline[6].setPoint(535, 325);
		m_ptline[7].setPoint(535, 410 - 90);
		m_ptline[8].setPoint(535, 325 + 90);


		m_ptLight[0].setPoint(32, 408);
		m_ptLight[1].setPoint(32, 492);
		m_ptLight[2].setPoint(32, 234);
		m_ptLight[3].setPoint(32, 536);
		m_ptLight[4].setPoint(32, 190);
		m_ptLight[5].setPoint(32, 453);
		m_ptLight[6].setPoint(32, 280);
		m_ptLight[7].setPoint(32, 365);
		m_ptLight[8].setPoint(32, 322);

		m_TypeImgeTexture = CCNode::create();
		addChild(m_TypeImgeTexture, 5);
		houseIndex = SHZGetHouseIndex::getInstance()->getIndex();
		if (0 == DF::shared()->GetGameType() % 2){
			char str[50];
			char str1[50];
			sprintf(str, "gb_top_0%d.png", houseIndex);
			sprintf(str1, "gb_bottom_0%d.png", houseIndex);

			if (0 == houseIndex)
			{
				CD::GetInstance()->StartPlay(houseIndex);
				this->addChild(CD::GetInstance(), 2);				
			}
			else if (1 == houseIndex)
			{
				CD::GetInstance()->StartPlay(houseIndex);
				this->addChild(CD::GetInstance(), 2);
			}
			else if (2 == houseIndex)
			{
				auto snow = ParticleSnow::create();
				snow->setPosition(Vec2(bgX, bgY * 2));
				snow->setTotalParticles(400);
				snow->setDuration(-1);
				snow->setAutoRemoveOnFinish(true);
				this->addChild(snow, 2);
			}
			/*-----更换房间背景---------*/
			auto top = cocos2d::CCSprite::createWithSpriteFrameName(str);
			auto body = cocos2d::Sprite::createWithSpriteFrameName(str1);
			top->setPosition(Vec2(bgX, bgY * 2 - 85));
			body->setPosition(Vec2(bgX, bgY - 63));
			addChild(top);
			addChild(body, 10);
		}
		else
		{
			m_BgImgeTexture = cocos2d::CCSprite::createWithSpriteFrameName("BG_0.png");
			m_BgImgeTexture->setPosition(Vec2(bgX, bgY));
			addChild(m_BgImgeTexture, 10);
		}


		/*---------修改bysl----------*/


		//金币数背景
		Sprite* gold = Sprite::createWithSpriteFrameName("sr_gold.png");
		gold->setPosition(Vec2(145, 695));
		this->addChild(gold, 100);
		//元宝数背景
		Sprite* ingot = Sprite::createWithSpriteFrameName("sr_-ingot.png");
		ingot->setPosition(Vec2(400, 695));
		this->addChild(ingot, 100);

		Sprite* scoreboard = Sprite::createWithSpriteFrameName("sg_score.png");
		scoreboard->setPosition(Vec2(220, 80));
		this->addChild(scoreboard, 100);

		cocos2d::CCSprite* ImageGunDong = cocos2d::CCSprite::createWithSpriteFrameName("GUNDONG.png");
		m_GunDongImgeTexture = cocos2d::CCSpriteBatchNode::createWithTexture(ImageGunDong->getTexture());
		m_GunDongImgeTexture->setVisible(true);

		//绘制裁剪区域
		DrawNode* shap = DrawNode::create();
		shap->drawSolidRect(Vec2(0, 0), Vec2(975, 405), ccc4f(255, 255, 255, 255));       //仅表示方框的大小
		ClippingNode* cliper = ClippingNode::create();
		cliper->setStencil(shap);
		cliper->setInverted(false);
		cliper->setPosition(Vec2(12, 170));       //其锚点为左下角
		this->addChild(cliper, 3);
		cliper->addChild(m_GunDongImgeTexture);

		cocos2d::CCSprite* ImageLine = cocos2d::CCSprite::createWithSpriteFrameName("line0.png");
		m_LineTexture = cocos2d::CCSpriteBatchNode::createWithTexture(ImageLine->getTexture());
		m_LineTexture->setVisible(false);
		addChild(m_LineTexture, 200);

		cocos2d::CCSprite* ImageLight = cocos2d::CCSprite::createWithSpriteFrameName("light1.png");
		m_LightTexture = cocos2d::CCSpriteBatchNode::createWithTexture(ImageLight->getTexture());
		m_LightTexture->setVisible(false);
		addChild(m_LightTexture, 200);

		// 卡片的框框， 带闪烁的
		m_BoxNode = CCNode::create();
		addChild(m_BoxNode, 50);


		//------文字提示------------

		//win
		m_winscoreViewTexture = cocos2d::CCSprite::createWithSpriteFrameName("win.png");
		m_winscoreViewTexture->setPosition(Vec2(kRevolutionWidth_SHZ / 2, kRevolutionHeight_SHZ / 2));//350,275));
		m_winscoreViewTexture->setVisible(false);
		addChild(m_winscoreViewTexture, 200);

		m_bibeiViewTexture = cocos2d::CCSprite::createWithSpriteFrameName("bibei.png");
		m_bibeiViewTexture->setPosition(Vec2(530, 475));
		m_bibeiViewTexture->setVisible(false);
		addChild(m_bibeiViewTexture, 200);

		m_defenViewTexture = cocos2d::CCSprite::createWithSpriteFrameName("defen.png");
		m_defenViewTexture->setPosition(Vec2(530, 475));
		m_defenViewTexture->setVisible(false);
		addChild(m_defenViewTexture, 200);

		m_tishiTexture = cocos2d::CCSprite::createWithSpriteFrameName("Tishi.png");
		m_tishiTexture->setPosition(Vec2(bgX, bgY + 100));
		m_tishiTexture->setVisible(false);
		addChild(m_tishiTexture, 500);

		/*------------修改bysl----------*/

		//字体
		//显示得分label
		m_lbScore2 = LabelAtlas::create("", GAME_NUMBER4, 20,25,'0');
		m_lbScore2->setAnchorPoint(Vec2(0.5, 0.5));
		m_lbScore2->setPosition(Vec2(250, 61));
		m_lbScore2->setString("0");
		addChild(m_lbScore2, 100);


		//总押分label
		m_lbScore5 = LabelAtlas::create("", GAME_NUMBER4, 20, 25, '0');
		m_lbScore5->setAnchorPoint(Vec2(0.5, 0.5));
		m_lbScore5->setPosition(Vec2(250, 93));
		m_lbScore5->setString("0");
		addChild(m_lbScore5, 100);

		//得分
		m_lbwinScore = CCLabelBMFont::create("0123456789", "bmfonts/winScore.fnt");
		m_lbwinScore->setPosition(Vec2(kRevolutionWidth_SHZ / 2/*620*/, 295));
		m_lbwinScore->setString("0");
		m_lbwinScore->setVisible(false);
		addChild(m_lbwinScore, 200);

		
		//创建聊天室
		//this->CreateChatRoom();

		//创建按钮
		CreatedGameBt();

		//创建领奖池
		this->createProgress();

		//设置滚动
		SetScrollState(true);

		schedule(schedule_selector(SHZCGameView0::On1000Timer), 0.1f);


	} while (0);

	return true;
}
//创建领奖进度条
void SHZCGameView0::createProgress()
{

	LotteryMission* pMission = LotteryMission::getInstance();
	//抽奖积分显示
	char lottery[50];
	m_ExpScore = pMission->getLotteryInfo().lShExpLottery;
	log("----------------------m_expScore = %ld-------------",m_ExpScore);
	sprintf(lottery, "%ld", m_ExpScore);
	m_lbaward = LabelAtlas::create(lottery, GAME_NUMNER2, 15,22,'0');
	m_lbaward->setPosition(Vec2(125, 10));
	m_btPool->addChild(m_lbaward, 4);


	//设置宝箱是否可以闪动的标志
	if (m_ExpScore >= m_ExpLimit[houseIndex])
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	//抽奖积分百分比显示
	char bit[100];
	sprintf(bit, "%lu/%lu", m_ExpScore, m_ExpLimit[houseIndex]);
	m_lbshow = LabelTTF::create(bit, "Arial", 20, CCSizeMake(140, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	m_lbshow->setFontFillColor(Color3B(255,255,255));
	m_lbshow->setVisible(true);
	m_lbshow->setPosition(Vec2(158, 60));

	m_btPool->addChild(m_lbshow, 5);
	//领奖进度条
	bg = Sprite::createWithSpriteFrameName("sg_bar0.png");
	bg->setPosition(Vec2(165, 58));
	m_btPool->addChild(bg,4);

	s_progressBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("sg_bar1.png"));
	s_progressBar->setType(kCCProgressTimerTypeBar);
	s_progressBar->setPosition(Vec2(86, 18));
	s_progressBar->setPercentage(0);
	s_progressBar->setMidpoint(Vec2(0, 0));
	s_progressBar->setBarChangeRate(Vec2(1, 0));

	bg->addChild(s_progressBar);
	

	//提示可以抽奖的闪动
	boxLight = Sprite::createWithSpriteFrameName("sg_box2.png");
	boxLight->setPosition(37, 47);
	m_btPool->addChild(boxLight);

	this->arriveLottery();
}

//记录玩家更新的经验
void SHZCGameView0::setLotteryExp(int myExp)
{
	m_ExpScore += myExp ;
	if (m_ExpScore > 1000000)
	{
		m_ExpScore = 1000000;
	}
}


//更新玩家获取的经验
void SHZCGameView0::updateLotteryExp()
{
	char lottery[50];
	sprintf(lottery, "%d", m_ExpScore);
	m_lbaward->setString(lottery);
	this->arriveLottery();
}
//抽奖百分比显示与否
void SHZCGameView0::arriveLottery()
{	
	if (m_ExpScore >= m_ExpLimit[houseIndex])
	{
		m_lbshow->setVisible(false);		
		bg->setVisible(false);
		
		if (flag)
		{
			boxLight->setVisible(true);
			tiShi->setVisible(true);
			auto blink = Blink::create(1, 1);
			boxLight->runAction(RepeatForever::create(blink));
			flag = false;
		}
		else
		{

		}
	}
	else
	{
		//抽奖积分百分比显示更新
		char bit[100];
		sprintf(bit, "%lu/%lu", m_ExpScore, m_ExpLimit[houseIndex]);
		m_lbshow->setVisible(true);
		m_lbshow->setString(bit);
		//宝箱闪动停止		
		if (!flag)
		{
			boxLight->stopAllActions();
			boxLight->setVisible(false);
			tiShi->setVisible(false);
			flag = true;
		}
		else
		{

		}
		//进度条更新
		bg->setVisible(true);
		ProgressTo* to = ProgressTo::create(1, m_ExpScore * 100 / m_ExpLimit[houseIndex]);
		s_progressBar->runAction(to);
		

	}
}


/*-----------修改bysl---------*/
void SHZCGameView0::CreateChatRoom()
{
	//创建输入框背景
	chatLabel = Sprite::createWithSpriteFrameName("sg_input.png");
	chatLabel->setPosition(Vec2(270, 30));
	addChild(chatLabel, 10000);

	//发送按钮
	m_btSend = createButton_SHZ("sg_set0.png", "sg_set1.png", "sg_set1.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btSend->setTag(TAG_BT_SEND_SHZ);
	m_btSend->setPosition(Vec2(450, 30));
	addChild(m_btSend, 10000);

	//创建显示栏
	showBoard = Sprite::createWithSpriteFrameName("sg_talk0.png");
	showBoard->setPosition(Vec2(270, 70));
	showBoard->setVisible(true);
	addChild(showBoard, 100);

	//发送按钮上升
	m_btUp = createButton_SHZ("sg_out.png", "sg_out.png", "sg_out.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btUp->setTag(TAG_BT_SHOWUP_SHZ);
	m_btUp->setPosition(Vec2(413, 105));
	showBoard->addChild(m_btUp);

	//创建显示栏展开
	spreadBoard = Sprite::createWithSpriteFrameName("sg_talk1.png");
	spreadBoard->setVisible(false);
	spreadBoard->setPosition(Vec2(223, -60));

	//发送按钮下降
	m_btDown = createButton_SHZ("sg_in.png", "sg_in.png", "sg_in.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btDown->setTag(TAG_BT_SHOWDOWN_SHZ);
	m_btDown->setPosition(Vec2(413, 361));
	spreadBoard->addChild(m_btDown);

	//绘制裁剪区域
	DrawNode* shap = DrawNode::create();
	shap->drawSolidRect(Vec2(0, 0), Vec2(445, 6), ccc4f(255, 255, 255, 255));       //仅表示方框的大小
	ClippingNode* cliper = ClippingNode::create();
	cliper->setStencil(shap);
	cliper->setInverted(true);
	cliper->setPosition(Vec2(47, 0));       //其锚点为左下角
	this->addChild(cliper, 200);
	cliper->addChild(spreadBoard);

}

void SHZCGameView0::createCoin()
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
	
	//去掉粒子特效
	//auto act1 = CallFunc::create(this,callfunc_selector(SHZCGameView0::playEff));
	//auto seq = Sequence::create(DelayTime::create(0.4f),act1,NULL);
	//this->runAction(seq);
}

//粒子特效
void SHZCGameView0::playEff()
{
	//_PLAY_SOUND_SHZ(GOLDWIN);
	auto particle = ParticleSystemQuad::create("images/common/Eff/baoxiangbao.plist");
	particle->setScale(1.2f);
	particle->setPosition(Vec2(537, 370));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(1.0f);									// 设置粒子系统的持续时间秒  
	this->addChild(particle,999);
}

//龙飞舞的动画
void SHZCGameView0::dragonRun()
{
	auto dragon = Sprite::create();
	dragon->setPosition(Vec2(kRevolutionWidth_SHZ / 2, kRevolutionHeight_SHZ / 2));
	dragon->setScale(2.0f);
	this->addChild(dragon,199);
	SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int i = 1; i < 13; i++)
	{
		sprintf(str,"%02d.png",i);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame, 1, mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}
	}

	cocos2d::CCAnimation *animation = CCAnimation::create(animFrames, 0.12f);
	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);
	auto call = CallFuncN::create(this, callfuncN_selector(SHZCGameView0::removeCoinFly));
	auto seq = Sequence::create(animate, call, NULL);
	dragon->runAction(seq);
}



void SHZCGameView0::createCoinFly(Node* sp, int m)
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
	Action* call = CallFuncN::create(pSprite, callfuncN_selector(SHZCGameView0::removeCoinFly));
	Action* call2 = CallFuncN::create(this, callfuncN_selector(SHZCGameView0::coinChange));
	auto seq = Sequence::create(act1,act2, call,call2, NULL);
	pSprite->runAction(seq);
	pSprite->runAction(RepeatForever::create(animate));	
}
void SHZCGameView0::coinChange(Node* obj)
{
	//添加金币
	auto newSp = Sprite::createWithSpriteFrameName("sg_gold.png");
	newSp->setPosition(Vec2(50, 695));
	this->addChild(newSp, 101);
	auto seq = Sequence::create(ScaleTo::create(0.1, 1.3), CallFunc::create([newSp](){newSp->removeFromParentAndCleanup(true); }), NULL);
	newSp->runAction(seq);
}
void SHZCGameView0::removeCoinFly(Node* obj)
{
	Sprite* sp = (Sprite*)obj;
	sp->stopAllActions();
	sp->removeFromParent();
	
}





void SHZCGameView0::removeCallback(cocos2d::CCNode *obj)
{
	((CCSprite*)obj)->removeFromParent();
}
//调用winscore
void SHZCGameView0::getWinScore()
{
	GetWinScore(0);
}

void SHZCGameView0::onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	Control* ctr = (Control*)obj;

	//控件判断
	switch (ctr->getTag())
	{
	case TAG_BT_SHOWUP_SHZ:
	{
						  showBoard->setVisible(false);
						  spreadBoard->setVisible(true);
						  auto up = MoveTo::create(0.5, Vec2(223, 200));
						  spreadBoard->runAction(up);
						  break;
	}
	case TAG_BT_SHOWDOWN_SHZ:
	{
							auto seq = Sequence::create(MoveTo::create(0.5, Vec2(223, -60)), CallFunc::create([this](){
								this->showBoard->setVisible(true);
								this->spreadBoard->setVisible(false);
							}), NULL);
							spreadBoard->runAction(seq);

							break;
	}
	case TAG_BT_SEND_SHZ:
	{
						log("cdjaskvfhjdokashfja");
						break;
	}


	case TAG_BT_HAND_SHZ:
	{
						if (1 == DF::shared()->GetQuanPanState()) return;

						m_IsZidong = false;

						m_btZidong->setVisible(true);
						m_btshoudong->setVisible(false);


						if (m_ScrollState)
						{
							m_btStop->setVisible(true);

						}
						else
						{

							if (!m_bGetScore)
							{
								m_btGetScore->setEnabled(true);
								m_btGetScore->setVisible(true);
							}
							else{
								m_btStart->setVisible(true);
								m_btStart->setEnabled(true);
								m_btGetScore->setVisible(false);
							}
							m_btGetScore->setEnabled(m_winscoreViewTexture->isVisible());
							m_btAddScore->setEnabled(true);
						}
						m_tishiTexture->setVisible(false);
						break;
	}
	case TAG_BT_ZIDONG_SHZ:
	{
							  _PLAY_SOUND_SHZ(PRESS);
						  if (m_CreditScore<m_CellScore*m_iCurLine)
						  {
							  //popup_SHZ(a_u8("筹码不足"), a_u8("筹码不足，请充值"));
							  DiableGamePopup();

							  return;
						  }

						  m_IsZidong = true;

						  m_btZidong->setVisible(false);

						  m_btshoudong->setVisible(true);

						  m_tishiTexture->setVisible(true);

						  if (m_ScrollState)
						  {
							  return;
						  }

						  m_btAddScore->setEnabled(false);
						  m_btStart->setEnabled(false);
						  m_btbibei->setEnabled(false);

						  if (ThisScore>0)
						  {
							  m_iTypeScrollTag = TYPESCROLLSTOP_SHZ + 6;
							  schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 3.0f);
						  }
						  else
						  {
							  if (m_CreditScore >= m_CellScore*m_iCurLine)
							  {
								  m_iTypeScrollTag = TYPESCROLLSTOP_SHZ + 5;
								  schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 2.0f);

							  }
							  else
							  {
								  m_btZidong->setVisible(true);

								  m_btshoudong->setVisible(false);
							  }
						  }
						  break;
	}
	case TAG_BT_SCORE_SHZ:
	{
						 if (NULL != m_goldDownLayer)
						 {
							 // 手动减少引用计数器
							 m_goldDownLayer->release();
							 m_goldDownLayer = NULL;
						 }

						 m_btStart->setVisible(true);
						 m_btGetScore->setVisible(false);
						 m_btbibei->setEnabled(false);
						 m_LightTexture->removeAllChildren();
						 m_LineTexture->removeAllChildren();
						 //金币飞出效果
						 auto seq = Sequence::create(CallFunc::create(this, callfunc_selector(SHZCGameView0::createCoin)), CallFunc::create(this, callfunc_selector(SHZCGameView0::getWinScore)), NULL);
						 this->runAction(seq);
						 unschedule(schedule_selector(SHZCGameView0::OnbibeiordefenTimer));
						 break;
	}
	case TAG_BT_ADD_SHZ:
	{
					   _PLAY_SOUND_SHZ(PRESS);
					   m_btZidong->setEnabled(true);
					   ChangeCellScore();

					   break;
	}
	case TAG_BT_BIBEI_SHZ:
	{
						 _PLAY_SOUND_SHZ(PRESS);
						 m_iGameViewState = 1;
						 m_CanHaveMoney = false;
						 m_btbibei->setEnabled(false);
						 m_btGetScore->setEnabled(false);
						 m_btZidong->setEnabled(false);

						 m_LightTexture->removeAllChildren();
						 m_LineTexture->removeAllChildren();

						 SHZSoundManager::shared()->stopMusic();

						 PLAZZ_PRINTF("TAG_BT_BIBEI_SHZ");

						 GetBiBeiData();
						 ChangeSceneAnim();

						 m_winscoreViewTexture->setVisible(false);
						 m_lbwinScore->setVisible(false);
						 m_bibeiViewTexture->setVisible(false);
						 m_defenViewTexture->setVisible(false);
						 unschedule(schedule_selector(SHZCGameView0::OnbibeiordefenTimer));

						 break;
	}
	case TAG_BT_START_SHZ:
	{
						_PLAY_SOUND_SHZ(OTHERPRESS);
						 if (m_CreditScore<m_CellScore*m_iCurLine)
						 {
							 //popup_SHZ(a_u8("筹码不足"), a_u8("筹码不足，请充值"));
							 DiableGamePopup();

							 return;
						 }

						 m_btAddScore->setEnabled(false);

						 //将手动和自动设置为不可点击
						 m_btshoudong->setEnabled(false);
						 m_btZidong->setEnabled(false);

						 m_IsZidong = false;
						 //发送开始滚动信息

						 unschedule(schedule_selector(SHZCGameView0::OnTwinkleTimer));
						 kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_TYPESCROLL_SHZ);

						 m_btStart->setEnabled(false);
						 break;
	}
	case TAG_BT_STOP_SHZ:
	{
						m_bAllStop = true;

						STOP_SOUND_SHZ("gundong");
						PLAY_SOUND_SHZ("gundong_1", false);

						unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
						m_btStop->setVisible(false);
						m_btStart->setVisible(true);
						m_ScrollState = false;
						m_TypeImgeTexture->removeAllChildren();
						for (int i = 0; i<15; i++)
							m_typeimgstruct[i].m_have = true;
						for (int i = 0; i<15; i++)
						{
							SetTypeImg(i);
						}

						ChecKLine();
						TypeFlash();
						break;
	}
	case TAG_BT_AWARD_SHZ:
	{
							 _PLAY_SOUND_SHZ(OTHERPRESS);
							 //获取抽奖的等级
							 int type = getDegreeType();
							 if (-1 == type)
							 {
								 auto dialog = Sprite::createWithSpriteFrameName("sg_tips.png");
								 dialog->setPosition(Vec2(355,80));
								 m_btPool->addChild(dialog);

								 auto act1 = FadeOut::create(1.0f);
								 auto seq = Sequence::create(DelayTime::create(2.0f), act1, CallFunc::create([dialog](){
									 dialog->removeFromParentAndCleanup(true);
								 }),NULL);
								 dialog->runAction(seq);
							 }
							 else
							 {
								
								 lotteryLayer = DrawMain::createLayer(type,m_ExpScore,houseIndex);
								 lotteryLayer->setTag(1000);  
								 Director::getInstance()->getRunningScene()->addChild(lotteryLayer, 1000);
					
								 break;
							 }
							 
	}
	}
}

//判断抽奖等级
int SHZCGameView0::getDegreeType()
{
	int type = 0;
	if (m_ExpScore < 2000)
	{
		type = -1;
		
	}
	else if (m_ExpScore >= m_ExpLimit[0] && m_ExpScore < m_ExpLimit[1])
	{
		if (houseIndex < 1)
		{
			type = 0;
		}
		else
		{
			type = -1;
		}
		
	}
	else if (m_ExpScore >= m_ExpLimit[1] && m_ExpScore <  m_ExpLimit[2])
	{
		if (houseIndex <= 1)
		{
			type = 1;
		}
		else
		{
			type = -1;
		}

	}
	
	else if (m_ExpScore >= m_ExpLimit[2] && m_ExpScore <  m_ExpLimit[3])
	{
		if (houseIndex <= 1)
		{
			type = 2;
		}
		else
		{
			type = -1;
		}
	
	}
	else if (m_ExpScore >= m_ExpLimit[3] && m_ExpScore <  m_ExpLimit[4])
	{
		type = 3;
	}
	else if (m_ExpScore >= m_ExpLimit[4] && m_ExpScore <  m_ExpLimit[5])
	{
		type = 4;
	}
	else 
	{
		type = 5;
	}
	return type;
}

//设置滚动
void SHZCGameView0::SetScrollState(bool bScroll)
{
	if (bScroll)
	{
		for (int i = 0; i<5; i++)
		{
			CCSprite* sprite = cocos2d::CCSprite::createWithSpriteFrameName("GUNDONG.png");
			sprite->setPosition(Vec2(160 + i * 185, 700));
			CCActionInterval * a1 = CCMoveTo::create(0.8f, Vec2(160 + i * 185, -340));

			CCActionInterval*  action1 = CCRepeatForever::create(
				//动作组:先在位置显示，然后执行动作a1
				CCSequence::create(CCPlace::create(Vec2(160 + i * 185, 700)), a1, NULL));

			sprite->runAction(action1);

			m_GunDongImgeTexture->addChild(sprite);
		}
	}
	else
	{
		//清除
		m_GunDongImgeTexture->removeAllChildren();
	}


}

void SHZCGameView0::setCreditScore(int iCreditScore)
{
	this->m_CreditScore = iCreditScore;
	PLAZZ_PRINTF("Creditscore:%d", this->m_CreditScore);
}

//设置下注
void SHZCGameView0::SetXiaZhu(int allcellscore)
{
	m_CellScore = allcellscore;

	char strScore[32];

	sprintf(strScore, "%d", allcellscore*m_iCurLine);

	m_lbScore5->setString(strScore);

	//char sSound[32] = { 0 };
	//sprintf(sSound, "shangfen");
	//SHZSoundManager::shared()->playSound(sSound);

}


//设置压线
void SHZCGameView0::SetYaLine(int cline)
{
	m_iCurLine = cline;
	ShowLine();
	ShowLight();
}

//闪烁
void SHZCGameView0::OnTwinkleTimer(float dt)
{
	m_TypeImgeTexture->removeAllChildren();
	m_BoxNode->removeAllChildren();

	if (0 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{

			if (i >= 5 && i <= 9)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{

				SetTypeImgBack(i);

				m_typeimgstruct[i].m_havequan = false;

			}
		}
	}
	else if (1 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 5 && i <= 8)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (2 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 5 && i <= 7)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (3 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 6 && i <= 9)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (4 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 7 && i <= 9)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (5 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 0 && i <= 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (6 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 0 && i <= 3)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (7 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 0 && i <= 2)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (8 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 1 && i <= 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (9 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 2 && i <= 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (10 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 10 && i <= 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);
			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (11 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 10 && i <= 13)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);
			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (12 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 10 && i <= 12)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);
			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (13 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 11 && i <= 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);
			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (14 == m_iTypeTimeTag)
	{
		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i >= 12 && i <= 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);
			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (15 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 0 || i == 6 || i == 12 || i == 8 || i == 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (16 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 0 || i == 6 || i == 12 || i == 8)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (17 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 0 || i == 6 || i == 12)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (18 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 6 || i == 12 || i == 8 || i == 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (19 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 12 || i == 8 || i == 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (20 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 10 || i == 6 || i == 2 || i == 8 || i == 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (21 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 10 || i == 6 || i == 2 || i == 8)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (22 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 10 || i == 6 || i == 2)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (23 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 6 || i == 2 || i == 8 || i == 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (24 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 2 || i == 8 || i == 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (25 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 0 || i == 1 || i == 7 || i == 3 || i == 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (26 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 0 || i == 1 || i == 7 || i == 3)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (27 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 0 || i == 1 || i == 7)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (28 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 1 || i == 7 || i == 3 || i == 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (29 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 7 || i == 3 || i == 4)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (30 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 10 || i == 11 || i == 7 || i == 13 || i == 14)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (31 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 10 || i == 11 || i == 7 || i == 13)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (32 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 10 || i == 11 || i == 7)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;
			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (33 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 11 || i == 7 || i == 13 || i == 14)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (34 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 7 || i == 13 || i == 14)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (35 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 5 || i == 11 || i == 12 || i == 13 || i == 9)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (36 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 5 || i == 11 || i == 12 || i == 13)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (37 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 5 || i == 11 || i == 12)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (38 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 11 || i == 12 || i == 13 || i == 9)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (39 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 12 || i == 13 || i == 9)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (40 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 5 || i == 1 || i == 2 || i == 3 || i == 9)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (41 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 5 || i == 1 || i == 2 || i == 3)
			{
				SetTypeAnim(i);
				m_typeimgstruct[i].m_index = 2;

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (42 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 5 || i == 1 || i == 2)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (43 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 1 || i == 2 || i == 3 || i == 9)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}
	else if (44 == m_iTypeTimeTag)
	{

		//闪烁第一条线
		for (int i = 0; i<15; i++)
		{
			if (i == 2 || i == 3 || i == 9)
			{
				m_typeimgstruct[i].m_index = 2;
				SetTypeAnim(i);

			}
			else
			{
				SetTypeImgBack(i);
				m_typeimgstruct[i].m_havequan = false;
			}
		}
	}


	unschedule(schedule_selector(SHZCGameView0::OnTwinkleTimer));

}

void SHZCGameView0::On1000Timer(float dt)
{

	if (m_InitScreen)
	{
		unschedule(schedule_selector(SHZCGameView0::On1000Timer));

		//自动播放动画

		for (int i = 0; i<15; i++)
		{
			m_typeimgstruct[i].m_index = 0;
			m_typeimgstruct[i].m_type = 0;
			m_typeimgstruct[i].m_CanAction = false;
			m_typeimgstruct[i].m_Cons = 0;
			m_typeimgstruct[i].m_Rows = 0;
			m_typeimgstruct[i].m_typeid = m_typeid[i];
			m_typeimgstruct[i].m_have = false;

		}

		if (m_IsYanshi)
		{

			for (int i = 0; i<45; i++)
				m_LineType[i] = false;

			m_ScrollState = true;
			m_iTypeScrollTag = TYPESCROLLSTOP_SHZ;
			schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 0.2f);

			char sSound[32] = { 0 };
			sprintf(sSound, "shuihuchuan3");
			SHZSoundManager::shared()->playMusic(sSound);

		}
	}

	m_InitScreen = true;
}

//类型滚动
//按照列的方向设置显示的图片
void SHZCGameView0::OnTypeScrollStopTimer(float dt)
{
	if (TYPESCROLLSTOP_SHZ == m_iTypeScrollTag)
	{
		if (m_typeimgstruct[10].m_have == true)
		{
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
 			m_iTypeScrollTag = m_iTypeScrollTag + 1;
			schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 0.08f);

		}

		if (m_typeimgstruct[5].m_have == true && m_typeimgstruct[10].m_have == false)
		{
			m_typeimgstruct[10].m_have = true;
			SetTypeImg(10);
		}

		if (m_typeimgstruct[0].m_have == true && m_typeimgstruct[5].m_have == false)
		{
			m_typeimgstruct[5].m_have = true;
			SetTypeImg(5);
		}

		if (m_typeimgstruct[0].m_have == false)
		{
			m_typeimgstruct[0].m_have = true;
			SetTypeImg(0);
		}
	}
	else if (TYPESCROLLSTOP_SHZ + 1 == m_iTypeScrollTag)
	{
		if (m_typeimgstruct[11].m_have == true)
		{
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
			m_iTypeScrollTag = m_iTypeScrollTag + 1;
			schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 0.08f);
		}
		if (m_typeimgstruct[6].m_have == true && m_typeimgstruct[11].m_have == false)
		{
			m_typeimgstruct[11].m_have = true;
			SetTypeImg(11);
		}

		if (m_typeimgstruct[1].m_have == true && m_typeimgstruct[6].m_have == false)
		{
			m_typeimgstruct[6].m_have = true;
			SetTypeImg(6);
		}

		if (m_typeimgstruct[1].m_have == false)
		{
			m_typeimgstruct[1].m_have = true;
			SetTypeImg(1);
		}

	}
	else if (TYPESCROLLSTOP_SHZ + 2 == m_iTypeScrollTag)
	{
		if (m_typeimgstruct[12].m_have == true)
		{
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
			m_iTypeScrollTag = m_iTypeScrollTag + 1;
			schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 0.05f);
		}
		if (m_typeimgstruct[7].m_have == true && m_typeimgstruct[12].m_have == false)
		{
			m_typeimgstruct[12].m_have = true;
			SetTypeImg(12);
		}

		if (m_typeimgstruct[2].m_have == true && m_typeimgstruct[7].m_have == false)
		{
			m_typeimgstruct[7].m_have = true;
			SetTypeImg(7);
		}

		if (m_typeimgstruct[2].m_have == false)
		{
			m_typeimgstruct[2].m_have = true;
			SetTypeImg(2);
		}

	}
	else if (TYPESCROLLSTOP_SHZ + 3 == m_iTypeScrollTag)
	{
		if (m_typeimgstruct[13].m_have == true)
		{
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
			m_iTypeScrollTag = m_iTypeScrollTag + 1;
			schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 0.06f);
		}
		if (m_typeimgstruct[8].m_have == true && m_typeimgstruct[13].m_have == false)
		{
			m_typeimgstruct[13].m_have = true;
			SetTypeImg(13);
		}
		if (m_typeimgstruct[3].m_have == true && m_typeimgstruct[8].m_have == false)
		{
			m_typeimgstruct[8].m_have = true;
			SetTypeImg(8);
		}
		if (m_typeimgstruct[3].m_have == false)
		{
			m_typeimgstruct[3].m_have = true;
			SetTypeImg(3);
		}

	}
	else if (TYPESCROLLSTOP_SHZ + 4 == m_iTypeScrollTag)
	{
		if (m_typeimgstruct[14].m_have == true)
		{
			STOP_SOUND_SHZ("gundong");
			STOP_SOUND_SHZ("gundong_1");

			m_btStop->setVisible(false);
			m_btStart->setVisible(true);


			m_ScrollState = false;
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
			ChecKLine();
			TypeFlash();
		}
		if (m_typeimgstruct[9].m_have == true && m_typeimgstruct[14].m_have == false)
		{
			m_typeimgstruct[14].m_have = true;
			SetTypeImg(14);
		}
		if (m_typeimgstruct[4].m_have == true && m_typeimgstruct[9].m_have == false)
		{
			m_typeimgstruct[9].m_have = true;
			SetTypeImg(9);
		}
		if (m_typeimgstruct[4].m_have == false)
		{
			m_typeimgstruct[4].m_have = true;
			SetTypeImg(4);
		}


	}
	else if (TYPESCROLLSTOP_SHZ + 5 == m_iTypeScrollTag)
	{
		unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
		SHZIClientKernel * kernel = SHZIClientKernel::get();

		if (m_IsZidong)
		{
			if (m_CreditScore<m_CellScore*m_iCurLine)
			{

				m_IsZidong = false;

				m_btZidong->setVisible(true);
				m_btZidong->setEnabled(false);

				m_btshoudong->setVisible(false);


				m_btStart->setEnabled(true);

				m_tishiTexture->setVisible(false);

				m_btAddScore->setEnabled(true);

				//popup_SHZ(a_u8("筹码不足"), a_u8("筹码不足，请充值"));
				DiableGamePopup();

				return;


			}
			else
			{
				m_btStart->setEnabled(false);
				kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_TYPESCROLL_SHZ);
			}
		}
		else
		{
			if (m_CreditScore >= m_CellScore*m_iCurLine){

				m_btStart->setEnabled(true);

			}
			else{

				DiableGamePopup();
				m_btStart->setEnabled(true);
				m_tishiTexture->setVisible(false);
				m_btAddScore->setEnabled(true);
			}

		}


	}
	else if (TYPESCROLLSTOP_SHZ + 6 == m_iTypeScrollTag)
	{
		if (m_IsZidong)
		{
			m_btGetScore->setEnabled(false);
		}

		if (NULL != m_goldDownLayer && m_goldDownLayer->isTouched())
		{
			if (m_IsZidong)
				GetWinScore(0);
			// 手动减少引用计数器
			m_goldDownLayer->release();
			m_goldDownLayer = NULL;
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
		}
		else if (NULL == m_goldDownLayer && m_IsZidong)
		{
			GetWinScore(0);
			unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
		}
	}
}


//得分
void SHZCGameView0::GetWinScore(int gettype)
{
	m_bGetScore = false;
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	CMD_C_GetScore_SHZ getscore;
	getscore.m_GetType = gettype;
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_GETSCORE_SHZ, &getscore, sizeof(CMD_C_GetScore_SHZ));

	this->updateLotteryExp();

}


//要求 
void SHZCGameView0::GetBiBeiData()
{

	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_REQUESTBEIBEI_SHZ);

}

//加注
void SHZCGameView0::ChangeCellScore()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_ADDSCORE_SHZ);

}


//获取滚动数据
void SHZCGameView0::ScrollInformation(int winscore, int index, int imgtypeid, bool bQuanpan)
{
	m_IsYanshi = false;
	m_ThisScore = winscore;
	m_CanHaveMoney = false;

	m_typeid[index] = imgtypeid;
	m_typeimgstruct[index].m_index = 0;
	m_typeimgstruct[index].m_type = 0;
	m_typeimgstruct[index].m_CanAction = false;
	m_typeimgstruct[index].m_Cons = 0;
	m_typeimgstruct[index].m_Rows = 0;
	m_typeimgstruct[index].m_typeid = m_typeid[index];
	m_typeimgstruct[index].m_havequan = false;
	m_typeimgstruct[index].m_have = false;
	m_typeimgstruct[index].m_RoundCount = 0;

	//最后一个就开始执行
	if (index == 14)
	{
		if (!m_IsZidong)
		{
			m_btStop->setEnabled(true);
			m_btStop->setVisible(true);
		}

		m_bGetScore = false;
		m_bAllStop = false;
		m_bQuanpan = bQuanpan;
		m_btAddScore->setEnabled(false);

		m_LightTexture->removeAllChildren();
		m_LineTexture->removeAllChildren();

		m_btStart->setEnabled(false);
		m_TypeImgeTexture->removeAllChildren();
		unschedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
		unschedule(schedule_selector(SHZCGameView0::OnTypeImgStructTimer));

		for (int i = 0; i<45; i++)
			m_LineType[i] = false;

		for (int i = 0; i<15; i++)
		{
			m_typeimgstruct[i].m_index = 0;
		}

		//设置为滚动状态
		m_ScrollState = true;

		//查询排列规则是否中奖
		ChecKLine();
		m_iTypeScrollTag = TYPESCROLLSTOP_SHZ;
		schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 0.2f);

		PLAY_SOUND_SHZ("gundong", true);
	}

}

//获取赢分
void SHZCGameView0::GetWinScore(int winscore, int gametype)
{
	ThisScore = winscore;

	char strSocre[32];
	sprintf(strSocre, "%d", ThisScore);
	m_lbScore2->setString(strSocre);

	m_btGetScore->setEnabled(false);
	m_btbibei->setEnabled(false);
	m_TempCCscore = m_CreditScore;
	m_TempDwonscore = ThisScore / 10;
	if (m_TempDwonscore<1)m_TempDwonscore = 1;


	PLAY_SOUND_SHZ("defen", false);
	
	unschedule(schedule_selector(SHZCGameView0::OnbibeiordefenTimer));
	m_bibeiViewTexture->setVisible(false);
	m_defenViewTexture->setVisible(false);

	m_bGetScore = false;
	schedule(schedule_selector(SHZCGameView0::OnDwonScoreTimer), 0.05f);
}


//更新自己上分
void SHZCGameView0::SetCreditScoreScore(int myScore)
{

	m_CreditScore = myScore;

	char strScore[32];

	sprintf(strScore, "%d", m_CreditScore);

	if (!m_IsYanshi)
	{
		if (m_CreditScore <= 0)
		{
			m_CellScore = 0;

			m_btAddScore->setEnabled(false);
			m_btStart->setEnabled(false);
			m_btZidong->setEnabled(false);
			m_btGetScore->setEnabled(false);
			m_btbibei->setEnabled(false);

		}
		else
		{
			if (m_CreditScore >= m_CellScore)
			{
				m_btAddScore->setEnabled(true);
			}

		}
	}
}

//获取赢分
//形成一分一分加的效果
void SHZCGameView0::OnDwonScoreTimer(float dt)
{
	char strSocre[32];
	//游戏下分DWONSCORE_SHZ
	ThisScore = ThisScore - m_TempDwonscore;
	m_CreditScore = m_CreditScore + m_TempDwonscore;

	sprintf(strSocre, "%d", ThisScore);
	m_lbScore2->setString(strSocre);

	m_lbwinScore->setString(strSocre);


	m_bGetScore = true;

	if (ThisScore<0)
	{
		ThisScore = 0;

		sprintf(strSocre, "%d", ThisScore);
		m_lbScore2->setString(strSocre);
		m_lbwinScore->setString(strSocre);
		m_CreditScore = m_TempCCscore + m_TempTTscore;

		m_btStart->setVisible(true);
	}
	if (ThisScore == 0)
	{
		m_CanHaveMoney = false;
		m_winscoreViewTexture->setVisible(false);
		m_lbwinScore->setVisible(false);
		unschedule(schedule_selector(SHZCGameView0::OnDwonScoreTimer));

		if (m_CreditScore>0)
		{
			m_ScrollState = false;
			m_btStart->setEnabled(true);
			m_btStart->setVisible(true);
			if (m_IsZidong)
			{
				m_btStart->setEnabled(false);
				m_btAddScore->setEnabled(false);
				m_iTypeScrollTag = TYPESCROLLSTOP_SHZ + 5;
				schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 2.0f);
			}
			else
			{
				m_btAddScore->setEnabled(true);
			}

		}

	}
}


//纤细结束后的动画闪烁
void SHZCGameView0::OnTypeImgStructTimer(float dt)
{
	m_bPlayTypeAnim = true;

	//联线结束后的动画闪烁
	unschedule(schedule_selector(SHZCGameView0::OnTypeImgStructTimer));
	m_bPlayTypeAnim = true;
	bool bShow = false;

	for (int i = 0; i<15; i++)
	{
		if (!m_typeimgstruct[i].m_CanAction)continue;
		if (m_havequan[i] == true) m_typeimgstruct[i].m_havequan = true;
		m_typeimgstruct[i].m_index++;
		m_typeimgstruct[i].m_Cons++;
		m_typeimgstruct[i].m_type = 1;

		char strName[64];
		sprintf(strName, "type%d", m_typeimgstruct[i].m_typeid);
		strcat(strName, "_1_ (%d).png");

		int iInexY = 0;

		if (i >= 5 && i<10)
		{
			iInexY = 1;
		}
		if (i >= 10)
		{
			iInexY = 2;
		}
		bShow = true;
		SequenceAction(strName, callfuncND_selector(SHZCGameView0::ShowTypeAnim2), itypeAnimCount_SHZ[m_typeimgstruct[i].m_typeid], Vec2(170 + i % 5 * 185, 510 - iInexY * 135), i);
		ForeverAction(m_BoxNode, "quan%d.png", 5, Vec2(170 + i % 5 * 185, 510 - iInexY * 135));
	}

	if (!bShow)
	{
		ShowTypeAnim2(NULL, NULL);
	}

}


//设置15个类型图
void SHZCGameView0::SetTypeImg(int iPos)
{


	int iInexY = 0;

	if (iPos >= 5 && iPos<10)
	{
		iInexY = 1;
	}
	if (iPos >= 10)
	{
		iInexY = 2;
	}

	char strName[64] = "";

	sprintf(strName, "type%d_0_ (1).png", m_typeimgstruct[iPos].m_typeid);

	CCSprite* pSprite = CCSprite::createWithSpriteFrameName(strName);

	pSprite->setPosition(Vec2(170 + iPos % 5 * 185, 510 - iInexY * 135));

	m_TypeImgeTexture->addChild(pSprite);

}

//设置15个类型图黑
void SHZCGameView0::SetTypeImgBack(int iPos)
{
	//m_typeimgstruct[iPos].m_index=1;

	int iInexY = 0;

	if (iPos >= 5 && iPos<10)
	{
		iInexY = 1;
	}
	if (iPos >= 10)
	{
		iInexY = 2;
	}

	char strName[64] = "";

	sprintf(strName, "type%d_0_ (12).png", m_typeimgstruct[iPos].m_typeid);

	CCSprite* pSprite = CCSprite::createWithSpriteFrameName(strName);

	pSprite->setPosition(Vec2(170 + iPos % 5 * 185, 510 - iInexY * 135));

	m_TypeImgeTexture->addChild(pSprite);

}

//设置15个类型图赢
void SHZCGameView0::SetTypeImgWin(int iPos)
{
	if (m_typeimgstruct[iPos].m_CanAction)
	{
		SetTypeImg(iPos);

	}
	else
	{
		SetTypeImgBack(iPos);
	}
}


//设置动画播放
void SHZCGameView0::SetTypeAnim(int typeIndex)
{
	m_typeimgstruct[typeIndex].m_CanAction = true;
	m_havequan[typeIndex] = true;
	m_typeimgstruct[typeIndex].m_havequan = true;
	m_typeimgstruct[typeIndex].m_index = 1;

	char strName[64];
	sprintf(strName, "type%d", m_typeimgstruct[typeIndex].m_typeid);
	strcat(strName, "_0_ (%d).png");

	int iInexY = 0;

	if (typeIndex >= 5 && typeIndex<10)
	{
		iInexY = 1;
	}
	if (typeIndex >= 10)
	{
		iInexY = 2;
	}
	m_bTwinkle = true;

	/*char sSound[32]={0};
	sprintf(sSound, "daoguang");
	SHZSoundManager::shared()->playSound(sSound);*/

	PLAY_SOUND_SHZ("daoguang", false);

	//if (m_bAllStop) return;

	SequenceAction(strName, callfuncND_selector(SHZCGameView0::ShowTypeAnim1), 12, Vec2(170 + typeIndex % 5 * 185, 510 - iInexY * 135), typeIndex);


	CCSprite* pSprite = CCSprite::createWithSpriteFrameName("quan0.png");

	pSprite->setPosition(Vec2(170 + typeIndex % 5 * 185, 510 - iInexY * 135));

	m_BoxNode->addChild(pSprite);

}


//类型动画播放
void SHZCGameView0::ShowTypeAnim1(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	deadSpt->removeFromParentAndCleanup(true);

	if (!m_bTwinkle)
	{

		return;
	}

	m_bTwinkle = false;

	for (int j = 0; j<15; j++)
	{

		m_typeimgstruct[j].m_havequan = false;

	}

	bool ishave = false;
	for (int n = m_iTypeTimeTag + 1; n<45; n++)
	{
		if (m_LineType[n] == true)
		{
			if (!m_IsYanshi)
			{
				double a = n / 5;
				int b = floor(a);
				int MaxNum = 0;
				int c = n % 5;

				if (c == 0)
				{
					for (int i = 0; i<5; i++)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0&&m_typeid[m_TypeArray_SHZ[b][i] != 9]){ MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; break; }

					}
					if (m_bQuanpan)
					{
						ThisScore = ThisScore + m_ManpanScore_SHZ[MaxNum] * m_CellScore;

					}
					else
					{
						ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][0] * m_CellScore;
					}

				}
				else  if (c == 1)
				{
					for (int i = 0; i<4; i++)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i] != 9]){ MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; break; }

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][1] * m_CellScore;
				}
				else  if (c == 2)
				{
					for (int i = 0; i<3; i++)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i] != 9]){ MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; break; }

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][2] * m_CellScore;
				}
				else  if (c == 3)
				{
					for (int i = 4; i >= 1; i--)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i] != 9]){ MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; break; }

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][1] * m_CellScore;
				}
				else  if (c == 4)
				{
					for (int i = 4; i >= 2; i--)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i] != 9]){ MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; break; }

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][2] * m_CellScore;
				}

				PLAZZ_PRINTF("MaxNum=%d", MaxNum);

			}

			//直接显示总分
			if (ThisScore != m_ThisScore)ThisScore = m_ThisScore;
			char strSocre[32];
			sprintf(strSocre, "%d", ThisScore);

			m_lbScore2->setString(strSocre);

			ishave = true;

			m_iTypeTimeTag = n;
			schedule(schedule_selector(SHZCGameView0::OnTwinkleTimer), 0.05f);
			//CBassMusicManager::GetInstance()->Play(15,false);

			break;
		}
	}
	if (ishave == false)
	{
		if (ThisScore != m_ThisScore)ThisScore = m_ThisScore;


		schedule(schedule_selector(SHZCGameView0::OnTypeImgStructTimer), 0.05f);
		int maxIndex = 0;
		m_TypeImgeTexture->removeAllChildren();
		for (int i = 0; i<15; i++)
		{
			SetTypeImgWin(i);
		}

		bool bplaysound = false;

		for (int i = 14; i >= 0; i--)
		{
			if (m_typeimgstruct[i].m_CanAction && m_typeimgstruct[i].m_typeid != 0 && m_typeimgstruct[i].m_typeid != 9)
			{
				maxIndex = i;
				bplaysound = false;
				break;
			}

			bplaysound = true;
		}

		if (!m_IsYanshi)
		{

			PLAZZ_PRINTF("%s=%d", a_u8("播放声音"), m_typeimgstruct[maxIndex].m_typeid);

			if (bplaysound)
			{
				char sSound[32] = { 0 };
				sprintf(sSound, "shuihuzhuan");
				SHZSoundManager::shared()->playSound(sSound);
			}
			else
			{
				switch (m_typeimgstruct[maxIndex].m_typeid)
				{
				case 0:
				{
						  char sSound[32] = { 0 };
						  sprintf(sSound, "shuihuzhuan");
						  SHZSoundManager::shared()->playSound(sSound);

						  break;
				}
				case 1:
				{
						  char sSound[32] = { 0 };
						  sprintf(sSound, "zhongyitang");
						  SHZSoundManager::shared()->playSound(sSound);

						  break;
				}
				case 2:
				{
						  char sSound[32] = { 0 };
						  sprintf(sSound, "titianxingdao");
						  SHZSoundManager::shared()->playSound(sSound);

						  break;
				}
				case 3:
				{
						  char sSound[32] = { 0 };
						  sprintf(sSound, "songjiang");
						  SHZSoundManager::shared()->playSound(sSound);

						  break;
				}
				case 4:
				{
						  SHZSoundManager::shared()->playSound("linchong");
						  break;
				}
				case 5:
				{
						  SHZSoundManager::shared()->playSound("luzhisheng");
						  break;
				}
				case 6:
				{
						  SHZSoundManager::shared()->playSound("dadao");
						  break;
				}
				case 7:
				{
						  SHZSoundManager::shared()->playSound("shuangqiang");
						  break;
				}
				case 8:
				{

						  SHZSoundManager::shared()->playSound("futou");

						  break;
				}
				default:
				{
						   SHZSoundManager::shared()->playSound("linchong");

						   break;
				}
				}
			}


		}

		m_TempTTscore = ThisScore;
	}
}

//类型动画播放
void SHZCGameView0::ShowTypeAnim2(cocos2d::CCNode* node, void* data)
{
	if (node != 0)
	{
		CCSprite* deadSpt = (CCSprite*)node;

		node->stopAllActions();

		deadSpt->removeFromParentAndCleanup(true);

		int iIndex = node->getTag();
	}


	if (!m_bPlayTypeAnim)
	{
		return;
	}

	m_bPlayTypeAnim = false;


	m_BoxNode->removeAllChildren();



	for (int n = 0; n<15; n++)
	{
		SetTypeImgWin(n);

		m_typeimgstruct[n].m_index = 0;
		m_typeimgstruct[n].m_type = 0;
		m_typeimgstruct[n].m_Cons = 0;
		m_typeimgstruct[n].m_Rows = 0;
		m_typeimgstruct[n].m_havequan = false;
		m_typeimgstruct[n].m_RoundCount = 0;
	}

	m_btStart->setVisible(true);

	if (ThisScore>0)
	{
		m_btStart->setEnabled(false);
		m_btGetScore->setEnabled(true);
		m_btbibei->setEnabled(true);

		m_lbwinScore->setVisible(true);

		char strScore[32];

		sprintf(strScore, "%d", ThisScore);

		m_lbwinScore->setString(strScore);

		m_winscoreViewTexture->setVisible(true);

		m_btStart->setVisible(false);

		m_btGetScore->setVisible(true);

		schedule(schedule_selector(SHZCGameView0::OnbibeiordefenTimer), 0.5f);

		SetWinLine();


		m_btshoudong->setEnabled(true);
		m_btZidong->setEnabled(true);

		if (m_ThreeMaLiCount <= 0)
			ShowGoldDownEffect();

	}
	else
	{
		if (m_CreditScore>0 && m_CellScore>0)
		{
			m_btStart->setEnabled(true);
			m_btAddScore->setEnabled(false);
		}
	}


	if (m_IsYanshi)
	{

		SHZSoundManager::shared()->stopMusic();

		CMD_C_Line_SHZ line;

		m_iCurLine = 9;

		ShowLine();
		ShowLight();

		line.iCurLine = m_iCurLine;
		SHZIClientKernel * kernel = SHZIClientKernel::get();
		kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_LINE_SHZ, &line, sizeof(line));


		m_btZidong->setEnabled(true);
		ChangeCellScore();

		m_iAddCount++;

	}

	if (!m_IsYanshi)
		m_CanHaveMoney = true;

	//是否进入小游戏
	if (m_ThreeMaLiCount>0)
	{
		m_btAddScore->setEnabled(false);
		m_btStart->setEnabled(false);
		m_btGetScore->setEnabled(false);
		m_btbibei->setEnabled(false);
		m_CanHaveMoney = false;

		m_iGameViewState = 2;

		PLAZZ_PRINTF("%s", a_u8("小玛丽界面"));

		unschedule(schedule_selector(SHZCGameView0::OnbibeiordefenTimer));
		m_lbwinScore->setVisible(false);
		m_defenViewTexture->setVisible(false);
		m_bibeiViewTexture->setVisible(false);
		m_winscoreViewTexture->setVisible(false);


		ChangeSceneAnim();
	}
	else
	{
		m_iGameViewState = 0;

		if (m_IsZidong&&!m_IsYanshi)
		{
			m_btAddScore->setEnabled(false);
			m_btStart->setEnabled(false);
			m_btGetScore->setEnabled(false);
			m_btbibei->setEnabled(false);
			m_iTypeScrollTag = TYPESCROLLSTOP_SHZ + 6;
			schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 2.0f);

		}
		else
		{
			m_btAddScore->setEnabled(true);

		}
	}

}
//查询排列规则是否中奖
void SHZCGameView0::ChecKLine()
{
	ThisScore = 0;
	for (int j = 0; j<9; j++)
	{

		if (!m_IsYanshi)
		{
			if (j>m_iCurLine - 1)
			{
				break;
			}
		}

		int CountSame = 0;
		int tempint[6];
		for (int i = 0; i<5; i++)
		{
			tempint[i] = m_typeid[m_TypeArray_SHZ[j][i]];
		}
		tempint[5] = 100;
		for (int i = 0; i<5; i++)
		{
			if (tempint[i] != 0 && tempint[i]!=9 && tempint[i + 1] != 0 && tempint[i+1]!=9 && tempint[i + 1] != 100)
			{
				if (tempint[i] == tempint[i + 1])
					CountSame++;
				else break;
			}
			else if (tempint[i] == 0 && tempint[i + 1] != 0 &&tempint[i + 1] != 9 && tempint[i + 1] != 100)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i + 1] != 9 &&tempint[i + 1] != 0 && tempint[i + 1] != 100)
			{
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i + 1] == 0)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i + 1] == 9)
			{
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i + 1] == 9)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i + 1] == 0)
			{
				CountSame++;
			}
			else if (tempint[i] != 0 && tempint[i]!=9 && tempint[i + 1] == 0)
			{
				CountSame++;
				tempint[i + 1] = tempint[i];
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i + 1] == 9)
			{
				CountSame++;
				tempint[i + 1] = tempint[i];
			}

		}

		PLAZZ_PRINTF("%s=%d", a_u8("连线"), CountSame);

		//5连线
		if (CountSame == 4){
			m_LineType[j * 5] = true;

		}

		//4连线
		if (CountSame == 3){
			m_LineType[j * 5 + 1] = true;


		}
		//3连线
		if (CountSame == 2){
			m_LineType[j * 5 + 2] = true;

		}
		CountSame = 0;
		for (int i = 1; i<6; i++)
		{
			tempint[i] = m_typeid[m_TypeArray_SHZ[j][i - 1]];
		}
		tempint[0] = 100;
		for (int i = 5; i >= 1; i--)
		{

			if (tempint[i] != 0 &&tempint[i] != 9 && tempint[i - 1] != 0 && tempint[i - 1] != 9)
			{
				if (tempint[i] == tempint[i - 1])CountSame++;
				else break;
			}
			else if (tempint[i] == 0 && tempint[i - 1] != 0 && tempint[i-1] != 9)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i - 1] != 9 && tempint[i-1] != 0)
			{
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i - 1] == 0)
			{
				CountSame++;

			}
			else if (tempint[i] == 9 && tempint[i - 1] == 9)
			{
				CountSame++;

			}
			else if (tempint[i] == 9 && tempint[i - 1] == 0)
			{
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i - 1] == 9)
			{
				CountSame++;
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i - 1] == 0)
			{
				CountSame++;
				tempint[i - 1] = tempint[i];
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i - 1] == 9)
			{
				CountSame++;
				tempint[i - 1] = tempint[i];
			}

		}
		//4连线
		if (CountSame == 3){
			m_LineType[j * 5 + 3] = true;

		}
		//3连线
		if (CountSame == 2){
			m_LineType[j * 5 + 4] = true;
		}

	}
	//获取对应积分

}

//类型连线动画
void SHZCGameView0::TypeFlash()
{
	//开始绘制够选
	bool  isHave = false;
	for (int n = 0; n<45; n++)
	{
		if (m_LineType[n] == true)
		{
			isHave = true;
			if (!m_IsYanshi)
			{
				double a = n / 5;
				int b = floor(a);
				int MaxNum = 0;
				int c = n % 5;

				if (c == 0)
				{
					for (int i = 0; i<5; i++)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i]] != 9)
						{
							MaxNum = m_typeid[m_TypeArray_SHZ[b][i]];
							break;
						}

					}

					if (m_bQuanpan)
					{
						ThisScore = ThisScore + m_ManpanScore_SHZ[MaxNum] * m_CellScore;

					}
					else
					{
						ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][0] * m_CellScore;
					}

				}
				else  if (c == 1)
				{
					for (int i = 0; i<4; i++)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i]] != 9)
						{
							MaxNum = m_typeid[m_TypeArray_SHZ[b][i]];
							break;
						}

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][1] * m_CellScore;
				}
				else  if (c == 2)
				{
					for (int i = 0; i<3; i++)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i]] != 9)
						{ 
							MaxNum = m_typeid[m_TypeArray_SHZ[b][i]];
							break; 
						}

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][2] * m_CellScore;
				}
				else  if (c == 3)
				{
					for (int i = 4; i >= 1; i--)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i]] != 9)
						{ 
							MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; 
							break;
						}

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][1] * m_CellScore;
				}
				else  if (c == 4)
				{
					for (int i = 4; i >= 2; i--)
					{
						if (m_typeid[m_TypeArray_SHZ[b][i]]>0 && m_typeid[m_TypeArray_SHZ[b][i]] != 9)
						{ 
							MaxNum = m_typeid[m_TypeArray_SHZ[b][i]]; 
						break; 
						}

					}
					ThisScore = ThisScore + AllScoreArr_SHZ[MaxNum][2] * m_CellScore;
				}
				if (ThisScore != m_ThisScore)
				{
					ThisScore = m_ThisScore;
				}				

			}

			char strScore[32];
			sprintf(strScore, "%d", ThisScore);
			m_lbScore2->setString(strScore);

			m_iTypeTimeTag = n;
			schedule(schedule_selector(SHZCGameView0::OnTwinkleTimer), 0.05f);
			break;
		}
	 }
	if (isHave == false)
	{
		if (ThisScore != m_ThisScore)ThisScore = m_ThisScore;

		m_TypeImgeTexture->removeAllChildren();

		for (int i = 0; i<15; i++)
		{
			SetTypeImgWin(i);
		}

		m_btStart->setVisible(true);

		char strScore[32];
		sprintf(strScore, "%d", ThisScore);
		m_lbScore2->setString(strScore);


		if (m_CreditScore >= 0)
		{
			if (m_IsZidong)
			{
				m_btStart->setEnabled(false);
				m_btAddScore->setEnabled(false);

				m_iTypeScrollTag = TYPESCROLLSTOP_SHZ + 5;
				schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer));
			}
			else
			{
				m_btStart->setEnabled(true);

				m_btAddScore->setEnabled(true);

				m_btZidong->setEnabled(true);
				m_btshoudong->setEnabled(true);
			}

		}
	}
}


//切换场景动画
void SHZCGameView0::ChangeSceneAnim()
{

	PLAZZ_PRINTF("QIEHUANCHANGJ");

	float bgX = kRevolutionWidth_SHZ / 2;
	float bgY = kRevolutionHeight_SHZ / 2;

	CCSprite *qizhi1 = CCSprite::createWithSpriteFrameName("qizhi1.png");




	int iImgW = qizhi1->getTextureRect().size.width;

	qizhi1->setPosition(Vec2(0 - iImgW, bgX));

	CCActionInterval * a1 = CCMoveTo::create(2.5f, Vec2(kRevolutionWidth_SHZ + iImgW, bgX - 80));

	CCSequence* act = CCSequence::create(a1, CCCallFuncND::create(this, callfuncND_selector(SHZCGameView0::ChangeScenAnimFinish), 0), 0);

	qizhi1->setTag(101);

	qizhi1->runAction(act);

	addChild(qizhi1, 1000);

	PLAZZ_PRINTF("QIEHUANCHANGJ555");

	//----------2
	CCSprite *qizhi2 = CCSprite::createWithSpriteFrameName("qizhi2.png");

	qizhi2->setPosition(Vec2(kRevolutionWidth_SHZ + iImgW, bgX - 80));

	CCActionInterval * a2 = CCMoveTo::create(2.5f, Vec2(0 - iImgW, bgX));

	qizhi2->setTag(100);

	PLAZZ_PRINTF("QIEHUANCHANGJ44444");

	CCSequence* act2 = CCSequence::create(a2, CCCallFuncND::create(this, callfuncND_selector(SHZCGameView0::ChangeScenAnimFinish), 0), 0);

	qizhi2->runAction(act2);

	PLAZZ_PRINTF("QIEHUANCHANGJ22222");

	addChild(qizhi2, 1000);



}


//场景动作结束
void SHZCGameView0::ChangeScenAnimFinish(cocos2d::CCNode* node, void* data)
{
	PLAZZ_PRINTF("QIEHUANCHANGJ111111");
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	if (node->getTag() == 100)
	{
		if (m_iGameViewState == 1)
		{
			m_GameSceneInterface->OnChangeBibeiView();
		}
		if (m_iGameViewState == 2)
		{
			m_GameSceneInterface->OnChangeMlView();
		}
	}

	deadSpt->removeFromParentAndCleanup(true);

}


//比倍得分
void SHZCGameView0::OnbibeiordefenTimer(float dt)
{

	static bool b = true;

	if (b)
	{

		static bool bshow = true;

		if (bshow)
		{
			m_bibeiViewTexture->setVisible(false);
			m_defenViewTexture->setVisible(true);

			bshow = false;

		}
		else
		{
			m_bibeiViewTexture->setVisible(true);
			m_defenViewTexture->setVisible(false);

			bshow = true;
		}

		b = false;

	}
	else
	{
		m_bibeiViewTexture->setVisible(false);
		m_defenViewTexture->setVisible(false);

		b = true;
	}



}


//创建动作(循环播放动画)
void SHZCGameView0::ForeverAction(cocos2d::CCSprite* pSprite, const char *strName, int iCount, int iFirst, int ilayer)
{

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int i = 0; i<iCount; i++)
	{
		sprintf(str, strName, i + iFirst);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame, 1, mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}

	}

	cocos2d::CCAnimation *animation = CCAnimation::create(animFrames, 0.1f);

	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);
	//添加动作，一直循环  
	pSprite->runAction(cocos2d::CCRepeatForever::create(animate));
	addChild(pSprite, ilayer);

}


//创建动作(循环播放动画)
void SHZCGameView0::ForeverAction(CCNode* pNode, const char *strName, int iCount, CCPoint pt)
{

	CCSprite* pSprite = CCSprite::create();

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int i = 0; i<iCount; i++)
	{
		sprintf(str, strName, i);
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
	//添加动作，一直循环  
	pSprite->runAction(cocos2d::CCRepeatForever::create(animate));
	pSprite->setPosition(pt);
	pNode->addChild(pSprite);

}


//创建动作(播放动画一次)
void SHZCGameView0::SequenceAction(const char *strImage, SEL_CallFuncND selector, int iCount, CCPoint ptImage, int iTag)
{
	//创建动画
	CCSprite* pSprite = CCSprite::create();

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int i = 0; i<iCount; i++)
	{
		sprintf(str, strImage, i + 1);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame, 1, mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}

	}

	cocos2d::CCAnimation *animation = CCAnimation::create(animFrames, 0.06f);

	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);

	CCSequence* act = CCSequence::create(animate, CCCallFuncND::create(this, selector, 0), 0);// 播放动画结束

	pSprite->runAction(act);

	pSprite->setPosition(ptImage);

	addChild(pSprite, 8);

}


//创建按钮
void SHZCGameView0::CreatedGameBt()
{
	auto bgBorad = Sprite::createWithSpriteFrameName("sg_down.png");
	bgBorad->setPosition(Vec2(485,75));
	this->addChild(bgBorad, 99);
	//加注按钮
	m_btAddScore = createButton_SHZ("sg_add0.png", "sg_add1.png", "sg_add2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btAddScore->setTag(TAG_BT_ADD_SHZ);
	m_btAddScore->setEnabled(false);
	m_btAddScore->setPosition(Vec2(471, 75));
	addChild(m_btAddScore, 100);

	//比倍
	m_btbibei = createButton_SHZ("sg_bibei0.png", "sg_bibei1.png", "sg_bibei2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btbibei->setTag(TAG_BT_BIBEI_SHZ);
	m_btbibei->setEnabled(false);
	m_btbibei->setPosition(Vec2(811, 75));
	addChild(m_btbibei, 100);

	//自动按钮
	m_btZidong = createButton_SHZ("sg_automatic0.png", "sg_automatic1.png", "sg_automatic2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btZidong->setTag(TAG_BT_ZIDONG_SHZ);
	m_btZidong->setPosition(Vec2(649, 75));
	m_btZidong->setEnabled(false);
	addChild(m_btZidong, 100);

	//手动按钮
	m_btshoudong = createButton_SHZ("sg_manual0.png", "sg_manual1.png", "sg_manual2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btshoudong->setTag(TAG_BT_HAND_SHZ);
	m_btshoudong->setVisible(false);
	m_btshoudong->setPosition(Vec2(649, 75));
	addChild(m_btshoudong, 100);

	//得分按钮
	m_btGetScore = createButton_SHZ("sg_account0.png", "sg_account1.png", "sg_account2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btGetScore->setTag(TAG_BT_SCORE_SHZ);
	m_btGetScore->setEnabled(false);
	//m_btGetScore->setEnabled(false);
	m_btGetScore->setPosition(Vec2(991, 78));
	addChild(m_btGetScore, 100);

	//开始
	m_btStart = createButton_SHZ("sg_start0.png", "sg_start1.png", "sg_start2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btStart->setTag(TAG_BT_START_SHZ);
	m_btStart->setEnabled(false);
	//m_btStart->setEnabled(false);
	m_btStart->setPosition(Vec2(991, 78));
	addChild(m_btStart, 100);


	//全停
	m_btStop = createButton_SHZ("sg_stop0.png", "sg_stop1.png", "sg_stop2.png", this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btStop->setTag(TAG_BT_STOP_SHZ);
	m_btStop->setVisible(false);
	//m_btStart->setEnabled(false);
	m_btStop->setPosition(Vec2(991, 78));
	addChild(m_btStop, 100);



	/*----------修改bysl---------*/
	//创建领奖池
	m_btPool = createButton_SHZ("sg_box.png", 0, 0, this, cccontrol_selector(SHZCGameView0::onBtnClick));
	m_btPool->setTag(TAG_BT_AWARD_SHZ);
	m_btPool->setVisible(true);
	m_btPool->setPosition(Vec2(130, 605));
	addChild(m_btPool, 1000);

	//创建显示可以抽奖的提示
	tiShi = Sprite::createWithSpriteFrameName("sg_box3.png");
	tiShi->setPosition(Vec2(165, 55));
	tiShi->setVisible(false);
	m_btPool->addChild(tiShi, 5);

}


//显示界面
void SHZCGameView0::ShowView(int iCreditScore)
{
	m_CreditScore = iCreditScore;

	ThisScore = 0;

	m_lbScore2->setString("0");

	m_LightTexture->removeAllChildren();
	m_LineTexture->removeAllChildren();


	if (m_CreditScore>0)
	{
		m_ScrollState = false;

		if (m_CreditScore >= m_CellScore * 9)
		{
			m_btStart->setVisible(true);
			m_btStart->setEnabled(true);

			if (m_IsZidong)
			{
				//m_btStart->setVisible(false);
				//m_btStart->setEnabled(false);
				m_iTypeScrollTag = TYPESCROLLSTOP_SHZ + 5;
				schedule(schedule_selector(SHZCGameView0::OnTypeScrollStopTimer), 2.0f);
			}
			else
			{
				m_btAddScore->setEnabled(true);

			}
		}
		else
		{
			m_btAddScore->setEnabled(true);
			m_btStart->setVisible(true);
			m_btGetScore->setEnabled(false);
		}

	}


}


//显示线
void SHZCGameView0::ShowLine()
{
	m_LineTexture->setVisible(true);

	m_LineTexture->removeAllChildren();

	char str[32];

	for (int i = 0; i<m_iCurLine; i++)
	{
		sprintf(str, "line%d.png", i);

		CCSprite* lineSp = CCSprite::createWithSpriteFrameName(str);

		lineSp->setPosition(m_ptline[i]);

		m_LineTexture->addChild(lineSp);
	}

}

//显示灯  
void SHZCGameView0::ShowLight()
{
	m_LightTexture->setVisible(true);

	m_LightTexture->removeAllChildren();

	char str[32];

	for (int i = 0; i<m_iCurLine; i++)
	{
		sprintf(str, "light%d.png", i + 1);

		CCSprite* lightSp = CCSprite::createWithSpriteFrameName(str);

		lightSp->setPosition(m_ptLight[i]);

		m_LightTexture->addChild(lightSp);

		CCSprite* lightSp2 = CCSprite::createWithSpriteFrameName(str);

		lightSp2->setPosition(Vec2(m_ptLight[i].x + 1010, m_ptLight[i].y));

		m_LightTexture->addChild(lightSp2);
	}

}

//显示赢分线
void SHZCGameView0::SetWinLine()
{
	m_LightTexture->setVisible(true);

	m_LightTexture->removeAllChildren();
	m_LineTexture->removeAllChildren();

	for (int j = 0; j<9; j++)
	{

		if (!m_IsYanshi)
		{
			if (j>m_iCurLine - 1)
			{
				break;
			}
		}

		bool bwin = false;
		bool awin = false;
		bool dragonFlag = false;
		bool flagTwo = false;
		int CountSame = 0;
		int tempint[6];
		for (int i = 0; i<5; i++)
		{
			tempint[i] = m_typeid[m_TypeArray_SHZ[j][i]];
		}
		tempint[5] = 100;
		for (int i = 0; i<5; i++)
		{
			if (tempint[i] != 0 && tempint[i] != 9 && tempint[i + 1] != 0 && tempint[i+1] != 9 && tempint[i + 1] != 100)
			{
				if (tempint[i] == tempint[i + 1]) CountSame++;
				else break;
			}
			else if (tempint[i] == 0 && tempint[i + 1] != 0 && tempint[i + 1] != 9&&tempint[i+1]!=100)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i + 1] != 9 && tempint[i + 1] != 0 && tempint[i + 1] != 100)
			{
				dragonFlag = true;
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i + 1] == 0)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i + 1] == 9)
			{
				dragonFlag = true;
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i + 1] == 9)
			{
				dragonFlag = true;
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i + 1] == 0)
			{
				dragonFlag = true;
				CountSame++;
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i + 1] == 0)
			{
				CountSame++;
				tempint[i + 1] = tempint[i];
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i + 1] == 9)
			{
				dragonFlag = true;
				CountSame++;
				tempint[i + 1] = tempint[i];
			}
		
			//else if (tempint[i] == 9 && tempint[i + 1] != 0 && tempint[i + 1] != 9)
			//{
			//	CountSame++;
			//}

		}

		PLAZZ_PRINTF("%s=%d", a_u8("连线"), CountSame);

		//5连线
		if (CountSame >= 2){

			bwin = true;
		}

		CountSame = 0;
		for (int i = 1; i<6; i++)
		{
			tempint[i] = m_typeid[m_TypeArray_SHZ[j][i - 1]];
		}
		tempint[0] = 100;
		for (int i = 5; i >= 1; i--)
		{
			if (tempint[i] != 0 && tempint[i] != 9 && tempint[i - 1] != 0 && tempint[i - 1] != 9)
			{
				if (tempint[i] == tempint[i - 1])CountSame++;
				else break;
			}
			else if (tempint[i] == 0 && tempint[i - 1] != 0 && tempint[i - 1] != 9)
			{
				CountSame++;
			}
			else if (tempint[i] == 9 && tempint[i - 1] != 9 && tempint[i - 1] != 0)
			{
				flagTwo = true;
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i - 1] == 0)
			{
				CountSame++;

			}
			else if (tempint[i] == 9 && tempint[i - 1] == 9)
			{
				flagTwo = true;
				CountSame++;

			}
			else if (tempint[i] == 9 && tempint[i - 1] == 0)
			{
				flagTwo = true;
				CountSame++;
			}
			else if (tempint[i] == 0 && tempint[i - 1] == 9)
			{
				flagTwo = true;
				CountSame++;
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i - 1] == 0)
			{
				CountSame++;
				tempint[i - 1] = tempint[i];
			}
			else if (tempint[i] != 0 && tempint[i] != 9 && tempint[i - 1] == 9)
			{
				flagTwo = true;
				CountSame++;
				tempint[i - 1] = tempint[i];
			}
			//else if (tempint[i] == 0 && tempint[i - 1] != 0 && tempint[i - 1] != 9)
			//{
			//	CountSame++;
			//}
			//else if (tempint[i] == 9 && tempint[i - 1] != 9 && tempint[i - 1] != 0)
			//{
			//	CountSame++;
			//}
		}

		if (CountSame >= 2){

			awin = true;
		}		

		if (bwin)
		{
			if (dragonFlag)
			{
				dragonRun();
				_PLAY_SOUND_SHZ(DRAGON);
			}
			char str[32];

			sprintf(str, "light%d.png", j + 1);

			CCSprite* lightSp = CCSprite::createWithSpriteFrameName(str);

			lightSp->setPosition(m_ptLight[j]);

			m_LightTexture->addChild(lightSp);

			CCSprite* lightSp2 = CCSprite::createWithSpriteFrameName(str);

			lightSp2->setPosition(Vec2(m_ptLight[j].x + 1010, m_ptLight[j].y));

			m_LightTexture->addChild(lightSp2);

			sprintf(str, "line%d.png", j);

			CCSprite* lineSp = CCSprite::createWithSpriteFrameName(str);

			lineSp->setPosition(m_ptline[j]);

			m_LineTexture->addChild(lineSp);
		}

		else
		{
			if (awin)
			{
				if (flagTwo)
				{
					dragonRun();
					_PLAY_SOUND_SHZ(DRAGON);
				}
				char str[32];

				sprintf(str, "light%d.png", j + 1);

				CCSprite* lightSp = CCSprite::createWithSpriteFrameName(str);

				lightSp->setPosition(m_ptLight[j]);

				m_LightTexture->addChild(lightSp);

				CCSprite* lightSp2 = CCSprite::createWithSpriteFrameName(str);

				lightSp2->setPosition(Vec2(m_ptLight[j].x + 1010, m_ptLight[j].y));

				m_LightTexture->addChild(lightSp2);

				sprintf(str, "line%d.png", j);

				CCSprite* lineSp = CCSprite::createWithSpriteFrameName(str);

				lineSp->setPosition(m_ptline[j]);

				m_LineTexture->addChild(lineSp);
			}
		}
	}
}


//显示全停按钮
void SHZCGameView0::OnShowStopBt(float dt)
{
	unschedule(schedule_selector(SHZCGameView0::OnShowStopBt));

	m_btStart->setVisible(false);
	m_btStop->setVisible(true);
	m_btStop->setEnabled(true);

}


void SHZCGameView0::IsDisableView()
{
	if (m_IsZidong){

		m_btStart->setVisible(false);
		m_btStart->setEnabled(false);
		m_bGetScore = true;
	}
}

void SHZCGameView0::DiableGamePopup()
{
	popup_SHZ(a_u8("筹码不足"), a_u8("筹码不足，请充值"), 0, 0, this,callfuncN_selector(SHZCGameView0::rechargeNotice));
}
//筹码不足时的充值提示
void SHZCGameView0::rechargeNotice(Node* node)
{
	//auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
	//rechargeNode->setPosition(Vec2(0, 0));
	//this->addChild(rechargeNode, 10000);
}

void SHZCGameView0::ShowGoldDownEffect() {
	if (NULL == m_goldDownLayer && ThisScore / m_CellScore >= 80)
	{
		m_goldDownLayer = SHZGoldDownLayer::create();
		this->getParent()->addChild(m_goldDownLayer, 2000);
	}
}