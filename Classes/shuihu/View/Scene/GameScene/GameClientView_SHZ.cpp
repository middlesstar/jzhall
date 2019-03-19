#include "GameClientView_SHZ.h"
#include "ClientKernelSink_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "shuihu/View/ui/UIHelp_SHZ.h"
#include "shuihu/View/ui/UIShezhi_SHZ.h"
#include "df/LocalStore.h"
#include "shuihu/plazz/plazz/CGPRecordScoreMission_SHZ.h"
#include "shuihu/View/Scene/LotteryScene/LotteryMission_SHZ.h"
#include "shuihu/View/Scene/GetHouseIndex_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "Player/Player.h"

#include "HallGameScene.h"
#include "shuihu/Resource_SHZ.h"

#include "shuihu/View/Scene/spriteFly.h"
//按钮
#define TAG_BT_BACK		100
#define TAG_BT_SETZHI	101
#define TAG_BT_HELP		102	
#define TAG_BT_RECHARGE 105

//艺术字
#define GAME_NUMNER   "images/number/number_0.png"


/*---------修改bysl----------*/
#define ROOMLIST_PLIST_SHZ      "selectroom/room.plist"
#define ROOMLIST_CCZ        "selectroom/room.png"

//幸运抽奖后的飞金币或者元宝
#define  Lottery_coin_fly             "jinbiFly"
#define  Lottery_yuanbao_fly          "yuanbaoFly"

#define Hall_Message_Refresh_Coin			"Hall_Message_Refresh_Coin"			//刷新金币显示


USING_NS_CC;
USING_NS_CC_EXT;

const int iVipColorTable_SHZ[][3] = 
			{ 
				  { 255, 255, 255 }, 
				  {  26, 231, 255 }, 
				  {   0, 255,  66 }, 
				  { 255, 255,   0 }, 
				  { 255, 162,   0 }, 
				  { 255,  48,   0 }, 
				  { 252,   0, 255 } 
			};

long long g_llWinScore_SHZ = 0;



bool SHZGameTest::init()
{
	this->setKeypadEnabled(true);

	return true;
}

//增加在Android平台下的返回键监听


void SHZGameTest::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:

		if (NULL != m_pGameView0Ref->m_goldDownLayer) {
			if (!m_pGameView0Ref->m_goldDownLayer->isTouched())
				return;
		}

		if (NULL != m_pGameViewMaliRef->m_goldDownLayer) {
			if (!m_pGameViewMaliRef->m_goldDownLayer->isTouched())
				return;
		}

		popup_SHZ(SSTRING("exit_game").c_str(), SSTRING("exit_game_content").c_str(), DLG_MB_OK | DLG_MB_CANCEL, 0, this, callfuncN_selector(SHZGameTest::onCloseCallback));
		break;
	default:
		break;
	}
}


void SHZGameTest::onCloseCallback(cocos2d::CCNode *pNode)
{
	switch (pNode->getTag())
	{
	case DLG_MB_OK:
		{
			SHZSoundManager::shared()->stopMusic();
			if (SHZIClientKernel::get())
				SHZIClientKernel::get()->Intermit(SHZ_GameExitCode_Normal);
		}
		break;
	}
}

//构造函数
SHZCGameClientView::SHZCGameClientView()
{

	m_CreditScore=0;
	
	m_GameState = 0;
	
	m_CellScore = 0;

	m_iDuihuan=1;

	m_bPlayTypeAnim=true;

	
	G_NOTIFICATION_REG_SHZ("ON_REWARD_GAME", SHZCGameClientView::onReward, 0);

	//注册抽奖后界面结果改变通知
	//G_NOTIFICATION_REG_SHZ("LotteryLayer_COMPLETE", SHZCGameClientView::updateLotteryResult, 0);

	DF::shared()->SetGameState(true);

	/*-----------修改bysl---------*/
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(ROOMLIST_PLIST_SHZ);

	G_NOTIFICATION_REG_SHZ(Lottery_coin_fly, SHZCGameClientView::jinbiFly, nullptr);
	G_NOTIFICATION_REG_SHZ(Lottery_yuanbao_fly, SHZCGameClientView::yuanbaoFly, nullptr);
	//刷新金币（充值刷新金币，暂时先关掉）
	//HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, SHZCGameClientView::refreshCoin, nullptr);

	
	return;
}

//析构函数
SHZCGameClientView::~SHZCGameClientView()
{
	/*-----------修改bysl---------*/
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(ROOMLIST_PLIST_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(ROOMLIST_CCZ);
	DF::shared()->SetGameState(false);

	G_NOTIFICATION_UNREG_SHZ("ON_REWARD_GAME");

	//取消抽奖结果改变的通知
	//G_NOTIFICATION_UNREG_SHZ("LotteryLayer_COMPLETE");

	G_NOTIFICATION_UNREG_SHZ(Lottery_coin_fly);
	G_NOTIFICATION_UNREG_SHZ(Lottery_yuanbao_fly);

	//充值刷新，暂时先关闭
	//HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);

	//切换场景时将金币数量更新至player上
	//CMD_GP_LogonSuccess HallLogonData = Player::getInstance()->getLogonData();
	//HallLogonData.lUserScore = m_NodeGameView0->m_CreditScore;
	//Player::getInstance()->setLogonData(HallLogonData);	
}

//重置界面
void SHZCGameClientView::ResetGameView()
{
	

}

void SHZCGameClientView::onReward(cocos2d::CCObject *obj){

	SHZMTData* __data = (SHZMTData*)obj;

	if (!__data) return;

	int reward = __data->mData1;

	m_NodeGameView0->m_CreditScore += reward;



	if (DF::shared()->GetGameType() == 0){

		SHZIClientKernel* kernel = SHZIClientKernel::get();
		kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_REWARD_SHZ, &reward, sizeof(reward));
		PLAZZ_PRINTF("send");
	}
}

//调整控件
void SHZCGameClientView::RectifyControl()
{

	float nWidth=kRevolutionWidth_SHZ;
	float nHeight=kRevolutionHeight_SHZ;

}

/** 初始化 */
bool SHZCGameClientView::init()
{
	RectifyControl();

	do 
	{
		m_iCount=0;

		float bgX = kRevolutionWidth_SHZ/2;
		float bgY = kRevolutionHeight_SHZ/2;

 		m_GameTest=SHZGameTest::create();		
 		addChild(m_GameTest);

		m_NodeGameView0= SHZCGameView0::create();
		m_NodeGameView0->SetGameSceneInterface(this);
		addChild(m_NodeGameView0,99);

		//比倍场景
		m_NodeGameView1= SHZCGameViewBibei::create();
		m_NodeGameView1->SetGameSceneInterface(this);
		m_NodeGameView1->setVisible(false);
		addChild(m_NodeGameView1,98);
		
		//小玛丽场景
		m_NodeGameView2= SHZCGameViewMali::create();
		m_NodeGameView2->SetGameSceneInterface(this);
		m_NodeGameView2->setVisible(false);
		addChild(m_NodeGameView2,98);

		m_GameTest->setViewRef(m_NodeGameView0, m_NodeGameView2);


		//用户分数
		m_LbUserScore = LabelAtlas::create("", GAME_NUMNER, 19, 22, '0');
		m_LbUserScore->setAnchorPoint(Vec2(0.5,0.5));
		m_LbUserScore->setPosition(Vec2(160, 695));
		addChild(m_LbUserScore,100);  


		//用户的元宝
		m_LbLv = LabelAtlas::create("", GAME_NUMNER, 19, 22, '0');
		m_LbLv->setPosition(Vec2(423, 695));
		m_LbLv->setAnchorPoint(Vec2(0.5,0.5));
		addChild(m_LbLv,100);  

		m_GameNotcieTexture=CCSprite::createWithSpriteFrameName("GameNoticeView.png");
		m_GameNotcieTexture->setPosition(Vec2(kRevolutionWidth_SHZ/2,kRevolutionHeight_SHZ/2+230));
		m_GameNotcieTexture->setVisible(false);
		m_GameNotcieTexture->setScaleY(1.2f);
		addChild(m_GameNotcieTexture,200);

		m_TailorView = SHZTailorView::create(CCSize(900,30));
		m_TailorView->setPosition(Vec2(75, 0));
		m_GameNotcieTexture->addChild(m_TailorView);

		//开启聊天
		auto chatNode = Node::create();
		this->addChild(chatNode,1000);
		//HallGameScene::openChatSprite(chatNode);
		//创建按钮
		CreatedGameBt();  
		
		//启动更新定时
		this->scheduleUpdate();  
		//ChangeSceneAnim();
		
		  
	} while (0);
	
	return true;
}

//更新抽奖后的金币数和元宝数
void  SHZCGameClientView::updateLotteryResult(Ref* obj)
{	
    //抽奖后的经验值
	char expLottery[50];
	/*------------------todosl-----------------*/
	m_NodeGameView0->m_ExpScore = SHZGetHouseIndex::getInstance()->getExp();
	//m_NodeGameView0->m_ExpScore = 0;
	sprintf(expLottery, "%d", m_NodeGameView0->m_ExpScore);
	m_NodeGameView0->m_lbaward->setString(expLottery);

	//抽奖后的进度条
	m_NodeGameView0->arriveLottery();
}

/** 释放 */
void SHZCGameClientView::OnDeinit()
{

}

/** 恢复 */
void SHZCGameClientView::OnRestore()
{

}



/** 更新 */
void SHZCGameClientView::update(float dt)
{
	float bgX = kRevolutionWidth_SHZ/2;
	float bgY = kRevolutionHeight_SHZ/2;

	SHZIClientKernel * kernel = SHZIClientKernel::get();

	if (kernel == NULL)
	{
			return;    
	}

	if (m_GameState==0)
	{
		m_LbUserScore->setString(toString(m_NodeGameView0->m_CreditScore));
		m_LbLv->setString(toString((int)Player::getInstance()->getHallData().llYuanBao));
		m_CreditScore=m_NodeGameView0->m_CreditScore;

	}else if (m_GameState==1)
	{
		m_LbUserScore->setString(toString(m_NodeGameView1->m_CreditScore));
		m_LbLv->setString(toString((int)Player::getInstance()->getHallData().llYuanBao));
		m_CreditScore=m_NodeGameView1->m_CreditScore;

	}else
	{
		m_LbUserScore->setString(toString(m_NodeGameView2->m_CreditScore));
		m_LbLv->setString(toString((int)Player::getInstance()->getHallData().llYuanBao));
		m_CreditScore=m_NodeGameView2->m_CreditScore;
	}
}

//充值之后刷新水浒传界面
void SHZCGameClientView::refreshCoin(Ref* ref)
{
	if (m_GameState == 0)
	{
		m_NodeGameView0->m_CreditScore += Player::getInstance()->getLogonData().lUserScore;
	}
	else if (m_GameState == 1)
	{
		m_NodeGameView1->m_CreditScore += Player::getInstance()->getLogonData().lUserScore;
	}
	else
	{
		m_NodeGameView2->m_CreditScore += Player::getInstance()->getLogonData().lUserScore;
	}	
}

//弹出提示退出的对话框
void SHZCGameClientView::closeOutGame(Node* node)
{
	switch (node->getTag())
	{
	case DLG_MB_OK:
	{
					  if (DF::shared()->GetGameType() == 0){
						  SHZCGPRecordScoreMission* mission = new SHZCGPRecordScoreMission(STATIC_DATA_STRING("address"), SERVER_PORT_SHZ);
						  mission->sendRecord();

						  delete mission;
					  }

					  if (SHZIClientKernel::get())
						  SHZIClientKernel::get()->Intermit(SHZ_GameExitCode_Normal);

					  char sSound[32] = { 0 };
					  sprintf(sSound, "gundong");
					  SHZSoundManager::shared()->stopSound(sSound);
					  SHZSoundManager::shared()->stopMusic();
	}
		break;
	}
}

// 按钮点击事件
void SHZCGameClientView::onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{
	Control* ctr = (Control*)obj;

	//控件判断
	switch (ctr->getTag())
	{
	case TAG_BT_BACK:
		{
			popup_SHZ(SSTRING("exit_game").c_str(), SSTRING("exit_game_content").c_str(), DLG_MB_OK | DLG_MB_CANCEL, 0, this, callfuncN_selector(SHZCGameClientView::closeOutGame));

			return;
		}
	case TAG_BT_HELP:
		{
			SHZUIHelp* help = SHZUIHelp::create();
			help->setPosition(Vec2(kRevolutionWidth_SHZ/2,kRevolutionHeight_SHZ/2));
			addChild(help,2000);
			_PLAY_SOUND_SHZ(OTHERPRESS);
			return;
		}
	case TAG_BT_SETZHI:
		{
			WindowBase* pWindowSetting = HallGameScene::createWindow(EWT_Setting);
			addChild((Node*)pWindowSetting, 2000);
			_PLAY_SOUND_SHZ(OTHERPRESS);
			return ;
		}

	case TAG_BT_RECHARGE:
	   {							
							//auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
							//rechargeNode->setPosition(Vec2(0, 0));
							//this->addChild(rechargeNode, 2000);
							m_GameTest->comeToHall();
							_PLAY_SOUND_SHZ(PRESS);
							return;

	   }

	}

}

//绘制按钮
void SHZCGameClientView::DrawBt()
{
	

}

//抽奖完毕飞金币
void SHZCGameClientView::jinbiFly(Ref* ref)
{
	//刷新抽奖后的经验值
	updateLotteryResult(nullptr);

	spriteFly* spFly = spriteFly::create();
	spFly->setPos(Vec2(537, 370), Vec2(55, 695));
	spFly->startAni(1);
	m_NodeGameView0->addChild(spFly, 1000);

	getScore = SHZGetHouseIndex::getInstance()->getLotteryScore();
	disScore = getScore / 10;
	if (disScore < 1)   disScore = 1;

	auto seq = Sequence::create(DelayTime::create(1.5f),
		CallFunc::create([this](){this->schedule(schedule_selector(SHZCGameClientView::addCoinScore), 0.1f); }), NULL);
	this->runAction(seq);

	playEff();

	
}
//抽奖完毕后飞元宝
void SHZCGameClientView::yuanbaoFly(Ref* ref)
{
	//刷新抽奖后的经验值
	updateLotteryResult(nullptr);

	spriteFly* spFly = spriteFly::create();
	spFly->setPos(Vec2(537, 370), Vec2(300, 695));
	spFly->startAni(2);
	m_NodeGameView0->addChild(spFly, 1000);


	getScore = SHZGetHouseIndex::getInstance()->getlotteryBao();
	disScore = getScore / 10;
	if (disScore < 1)   disScore = 1;
	auto seq = Sequence::create(DelayTime::create(1.5f), 
		                         CallFunc::create([this](){this->schedule(schedule_selector(SHZCGameClientView::addBaoScore), 0.1f); }),NULL);
	this->runAction(seq);	

	playEff();
}


//一分一分加金币的效果
void SHZCGameClientView::addCoinScore(float t)
{
	getScore -= disScore;
	if (getScore < 0)
	{
		getScore = 0;

	}
	if (getScore == 0)
	{
		m_NodeGameView0->m_CreditScore += disScore;
		this->unschedule(schedule_selector(SHZCGameClientView::addCoinScore));
		
	}
	if (getScore > 0)
	{
		m_NodeGameView0->m_CreditScore += disScore;
	}
}
//一分一分加元宝的效果
void SHZCGameClientView::addBaoScore(float t)
{
	int yuabaoNum = Player::getInstance()->getHallData().llYuanBao;
	getScore -= disScore;
	if (getScore < 0)
	{
		getScore = 0;

	}
	if (getScore == 0)
	{
		
		yuabaoNum += disScore;
		m_LbLv->setString(StringUtils::format("%d", yuabaoNum));
		this->unschedule(schedule_selector(SHZCGameClientView::addBaoScore));

	}
	if (getScore > 0)
	{
		yuabaoNum += disScore;
		m_LbLv->setString(StringUtils::format("%d", yuabaoNum));
	}
	//修改元宝的值
	CMD_GP_LogonSuccess_Hall data = Player::getInstance()->getHallData();
	data.llYuanBao = yuabaoNum;
	Player::getInstance()->setHallData(data);

}

//粒子特效
void SHZCGameClientView::playEff()
{
	_PLAY_SOUND_SHZ(GOLDWIN);
	auto particle = ParticleSystemQuad::create("images/common/Eff/baoxiangbao.plist");
	particle->setScale(1.2f);
	particle->setPosition(Vec2(537, 220));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(1.5f);									// 设置粒子系统的持续时间秒  
	this->addChild(particle, 999);
}

//连线时候的动画判断过程
bool  SHZCGameClientView::TypeImgVoid(int timeID,int typeIndex,int abindex)
{
	return false;
}

//获取滚动数据
void SHZCGameClientView::ScrollInformation(int winscore,int index,int imgtypeid,bool bquanpan)
{
	m_NodeGameView0->ScrollInformation(winscore,index,imgtypeid,bquanpan);
}


//设置小玛利信息
void SHZCGameClientView::SetMaLiInformation(int winscore,int index,int imgtypeid,int gamecount,int tyepindex,bool nowbegin)
{
	m_NodeGameView0->m_ThreeMaLiCount = gamecount;
	m_NodeGameView2->SetMaLiInformation(winscore,index,imgtypeid,gamecount,tyepindex,nowbegin);
}


//获取赢分
void SHZCGameClientView::GetWinScore(int winscore,int gametype)
{
	g_llWinScore_SHZ += winscore;
	//recordScore(winscore);
	if(gametype==0)
	{
		m_NodeGameView0->GetWinScore(winscore,gametype);

	}
	else if(gametype==1)
	{
		m_NodeGameView1->GetWinScore(winscore,gametype);
	
	}else 
	{
		m_NodeGameView2->GetWinScore(winscore,gametype);
	}
}


//设置下注
void SHZCGameClientView::SetXiaZhu(int allcellscore)
{
	m_CellScore =  allcellscore;

	m_NodeGameView0->SetXiaZhu(allcellscore);

	if(m_CellScore>0)m_NodeGameView0->m_btStart->setEnabled(true);
}

void SHZCGameClientView::SetYaLine(int line)
{

	m_NodeGameView0->SetYaLine(line);

}

//比倍结果
void SHZCGameClientView::SetBiBei(int winscore,int S1,int S2)
{
	m_NodeGameView1->SetBiBei(winscore,S1,S2);

}

//比倍数组
void SHZCGameClientView::CopyBiBeiData(int ibibeidata,int ipos)
{
	m_NodeGameView1->CopyBiBeiData(ibibeidata,ipos);
}
//上下分
void SHZCGameClientView::AddOrRemoveScore(bool addorremove)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	CMD_C_UpOrDownScore_SHZ  UporDown;
	UporDown.m_bUpOrDown = addorremove;
	//if(!m_IsLookUser)
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_UPORDOWNESCORE_SHZ,&UporDown,sizeof(CMD_C_UpOrDownScore_SHZ));

}


//得分
void SHZCGameClientView::GetWinScore(int gettype)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	CMD_C_GetScore_SHZ getscore;
	getscore.m_GetType = gettype;
	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_GETSCORE_SHZ,&getscore,sizeof(CMD_C_GetScore_SHZ));

}

//比倍
void SHZCGameClientView::GetBiBeiData()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_REQUESTBEIBEI_SHZ);

}

//加注
void SHZCGameClientView::ChangeCellScore()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_ADDSCORE_SHZ);
}
//记录玩家更新的经验
void SHZCGameClientView::setLotteryExp(int myExp)
{
	 myExp /= 10;
	 if (0 != myExp)
	{
		 m_NodeGameView0->setLotteryExp(myExp);
	}	

}




//更新自己上分
void SHZCGameClientView::SetCreditScoreScore(int myScore) 
{
	m_CreditScore=myScore;
	m_NodeGameView0->SetCreditScoreScore(myScore);

}

//创建动作(循环播放动画)
void SHZCGameClientView::ForeverAction(cocos2d::CCSprite* pSprite,int32 iWidth, int32 iHeight,int ilayer)
{
	float width  = pSprite->getContentSize().width/iWidth;  
	float height = pSprite->getContentSize().height/iHeight;  


	// 创建动画 设置动画中每张图片出现的间隔时间  
	cocos2d::CCAnimation *animation = cocos2d::CCAnimation::create();  
	animation->setDelayPerUnit(0.1f);  

	CCPoint ptSpr=pSprite->getTextureRect().origin;

	for (int k = 0; k <iHeight ; k++)
	{
		for (int i =0; i<iWidth; i++)  
			animation->addSpriteFrameWithTexture(pSprite->getTexture(),cocos2d::CCRectMake(ptSpr.x+i*width , ptSpr.y+k*height, width, height));  
	}
	
	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);  
	//添加动作，一直循环  
	pSprite->runAction(cocos2d::CCRepeatForever::create(animate));  
	//    设置循环的次数  
	//    fish->runAction(CCRepeat::create(animate, 10));  
	//   把精灵添加到层上，背景层参数为0,我就把此动画精灵与背景层设置不在同一个层上  
	pSprite->setVisible(false);
	addChild(pSprite,ilayer);  

}

//创建动作(环播循放动画)
void SHZCGameClientView::ForeverAction(cocos2d::CCSprite* pSprite,char *strName,int iCount,int iFirst,int ilayer)
{

	CCSpriteFrameCache *cache = CCSpriteFrameCache::getInstance();
	Vector<AnimationFrame*> vecSprite;

	char str[64];

	ValueMap mapTmp;
	for (int i=0;i<iCount;i++)
	{
		sprintf(str,strName,i+iFirst);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );  
		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame, 1, mapTmp);
			vecSprite.pushBack(aniFrame);
			aniFrame->release();
		}
		
	}

	cocos2d::CCAnimation *animation = CCAnimation::create(vecSprite, 0.1f);

	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);  
	//添加动作，一直循环  
	pSprite->runAction(cocos2d::CCRepeatForever::create(animate));   
	//   把精灵添加到层上，背景层参数为0,我就把此动画精灵与背景层设置不在同一个层上  
	addChild(pSprite,ilayer);  

}

//创建动作(播放动画一次)
void SHZCGameClientView::SequenceAction(const char *strImage,SEL_CallFuncND selector,int iCount,CCPoint ptImage,int iTag)
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

	cocos2d::CCAnimation *animation =CCAnimation::create(animFrames, 0.07f);

	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);  

	CCSequence* act = CCSequence::create(animate, CCCallFuncND::create(this, selector, 0),0);// 播放动画结束

	pSprite->runAction(act);

	pSprite->setPosition(ptImage);

	addChild(pSprite,8);

}

void SHZCGameClientView::onEnterTransitionDidFinish()
{
	CCScene::onEnterTransitionDidFinish();
	if (0 == DF::shared()->GetGameType())
		SHZIClientKernel::get()->SendGameOption();

}

void SHZCGameClientView::onEnter(){

	CCScene::onEnter();

	//初始化用户的分数
	SHZCGlobalUserInfo   * pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	tagSHZUserInsureInfo * pUserInsureData = pGlobalUserInfo->GetUserInsureInfo();
	
	SetCreditScoreScore(pUserInsureData->lUserScore);
	//m_LbLv->setString(toString(JZUserDefault::getInstance()->getIntegerForKey(ACCOUNT_SECTION, ACCOUNT_LEVEL, 1)));

}

//创建按钮
void SHZCGameClientView::CreatedGameBt()
{
	//返回按钮
	m_btGameBack = createButton_SHZ("sr_back.png", "sr_back.png", "sr_back.png", this, cccontrol_selector(SHZCGameClientView::onBtnClick));
	m_btGameBack->setTag(TAG_BT_BACK);
	m_btGameBack->setPosition(Vec2(1011, 683));
	addChild(m_btGameBack,100);


	//设置按钮
	m_btshezhi = createButton_SHZ("sr_settings.png", "sr_settings.png", "sr_settings.png", this, cccontrol_selector(SHZCGameClientView::onBtnClick));
	m_btshezhi->setTag(TAG_BT_SETZHI);
	m_btshezhi->setPosition(Vec2(924, 683));
	addChild(m_btshezhi,100);

	//帮助按钮
	m_btHelp = createButton_SHZ("sr_rule.png", "sr_rule.png", "sr_rule.png", this, cccontrol_selector(SHZCGameClientView::onBtnClick));
	m_btHelp->setTag(TAG_BT_HELP);
	m_btHelp->setPosition(Vec2(841, 683));
	addChild(m_btHelp,100);


	//充值按钮
	m_btRecharge = createButton_SHZ("sr_recharge.png", "sr_recharge.png", 0, this, cccontrol_selector(SHZCGameClientView::onBtnClick));
	m_btRecharge->setTag(TAG_BT_RECHARGE);
	m_btRecharge->setPosition(Vec2(748, 683));
	addChild(m_btRecharge,100);

	
}


//切换比倍界面
void SHZCGameClientView::OnChangeBibeiView()
{
	//不显示上边的按钮
	m_btGameBack->setVisible(false);
	m_btshezhi->setVisible(false);
	m_btHelp->setVisible(false);
	m_btRecharge->setVisible(false);
	
	m_LbUserScore->setVisible(true);
	m_LbLv->setVisible(true);
	/*--------------------------------*/
	m_NodeGameView0->setVisible(false);
	m_NodeGameView0->m_btPool->setVisible(false);
	//给用户增加经验值
	m_NodeGameView0->updateLotteryExp();

	m_NodeGameView1->setVisible(true);

	m_NodeGameView2->setVisible(false);

	m_NodeGameView1->StartAnim(m_NodeGameView0->m_CreditScore,m_GameState==0?m_NodeGameView0->ThisScore:m_NodeGameView2->ThisScore);

	//不显示

	m_GameState=1;
}

//切换主界面
void SHZCGameClientView::OnChangeGameView(int iCreditScore)
{
	ShowTopView(true);
	m_NodeGameView0->setVisible(true);
	m_NodeGameView0->m_btPool->setVisible(true);


	m_NodeGameView1->setVisible(false);

	m_NodeGameView2->setVisible(false);

	m_NodeGameView0->ShowView(iCreditScore);
	m_NodeGameView0->IsDisableView();  //判断是否Disable View
	
	m_GameState=0;
}

//切换玛丽界面
void SHZCGameClientView::OnChangeMlView()
{
	ShowTopView(false);
	m_NodeGameView0->setVisible(false);
	m_NodeGameView0->m_btPool->setVisible(false);
	//给用户增加经验值
	m_NodeGameView0->updateLotteryExp();

	m_NodeGameView1->setVisible(false);

	m_NodeGameView2->setVisible(true); 

	m_NodeGameView2->StartMali(m_NodeGameView0->m_CreditScore,m_NodeGameView0->m_ThisScore,m_NodeGameView0->m_CellScore,m_NodeGameView0->m_IsZidong);

	m_GameState=2;
}

  
//显示顶上的横幅
void SHZCGameClientView::ShowTopView(bool bshow)
{
	if (!bshow)
	{
		m_btGameBack->setVisible(false);
		m_btshezhi->setVisible(false);	
		m_btHelp->setVisible(false);
		m_btRecharge->setVisible(false);
		/*------修改bysl----------*/
		m_LbUserScore->setVisible(false);
		m_LbLv->setVisible(false);

	}else
	{
		m_btGameBack->setVisible(true);
		m_btshezhi->setVisible(true);	
		m_btHelp->setVisible(true);
		m_btRecharge->setVisible(true);
		/*------修改bysl----------*/  

		m_LbUserScore->setVisible(true);
		m_LbLv->setVisible(true);
	}	

}

//游戏公告
void SHZCGameClientView::SetGameNotice(const char* strMsg)
{
	CCLabelTTF *m_lbMsg=CCLabelTTF::create("34343443", "Arial", 25, CCSizeMake(1000, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	m_lbMsg->setPosition(Vec2(kRevolutionWidth_SHZ + m_lbMsg->getContentSize().width / 3,18));
	m_lbMsg->setString(strMsg);

	int vIndex = vipColorIndex(strMsg);
	m_lbMsg->setColor(cocos2d::ccc3(iVipColorTable_SHZ[vIndex][0], iVipColorTable_SHZ[vIndex][1], iVipColorTable_SHZ[vIndex][2]));
	
	CCActionInterval * a1=CCMoveTo::create(10.0f,Vec2(-100,18));
	CCSequence* act = CCSequence::create(CCDelayTime::create(m_iCount* 6.0f),a1, CCCallFuncND::create(this, callfuncND_selector(SHZCGameClientView::RollText), 0),0);


	PLAZZ_PRINTF("%s",strMsg);

	m_iCount++;

	m_GameNotcieTexture->setVisible(true);

	m_lbMsg->runAction(act);

	m_TailorView->addChild(m_lbMsg,201);
}

void SHZCGameClientView::RollText(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	m_iCount--;

	if (m_iCount<=0)
	{
		m_iCount=0;
		m_GameNotcieTexture->setVisible(false);
	}

	deadSpt->removeFromParentAndCleanup(true);
}

void SHZCGameClientView::DisableGamePopUp()
{
	m_NodeGameView0->DiableGamePopup();
}

int SHZCGameClientView::vipColorIndex(const char* strMsg){

	const char* pVipLoc = strstr(strMsg, "VIP");
	 
	int index = (NULL == pVipLoc ? 0 : *(pVipLoc + 3) - 48);

	log("CGameClientView::vipColorIndex: %d", index);
	return index;
}

void SHZCGameClientView::recordScore(long addscore){


	/*分数小于等于零，直接返回*/
	if (addscore <= 0) return;

	SHZCGlobalUserInfo   * pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pUserData = pGlobalUserInfo->GetGlobalUserData();

	unsigned long gameId = pUserData->dwGameID;

	char str[32];

	sprintf(str, "record_%lu", gameId);

	std::string strScore = JZUserDefault::getInstance()->getStringForKey("ACCOUNT", str, "");

	long long score = 0;

	if (strScore.empty())
	{
		score = addscore;

	}
	else
	{
		score = atoll(strScore.c_str()) + addscore;
	}

	char szScore[70];

	sprintf(szScore, "%lld", score);
	PLAZZ_PRINTF("win Score : %s", szScore);

	JZUserDefault::getInstance()->setStringForKey("ACCOUNT", str, szScore);
	PLAZZ_PRINTF("win Score : %lld", score);
}



void SHZGameTest::comeToHall()
{

	if (NULL != m_pGameView0Ref->m_goldDownLayer) {
		if (!m_pGameView0Ref->m_goldDownLayer->isTouched())
			return;
	}

	if (NULL != m_pGameViewMaliRef->m_goldDownLayer) {
		if (!m_pGameViewMaliRef->m_goldDownLayer->isTouched())
			return;
	}

	SHZGetHouseIndex::getInstance()->setRechargeFlag(true);
	SHZSoundManager::shared()->stopMusic();
	if (SHZIClientKernel::get())
		SHZIClientKernel::get()->Intermit(SHZ_GameExitCode_Normal);
}