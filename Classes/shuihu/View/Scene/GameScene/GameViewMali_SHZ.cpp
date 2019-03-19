#include "GameViewMali_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
USING_NS_CC;
USING_NS_CC_EXT;

//按钮标识
#define TAG_BT_ML_SCORE		1			//得分
#define TAG_BT_ML_BIBEI		2			//比倍

#define ZHUANQUAN			100


const int m_ThreeTypeid_SHZ[24]={0,5,7,1,8,6,0,3,5,7,8,2,0,4,6,8,5,3,0,4,7,8,6,2};

const int m_ThreeTypeScore[9]={0,200,100,70,50,20,10,5,2};


SHZCGameViewMali::SHZCGameViewMali(void)
{
	m_ThreeMaLiCount=0;

	m_ServerPai=0;

	memset(m_ThreeInt,0,sizeof(m_ThreeInt));

	memset(m_typeimgstructthree,0,sizeof(m_typeimgstructthree));

	memset(m_typeid,0,sizeof(m_typeid));

	m_izhuanQuanID=0;			

	m_CurrThreePai=0;

	m_CurrQuanCount=0;

	m_ThreeShan=0;

	m_ThisScore=0;

	ThisScore=0;

	m_CellScore=0;

	m_CreditScore=0;

	m_bzidong=false;

    m_goldDownLayer = NULL;
}

SHZCGameViewMali::~SHZCGameViewMali(void)
{
}
SHZCGameViewMali* SHZCGameViewMali::create()
{
	SHZCGameViewMali* layer = new SHZCGameViewMali();
	if (layer && layer->init())
	{
		layer->autorelease();

		return layer;
	}

	delete layer;

	return 0;
}
bool SHZCGameViewMali::init()
{
	float bgX = kRevolutionWidth_SHZ/2;
	float bgY = kRevolutionHeight_SHZ/2;

	for (int i=0;i<7;i++)
	{
		m_ptTypeImage[i].setPoint(81+152*i,645+23);
	}

	for (int i=0;i<5;i++)
	{
		m_ptTypeImage[i+7].setPoint(995,555-i*92+23);
	}

	for (int i=0;i<7;i++)
	{
		m_ptTypeImage[i+12].setPoint(995-i*152,90+23);
	}
	for (int i=0;i<5;i++)
	{
		m_ptTypeImage[i+19].setPoint(81,180+i*92+23);
	}

	do 
	{
		cocos2d::CCSprite* ImageGunDong=cocos2d::CCSprite::createWithSpriteFrameName("GUNDONG.png");
		m_GunDongImgeTexture = cocos2d::CCSpriteBatchNode::createWithTexture(ImageGunDong->getTexture());
		addChild(m_GunDongImgeTexture);

		m_BgImgeTexture = cocos2d::CCSprite::createWithSpriteFrameName("BG_2.png");
		m_BgImgeTexture->setPosition(Vec2(bgX,bgY));
		addChild(m_BgImgeTexture,10);

		m_TypeImageTexture = ::CCSprite::createWithSpriteFrameName("ml_type0.png");
		m_TypeImageTexture->setPosition(m_ptTypeImage[0]);
		addChild(m_TypeImageTexture,100);

		m_TypeNode=CCNode::create();
		addChild(m_TypeNode);


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


		//得分按钮
		m_btGetScore = createButton_SHZ("bt_mlscore0.png", "bt_mlscore1.png", "bt_mlscore2.png", this, cccontrol_selector(SHZCGameViewMali::onBtnClick));
		m_btGetScore->setTag(TAG_BT_ML_SCORE);
		m_btGetScore->setEnabled(false);
		//m_btGetScore->setEnabled(false);
		m_btGetScore->setPosition(Vec2(680+80,30));
		addChild(m_btGetScore,100);


		//比倍按钮
		m_btbibei = createButton_SHZ("bt_mlbibei0.png", "bt_mlbibei1.png", "bt_mlbibei2.png", this, cccontrol_selector(SHZCGameViewMali::onBtnClick));
		m_btbibei->setTag(TAG_BT_ML_BIBEI);
		m_btbibei->setEnabled(false);
		//m_btbibei->setEnabled(false)
		m_btbibei->setPosition(Vec2(680+80*3,30));
		addChild(m_btbibei,100);




		//字体
		m_lbScore1= CCLabelTTF::create("34343443", "Arial", 18, CCSizeMake(120, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		m_lbScore1->setPosition(Vec2(580,400+25));
		m_lbScore1->setString("0");
		addChild(m_lbScore1,100);

		m_lbScore2= CCLabelTTF::create("34343443", "Arial", 18, CCSizeMake(120, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		m_lbScore2->setPosition(Vec2(360,168+25));
		m_lbScore2->setColor(ccc3(255,255,0));
		m_lbScore2->setString("0");
		addChild(m_lbScore2,100);

		m_lbScore3=CCLabelTTF::create("34343443", "Arial", 18, CCSizeMake(120, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		m_lbScore3->setPosition(Vec2(605,168+25));
		m_lbScore3->setString("9");
		addChild(m_lbScore3,100);

		m_lbScore4=CCLabelTTF::create("34343443", "Arial", 18, CCSizeMake(120, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		m_lbScore4->setPosition(Vec2(830,168+25));
		m_lbScore4->setString("0");
		addChild(m_lbScore4,100);


		m_lbwinScore=CCLabelBMFont::create("0123456789","bmfonts/winScore.fnt");
		m_lbwinScore->setPosition(Vec2(kRevolutionWidth_SHZ / 2/*620*/, 295));
		m_lbwinScore->setString("0");
		m_lbwinScore->setVisible(false);
		addChild(m_lbwinScore,100);



		SetScrollState(true);

	} while (0);

	return true;
}

void SHZCGameViewMali::onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	Control* ctr = (Control*)obj;

	//SoundManager::shared()->playSound("btsounds");

	//控件判断
	switch (ctr->getTag())
	{
	case TAG_BT_ML_SCORE:
		{
			m_btGetScore->setEnabled(false);
			m_btbibei->setEnabled(false);

			if (NULL != m_goldDownLayer)
			{	
				// 手动减少引用计数器
				m_goldDownLayer->release();
				m_goldDownLayer = NULL;
			}
			GetWinScore(2);

			break;
		}
	case TAG_BT_ML_BIBEI:
		{
			m_btGetScore->setEnabled(false);
			m_btbibei->setEnabled(false);

			m_GameSceneInterface->OnChangeBibeiView();

			break;
		}

	}
}
   
//得分
void SHZCGameViewMali::GetWinScore(int gettype)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	CMD_C_GetScore_SHZ getscore;
	getscore.m_GetType = gettype;
	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_GETSCORE_SHZ,&getscore,sizeof(CMD_C_GetScore_SHZ));

}
//获取赢分
void SHZCGameViewMali::GetWinScore(int winscore,int gametype)
{
		ThisScore = winscore;

		char strSocre[32];
		sprintf(strSocre,"%d",ThisScore);
		m_lbScore2->setString(strSocre);

		m_btGetScore->setEnabled(false);
		m_btbibei->setEnabled(false);
		m_TempCCscore = m_CreditScore;
		m_TempDwonscore = ThisScore / 10;
		if(m_TempDwonscore<1)m_TempDwonscore = 1;
		m_TempTTscore = ThisScore;
		//CBassMusicManager::GetInstance()->Play(19,false);

		char sSound[32]={0};
		sprintf(sSound, "defen");
		SHZSoundManager::shared()->playSound(sSound);

		schedule(schedule_selector(SHZCGameViewMali::OnDwonScoreTimer),0.1f);
	
}
//设置小玛利信息
void SHZCGameViewMali::SetMaLiInformation(int winscore,int index,int imgtypeid,int gamecount,int tyepindex,bool nowbegin)
{

	char strNum[32];

	sprintf(strNum,"%d",gamecount);

	m_lbScore1->setString(strNum);

	m_bRoll=true;
	m_bNext=false;
	m_CurrQuanCount    = 0;
	m_MaLiSquenceCount = 0;
	m_ThreeMaLiCount = gamecount;
	m_ServerPai = tyepindex;
	m_ThreeInt[index] = imgtypeid;

	//如果是马上开始就开始吧
	if(nowbegin&&index==3&&tyepindex!=1000)
	{
		for(int i=0;i<4;i++)
			m_typeid[i]=m_ThreeInt[i];

		m_izhuanQuanID=ZHUANQUAN+4;
		schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),2.0f);

		PLAZZ_PRINTF("%s",a_u8("开始小玛丽"));

		//SetTimer(ZHUANQUAN+4,2000,NULL);
	}

	//结束小玛利游戏
	if(nowbegin&&index==3&&tyepindex==1000)
	{
		m_btGetScore->setEnabled(true);
		m_btbibei->setEnabled(true);
		
		char strScore[32];

		sprintf(strScore,"%d",ThisScore);

		m_lbwinScore->setString(strScore);
		m_lbwinScore->setVisible(true);
		m_winscoreViewTexture->setVisible(true);
		schedule(schedule_selector(SHZCGameViewMali::OnbibeiordefenTimer),0.5f);

       ShowGoldDownEffect();
		if (m_bzidong)
		{

			m_btGetScore->setEnabled(false);
			m_btbibei->setEnabled(false);
			//GetWinScore(2);
		    
            unschedule(schedule_selector(SHZCGameViewMali::OnbibeiordefenTimer));
            schedule(schedule_selector(SHZCGameViewMali::TouchStateCheck), 0.5f);
		}

		char sSound[32]={0};
		sprintf(sSound, "winsound");
		SHZSoundManager::shared()->playSound(sSound);

	}
}
//开始小玛丽
void SHZCGameViewMali::StartMali(int icreditscore,int ibibeiScore,int icellscore,bool bzidong)
{
	m_bzidong=bzidong;
	unschedule(schedule_selector(SHZCGameViewMali::OnbibeiordefenTimer));
	m_lbwinScore->setVisible(false);
	m_defenViewTexture->setVisible(false);
	m_bibeiViewTexture->setVisible(false);
	m_winscoreViewTexture->setVisible(false);

	ThisScore=ibibeiScore;

	m_CellScore=icellscore;

	m_CreditScore=icreditscore;

	char strScore[32];

	sprintf(strScore,"%d",icreditscore);

	m_lbScore2->setString(strScore);

	sprintf(strScore,"%d",ibibeiScore);

	m_lbScore3->setString(strScore);

	sprintf(strScore,"%d",icellscore*9);

	m_lbScore4->setString(strScore);

	for(int i=0;i<4;i++)
	{
		m_typeid[i]=m_ThreeInt[i];
		m_typeimgstructthree[i].m_typeid = m_typeid[i];
		m_typeimgstructthree[i].m_have = false;
	}
	m_bNext=false;
	m_izhuanQuanID=ZHUANQUAN+2;
	unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));
	schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),0.08f);
	m_CurrThreePai=0;
	m_CurrQuanCount=0;
	char sSound[32]={0};
	sprintf(sSound, "malistart1");
	SHZSoundManager::shared()->playSound(sSound);
}

void SHZCGameViewMali::OnZhuanQuanTimer(float dt)
{
	if(ZHUANQUAN+2 ==m_izhuanQuanID )
	{
		if (m_bRoll)
		{
			m_bRoll=false;
			m_TypeNode->removeAllChildren();
			
		}

		m_CurrThreePai++;

		if(m_CurrThreePai>5)
		{
			m_typeimgstructthree[0].m_have=true;
		}
		if(m_CurrThreePai>10)
		{
			m_typeimgstructthree[1].m_have=true;
		}
		if(m_CurrThreePai>15)
		{
			m_typeimgstructthree[2].m_have=true;
		}
		if(m_CurrThreePai>20)
		{
			char sSound[32]={0};
			
			m_typeimgstructthree[3].m_have=true;
		
		}
		//CBassMusicManager::GetInstance()->Play(21,false);

		/*	char sSound[32]={0};
		sprintf(sSound, "gudong");
		SoundManager::shared()->playSound(sSound);*/

		SHZSoundManager::shared()->playSound("ding");
		
		if(m_CurrThreePai>=24){
			m_CurrThreePai=0;
			m_CurrQuanCount++;  
			m_bNext=false;
		}

		m_TypeImageTexture->setPosition(m_ptTypeImage[m_CurrThreePai]);

		char str[32]="";

		sprintf(str,"ml_type%d.png",m_ThreeTypeid_SHZ[m_CurrThreePai]);

		m_TypeImageTexture->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));

		if(m_CurrQuanCount==0)
		{
			if (!m_bNext)
			{
				unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));
				schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),0.02f);
				m_bNext=true;
				SetTypeImg(0);
				
			}


		}
		if(m_CurrQuanCount==1)
		{
			if (!m_bNext)
			{
				unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));
				schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),0.04f);
				m_bNext=true;
				SetTypeImg(1);
			}

		
		}
		if(m_CurrQuanCount==2)
		{
			if (!m_bNext)
			{
				unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));
				schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),0.08f);
				m_bNext=true;
				SetTypeImg(2);

				
			}
		}
		if(m_CurrQuanCount==3)
		{
			if (!m_bNext)
			{
				m_bNext=true;
				SetTypeImg(3);
			}

			if(m_ServerPai==m_CurrThreePai)
			{
				/*KillTimer(nIDEvent);*/
				unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));
				bool ishaveyou = false;
				SetTypeImg(0);
				//检查４个中是否有和转的一样
				for(int i=0;i<4;i++)
				{
					if(m_typeimgstructthree[i].m_typeid == m_ThreeTypeid_SHZ[m_ServerPai])
					{
						m_typeimgstructthree[i].m_CanAction = true;
						m_typeimgstructthree[i].m_have=true;
						m_typeimgstructthree[i].m_havequan = true;
						ishaveyou = true;
						ThisScore = ThisScore +  m_CellScore*9*m_ThreeTypeScore[m_typeimgstructthree[i].m_typeid];
					}

				}
				//检查是否有4个一样的
				if(m_typeimgstructthree[0].m_typeid==m_typeimgstructthree[1].m_typeid&&m_typeimgstructthree[1].m_typeid==m_typeimgstructthree[2].m_typeid&&m_typeimgstructthree[2].m_typeid==m_typeimgstructthree[3].m_typeid)
				{
					for(int i=0;i<4;i++)
					{
						m_typeimgstructthree[i].m_CanAction = true;
						m_typeimgstructthree[i].m_have=true;
						m_typeimgstructthree[i].m_havequan = true;
					}
					ThisScore = ThisScore +  m_CellScore*9*500;
					ishaveyou = true;
				}
				if(m_typeimgstructthree[0].m_typeid==m_typeimgstructthree[1].m_typeid&&m_typeimgstructthree[1].m_typeid==m_typeimgstructthree[2].m_typeid&&m_typeimgstructthree[2].m_typeid!=m_typeimgstructthree[3].m_typeid)
				{
					for(int i=0;i<3;i++)
					{
						m_typeimgstructthree[i].m_CanAction = true;
						m_typeimgstructthree[i].m_have=true;
						m_typeimgstructthree[i].m_havequan = true;
					}
					ThisScore = ThisScore +  m_CellScore*9*20;
					ishaveyou = true;
				}
				if(m_typeimgstructthree[0].m_typeid!=m_typeimgstructthree[1].m_typeid&&m_typeimgstructthree[1].m_typeid==m_typeimgstructthree[2].m_typeid&&m_typeimgstructthree[2].m_typeid==m_typeimgstructthree[3].m_typeid)
				{
					for(int i=1;i<4;i++)
					{
						m_typeimgstructthree[i].m_CanAction = true;
						m_typeimgstructthree[i].m_have=true;
						m_typeimgstructthree[i].m_havequan = true;
					}
					ThisScore = ThisScore +  m_CellScore*9*20;
					ishaveyou = true;
				}
				char strScore[32];

				sprintf(strScore,"%d",ThisScore);

				m_lbScore3->setString(strScore);

				if(ishaveyou==false)
				{
					MaLiSendMessage();
				}
				else
				{
					char sSound[32]={0};
					sprintf(sSound, "winsound");
					SHZSoundManager::shared()->playSound(sSound);

					//CBassMusicManager::GetInstance()->Play(115,false);
					m_ThreeShan = true;
					//KillTimer(ZHUANQUAN+3);
					//SetTimer(ZHUANQUAN+3,50,NULL);
					m_izhuanQuanID=ZHUANQUAN+3;
					unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));
					schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),0.05f);

				}

			}
		}

	}
	else if(ZHUANQUAN+4 ==m_izhuanQuanID )
	{
		//KillTimer(nIDEvent);
		unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));

		for(int i=0;i<4;i++)
		{

			m_typeimgstructthree[i].m_index = 0;
			m_typeimgstructthree[i].m_type = 0;
			m_typeimgstructthree[i].m_CanAction=false;
			m_typeimgstructthree[i].m_Cons= 0;
			m_typeimgstructthree[i].m_Rows= 0;
			m_typeimgstructthree[i].m_typeid = m_typeid[i];
			m_typeimgstructthree[i].m_havequan =  false;
			m_typeimgstructthree[i].m_have = false;
		}

		m_CurrQuanCount =0;
		//SetTimer(ZHUANQUAN+2,50,NULL);
		PLAZZ_PRINTF("%s",a_u8("滚动"));
		m_izhuanQuanID=ZHUANQUAN+2;
		schedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer),0.05f);
	}
	else if(ZHUANQUAN+3 ==m_izhuanQuanID )
	{
		unschedule(schedule_selector(SHZCGameViewMali::OnZhuanQuanTimer));

		char strName[32]="";

		sprintf(strName,"ml_type%d.png",m_ThreeTypeid_SHZ[m_CurrThreePai]);

		CCSprite* pSprite=CCSprite::createWithSpriteFrameName(strName);

		pSprite->setPosition(m_ptTypeImage[m_CurrThreePai]);

		CCActionInterval* a1=CCScaleTo::create(0.1f,1.1f);

		CCActionInterval* a2=CCScaleTo::create(0.1f,1.0f);

		CCSequence* a3=CCSequence::create(CCRepeat::create(CCSequence::create(a1,a2,0),10),CCCallFuncND::create(this, callfuncND_selector(SHZCGameViewMali::TypescalingAnim), 0),0);


		pSprite->runAction(a3);

		addChild(pSprite,100);

		for(int i=0;i<4;i++)
		{
			if (!m_typeimgstructthree[i].m_CanAction)
			{
				continue;
			}

			char strName[64];
			sprintf(strName,"type%d",m_typeimgstructthree[i].m_typeid);
			strcat(strName,"_1_ (%d).png");

			static int typeAnimCount[]={53,41,47,36,35,54,30,44,42};

			m_MaLiSquenceCount++; //计数调用的次数
			SequenceAction(strName,callfuncND_selector(SHZCGameViewMali::ShowTypeAnim),typeAnimCount[m_typeimgstructthree[i].m_typeid],Vec2(270+i*185,270+23));
		}

	}
}
//类型缩放
void SHZCGameViewMali::TypescalingAnim(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	deadSpt->removeFromParentAndCleanup(true);
}

//类型动画播放
void SHZCGameViewMali::ShowTypeAnim(cocos2d::CCNode* node, void* data)
{
	CCSprite* deadSpt = (CCSprite*)node;

	node->stopAllActions();

	deadSpt->removeFromParentAndCleanup(true);

	//将四张牌重新赋值
	for(int j=0;j<4;j++)
	{
		m_typeimgstructthree[j].m_index = 0;
		m_typeimgstructthree[j].m_type = 0;
		m_typeimgstructthree[j].m_CanAction=false;
		m_typeimgstructthree[j].m_Cons= 0;
		m_typeimgstructthree[j].m_Rows= 0;
		m_typeimgstructthree[j].m_havequan =  false;
		m_typeimgstructthree[j].m_have = true;
	}
	m_ThreeShan = false;

	if ((--m_MaLiSquenceCount) == 0)
	     MaLiSendMessage();
}

//发送玛丽消息
void SHZCGameViewMali::MaLiSendMessage()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();

	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_MALISTART_SHZ);   //开始马力游戏

}
//设置滚动
void SHZCGameViewMali::SetScrollState(bool bScroll)
{
	if (bScroll)
	{
		for (int i=0;i<4;i++)
		{
			CCSprite* sprite = cocos2d::CCSprite::createWithSpriteFrameName("GUNDONG.png");
			sprite->setScaleX(0.9f);
			sprite->setPosition(Vec2(160+i*183,890+23));
			CCActionInterval * a1=CCMoveTo::create(0.8f,Vec2(265+i*183,-120+23));


			CCActionInterval*  action1 = CCRepeatForever::create(
				//动作组:先在位置显示，然后执行动作a1
				CCSequence::create( CCPlace::create(Vec2(265+i*183,890+23)), a1, NULL)); 

			sprite->runAction(action1);

			m_GunDongImgeTexture->addChild(sprite);
		}
	}else
	{
		//清除
		m_GunDongImgeTexture->removeAllChildren();
	}


}
//设置4个类型图
void SHZCGameViewMali::SetTypeImg(int iPos)
{
	//if (m_typeimgstruct[iPos].m_have)
	{
		char strName[64]="";

		sprintf(strName,"type%d_0_ (1).png",m_typeimgstructthree[iPos].m_typeid);

		CCSprite* pSprite=CCSprite::createWithSpriteFrameName(strName);
		pSprite->setScale(0.9f);
		pSprite->setPosition(Vec2(270+iPos*182,265+23));

		m_TypeNode->addChild(pSprite);
	}
}

//获取赢分
void SHZCGameViewMali::OnDwonScoreTimer(float dt)
{

	char strSocre[32];
	//游戏下分DWONSCORE_SHZ
	ThisScore = ThisScore - m_TempDwonscore;
	m_CreditScore = m_CreditScore + m_TempDwonscore;
	sprintf(strSocre,"%d",m_CreditScore);
	m_lbScore2->setString(strSocre);
	sprintf(strSocre,"%d",ThisScore);
	m_lbScore3->setString(strSocre);

	m_lbwinScore->setString(strSocre);

	if(ThisScore<0)
	{
		ThisScore = 0;

		sprintf(strSocre,"%d",ThisScore);
		m_lbScore3->setString(strSocre);
		m_lbwinScore->setString(strSocre);

		m_CreditScore=m_TempCCscore+m_TempTTscore;
		sprintf(strSocre,"%d",m_CreditScore);
		m_lbScore2->setString(strSocre);

		
		
	}
	if(ThisScore==0)
	{
		unschedule(schedule_selector(SHZCGameViewMali::OnDwonScoreTimer));

		m_GameSceneInterface->OnChangeGameView(m_CreditScore);
	}
}

//创建动作(播放动画一次)
void SHZCGameViewMali::SequenceAction(const char *strImage,SEL_CallFuncND selector,int iCount,CCPoint ptImage)
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

//比倍得分
void SHZCGameViewMali::OnbibeiordefenTimer(float dt)
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

void SHZCGameViewMali::ShowGoldDownEffect() {
    if (NULL == m_goldDownLayer)
    {
        m_goldDownLayer = SHZGoldDownLayer::create();
        addChild(m_goldDownLayer, 10000);
    }
}

void SHZCGameViewMali::TouchStateCheck(float dt)
{
    if (m_goldDownLayer->isTouched()) {
		// 手动减少引用计数器
		m_goldDownLayer->release();
        m_goldDownLayer = NULL;
        unschedule(schedule_selector(SHZCGameViewMali::TouchStateCheck));
        schedule(schedule_selector(SHZCGameViewMali::OnbibeiordefenTimer), 0.5f);
        GetWinScore(2);
    }
}