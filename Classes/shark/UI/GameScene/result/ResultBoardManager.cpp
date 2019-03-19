#include "ResultBoardManager.h"
#include "shark/GameInfo.h"

#define   RESULTBOARD   "result_board"
#define   COMMONSHOW    "commonShow"
#define   WINSHOW       "result_win"
#define   LOSTSHOW      "result_lost"
#define   STAKEBUTTON   "stakeBtn"


#define GOLD_FLY_PLIST	"images/common/Eff/gold2.plist"
#define GOLD_FLY_PNG	"images/common/Eff/gold2.png"

static int special[8] = { 2, 5, 9, 13, 16, 19, 23, 27 };
ResultBoardManager::ResultBoardManager()
:pMainNode(nullptr)
, commandShow(nullptr)
, winNode(nullptr)
, stakeButton(nullptr)
, scoreLabel(nullptr)
, ticketLabel(nullptr)
, yuanBaoLabel(nullptr)
, winLogo(nullptr)
, bigWinLogo(nullptr)
, ticketNum(0)
, yuanbaoNum(0)
, scoreNum(0)
{
	//加载金币的图片
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(GOLD_FLY_PLIST);
}
ResultBoardManager::~ResultBoardManager()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(GOLD_FLY_PLIST);
	Director::getInstance()->getTextureCache()->removeTextureForKey(GOLD_FLY_PNG);
}

void ResultBoardManager::setContent()
{
	pMainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, RESULTBOARD));
	CC_ASSERT(pMainNode != nullptr);
	pMainNode->setVisible(false);

	stakeButton = dynamic_cast<Node*>(_seekNodeByName(m_pNode, STAKEBUTTON));
	CC_ASSERT(stakeButton != nullptr);

	commandShow = dynamic_cast<Node*>(_seekNodeByName(m_pNode, COMMONSHOW));
	CC_ASSERT(commandShow != nullptr);
	commandShow->setVisible(false);

	winNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, WINSHOW));
	CC_ASSERT(winNode != nullptr);


	scoreLabel = dynamic_cast<TextAtlas*>(_seekNodeByName(commandShow, "winNumber"));
	CC_ASSERT(scoreLabel != nullptr);
    
	ticketLabel = dynamic_cast<TextAtlas*>(_seekNodeByName(commandShow, "ticketNum"));
	CC_ASSERT(ticketLabel != nullptr);

	yuanBaoLabel = dynamic_cast<TextAtlas*>(_seekNodeByName(commandShow, "yuabaoNum"));
	CC_ASSERT(yuanBaoLabel != nullptr);

	winLogo = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "win_logo"));
	CC_ASSERT(winLogo != nullptr);
	winLogo->setVisible(false);

	bigWinLogo = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "bigWin_logo"));
	CC_ASSERT(bigWinLogo != nullptr);
	bigWinLogo->setVisible(false);
	//押分按钮的显示
	char str[20];
	for (int i = 0; i < 12; i++)
	{
		sprintf(str, "btn%d", i + 1);
		Button* btn = dynamic_cast<Button*>(_seekNodeByName(stakeButton, str));
		btn->setVisible(false);
		m_stakeArray[i].dependNode(btn);

		btn->addTouchEventListener(&m_stakeArray[i], toucheventselector(OneOfResult::BtEvent));
	}
}

//设置倍率
void ResultBoardManager::SetRate(int index)
{
	int *rate = GameInfo::GetInstance()->getRate();
	for (int i = 0; i < 12; i++)
	{
		if (index == i)
		m_stakeArray[i].SetBeiLv(rate[i]);
	}	
}

//设置显示本局游戏中的按钮
void ResultBoardManager::showWinBtn(int index,bool flag,int score)
{
	for (int i = 0; i < 12; i++)
	{
		if (i == index)
		{
			m_stakeArray[i].showWinBtn(flag, score);
		}
		
	}
}


//开启闪烁
void ResultBoardManager::startPlay()
{
	pMainNode->setVisible(true);
	commandShow->setVisible(true);	
	summaryNum();
}
//关闭闪烁
void ResultBoardManager::stopPlay()
{
	for (int i = 0; i < 12; i++)
	{
		m_stakeArray[i].stopAll();
	}
	//设置赢分显示还原
	setRestore();
	//清空本局数据
	scoreNum = 0;
	ticketNum = 0;
	yuanbaoNum = 0;
	pMainNode->setVisible(false);
	commandShow->setVisible(false);
}

//设置赢分显示
void ResultBoardManager::setWin()
{
	winLogo->setVisible(true);
}

//设置超级大赢家显示
void ResultBoardManager::setBigWin()
{
	bigWinLogo->setVisible(true);
}

//设置显示还原
void ResultBoardManager::setRestore()
{
	winLogo->setVisible(false);
	bigWinLogo->setVisible(false);
}

//设置赢取分数，元宝数，船票数
void ResultBoardManager::setWinScore(int score, int yuanbao, int ticket)
{
	log("-------scoreNum = %d---------", score);
	scoreLabel->setString(StringUtils::format("%d", score));
	ticketLabel->setString(StringUtils::format("%d", ticket));
	yuanBaoLabel->setString(StringUtils::format("%d", yuanbao));
}


//计算本局游戏的得分
void ResultBoardManager::summaryNum()
{
	auto gameInfo = GameInfo::GetInstance();

	int firstIndex = gameInfo->getopenRelt().resultIndex[0];
	int secondIndex = gameInfo->getopenRelt().resultIndex[1];
	int thirdIndex = gameInfo->getopenRelt().resultIndex[2];
	if (firstIndex>-1 && firstIndex<28)
	{
		//金鲨
		if (firstIndex == 2 || firstIndex == 9 || firstIndex == 16||firstIndex == 23)
		{
			sharkSummary(1);			
			if (firstIndex == 23)
			{
				colorSummary(secondIndex);
				colorSummary(thirdIndex);
			}		
			else
			{
				generalSummary(secondIndex);
			}
		}
		//银鲨
		else if (firstIndex == 5||firstIndex == 13 || firstIndex == 19 || firstIndex == 27)
		{
			sharkSummary(2);			
			if (firstIndex == 5)
			{
				colorSummary(secondIndex);
				colorSummary(thirdIndex);

			}	
			else
			{
				generalSummary(secondIndex);
			}
		}
		else
		{
			generalSummary(firstIndex);
		}
		summaryScore();
	}

	
}

//中普通动物
void ResultBoardManager::generalSummary(int index)
{
	int *rate = GameInfo::GetInstance()->getRate();
	int animalRate = 0;
	int typeRate = 0;
	int btnIndex = 0;
	int typeIndex = 0;
	if (index == 0 || index == 1)
	{
		//熊猫
		animalRate = rate[9];
		typeRate = rate[3];
		btnIndex = 9;
		typeIndex = 3;

	}
	else if (index == 3 || index == 4)
	{
		//猴子
		animalRate = rate[10];
		typeRate = rate[3];
		btnIndex = 10;
		typeIndex = 3;
	}

	else if (index == 6 || index == 7 || index == 8)
	{
		//兔子
		animalRate = rate[11];
		typeRate = rate[3];
		btnIndex = 11;
		typeIndex = 3;
	}
	else if (index == 10 || index == 11 || index == 12)
	{
		//燕子
		animalRate = rate[4];
		typeRate = rate[0];
		btnIndex = 4;
		typeIndex = 0;
	}
	else if (index == 14 || index == 15)
	{
		//鸽子
		animalRate = rate[5];
		typeRate = rate[0];
		btnIndex = 5;
		typeIndex = 0;
	}
	else if (index == 17 || index == 18)
	{
		//孔雀
		animalRate = rate[6];
		typeRate = rate[0];
		btnIndex = 6;
		typeIndex = 0;
	}
	else if (index == 20 || index == 21 || index == 22)
	{
		//老鹰
		animalRate = rate[7];
		typeRate = rate[0];
		btnIndex = 7;
		typeIndex = 0;
	}
	else if (index == 24 || index == 25 || index == 26)
	{
		//狮子
		animalRate = rate[8];
		typeRate = rate[3];
		btnIndex = 8;
		typeIndex = 3;
	}


	int animalScore = GameInfo::GetInstance()->getownInfo().ownStake[btnIndex];
	int typeScore = GameInfo::GetInstance()->getownInfo().ownStake[typeIndex];
	if (animalScore != 0)
	{
		scoreNum += animalScore*animalRate;
		showWinBtn(btnIndex,true, animalScore*animalRate);
	}
	else
	{
		showWinBtn(btnIndex,false, animalScore);
	}

	if (typeScore != 0)
	{
		scoreNum += typeScore*typeRate;
		showWinBtn(typeIndex,true, typeScore*typeRate);
	}
	else
	{
		showWinBtn(typeIndex,false, typeScore);
	}
	//显示中的动物的倍率
	SetRate(btnIndex);
	//显示中的类型的倍率
	SetRate(typeIndex);
	

}
//中鲨鱼
void ResultBoardManager::sharkSummary(int index)
{
	int *rate = GameInfo::GetInstance()->getRate();
	int stakeRate = rate[index];
	int score = GameInfo::GetInstance()->getownInfo().ownStake[index];
	scoreNum += score*stakeRate;

	SetRate(index);
	if (0 == score)
	{
		showWinBtn(index ,false, score);
	}
	else
	{
		showWinBtn(index,true, score*stakeRate);
	}

}

//中彩金鲨
void ResultBoardManager::colorSummary(int index)
{
	int curIndex = 0;
	for (int i = 0; i < 8; i++)
	{
		if (index == special[i])
		{
			curIndex = index;
			break;		
			
		}
	}

	if (curIndex != 0)
	{
		colorShark(index);
	}
	else
	{
		generalSummary(index);
	}
	
	
}

//彩金鲨具体实现
void ResultBoardManager::colorShark(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	int *rate = GameInfo::GetInstance()->getRate();
	if (index == 9 || index == 23)
	{
		//此处是彩金界面的逻辑
	}
	else
	{
		//1元宝，2船票
		if (gameInfo->getopenRelt().aType == 1)
		{
			//所有的都是元宝		
			int scoreStake = gameInfo->getownInfo().ownTotal;
			if (scoreStake == 0) return;
			int achiveNum = scoreStake / 100;
			log("---------%d---------", achiveNum);
			if (achiveNum == 0)
			{
				yuanbaoNum += 1;
			}
			else
			{
				yuanbaoNum += achiveNum;
			}
		}
		else if (gameInfo->getopenRelt().aType == 2)
		{
			//除了2或16其余都是元宝
			if (index == 2 || index == 16)
			{
				int scoreStake = gameInfo->getownInfo().ownTotal;
				if (scoreStake == 0) return;
				int achiveNum = scoreStake / 1000;
				log("---------%d---------", achiveNum);
				if (achiveNum == 0)
				{
					ticketNum += 1;
				}
				else
				{
					ticketNum += achiveNum;
				}
			}
			else
			{
				int scoreStake = gameInfo->getownInfo().ownTotal;
				if (scoreStake == 0)return;
				int achiveNum = scoreStake / 100;
				log("---------%d---------", achiveNum);
				if (achiveNum == 0)
				{
					yuanbaoNum += 1;
				}
				else
				{
					yuanbaoNum += achiveNum;
				}
			}

		}
	}
	
}

//结算游戏得分
void ResultBoardManager::summaryScore()
{
	auto gameInfo = GameInfo::GetInstance();
	bool flag = GameInfo::GetInstance()->gettophead();
	int stakeNum = gameInfo->getownInfo().ownTotal;

	int score = scoreNum;
	setWinScore(score, yuanbaoNum, ticketNum);
	if (flag && (scoreNum > 3 * stakeNum))
	{
		setBigWin();
	}
	else
	{
		setWin();
	}

	_STOP_MUSCI_SHARK;
	_PLAY_SOUND_SHARK(WINER_MUSIC);
	//播放金币喷泉
	if (scoreNum > _ZERO)
	{
		//延迟1.5秒播动画
		scheduleOnce(schedule_selector(ResultBoardManager::goldandnumroll), 2.f);
		scheduleOnce(schedule_selector(ResultBoardManager::playEff), 3.0f);
	}
	
}

//显示特效
void ResultBoardManager::playEff(float t)
{
	auto particle = ParticleSystemQuad::create("images/common/Eff/baoxiangbao.plist");
	particle->setScale(1.2f);
	particle->setPosition(Vec2(-66, 74));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(2.f);									// 设置粒子系统的持续时间秒  
	winNode->addChild(particle);
}



//播放金币喷泉和数字滚动
void ResultBoardManager::goldandnumroll(float t)
{
	if (scoreNum > _ZERO)
	{
		playGoldFly(scoreLabel->getPosition() + Vec2(-70, -20), Vec2(-485, 150.5));
		_PLAY_SOUND_SHARK(WINGOLD_MUSIC);
		//schedule(schedule_selector(ResultBoardManager::numroll), 0.1f);
	}
}
//数字滚动效果
void ResultBoardManager::numroll(float t)
{
	//几次滚动完成
	static int index = _ZERO;
	if (index < 30)
	{
		setGetnum((SCORE)(scoreNum - ((1 + index) * scoreNum) / 30));
		index++;
	}
	else
	{
		setGetnum(_ZERO);
		index = _ZERO;
		unschedule(schedule_selector(ResultBoardManager::numroll));
	}
}
//设置GetNum艺术数字string
void ResultBoardManager::setGetnum(SCORE l)
{
	std::stringstream str;
	str << l;
	scoreLabel->setString(str.str());
}
//播放金币飞行动画
void ResultBoardManager::playGoldFly(Vec2 startPos, Vec2 endPos)
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (size_t i = _ZERO; i < 20; i++)
	{
		//创建动画
		Sprite* pSprite = Sprite::create();
		winNode->addChild(pSprite);
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
			MoveTo::create(time0, startPos + Vec2(random(-150, 80), random(0, 180))),
			CCEaseExponentialInOut::create(ScaleTo::create(time0, random(0.55f, 0.77f))),
			NULL);
		auto ac2 = DelayTime::create(0.2f);
		float time1 = random(0.5f, 1.3f);
		auto ac3 = Spawn::create(
			CCEaseExponentialIn::create(MoveTo::create(time1, endPos)),
			//CCEaseExponentialIn::create((FadeOut::create(time1))),
			NULL);
		auto func0 = CallFuncN::create([this](Node* n){	n->removeFromParent(); });
		auto func1 = CallFuncN::create([this](Node* n){ n->setVisible(true); });

		auto seq = Sequence::create(ac0, func1, ac1, ac2, ac3, func0, NULL);
		pSprite->runAction(seq);
	}
}