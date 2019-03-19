#include "RedPacket.h"



RedPacket::RedPacket() :
m_bIsActivity(false),
m_fadeout(NULL),
m_screenFadeout(NULL),
m_colorLayer(NULL),
m_bIsCreatHF(true)
{
}

RedPacket::~RedPacket()
{
	m_packetList.clear();
	m_fadeout->release();
	m_screenFadeout->release();
	m_colorLayer->release();

}


void RedPacket::OnInit()
{
	m_fadeout = CCFadeOut::create(PACKET_TEXT_MAX_TIME);
	CC_ASSERT(NULL != m_fadeout);
	m_fadeout->retain();


	m_screenFadeout = CCFadeOut::create(PACKET_SCREEN_CLEAR);
	CC_ASSERT(NULL != m_screenFadeout);
	m_screenFadeout->retain();



	m_colorLayer = CCLayerColor::create(PACKET_LAYER_COLOR, VISIBLE_SIZE.x, VISIBLE_SIZE.y);
	CC_ASSERT(NULL != m_colorLayer);
	addChild(m_colorLayer);
	m_colorLayer->setOpacity(_ZERO);
	m_colorLayer->retain();

}


void RedPacket::update(float t)
{
	//位置临时变量
	Vec2 v2;

	for (Vector<OnePacket*>::iterator itor = m_packetList.begin(); itor != m_packetList.end(); /*++itor*/)
	{
		OnePacket *index = (OnePacket*)(*itor);

		v2 = index->getPosition();
		v2 += Vec2(index->m_vX * t, index->m_vY * t);
		index->setPosition(v2);
		index->m_time += t;
		if (index->m_time >= PACKET_UP_DOWN_TIME)
		{
			index->m_time = _ZERO;
			index->m_vY = -index->m_vY;
		}
		if (v2.x > VISIBLE_SIZE.x)
		{
			removeChild(index, true);
			itor = m_packetList.erase(itor);
		}
		else
		{
			++itor;
		}
	}


	//红包文字
	for (Vector<PacketText*>::iterator itor = m_textList.begin(); itor != m_textList.end(); /*++itor*/)
	{
		PacketText *index = (PacketText*)(*itor);
		v2 = index->getPosition();
		v2 += Vec2(_ZERO, PACKET_TEXT_V_Y * t);
		index->setPosition(v2);
		index->time += t;
		if (index->time > PACKET_TEXT_MAX_TIME)
		{
			removeChild(index, true);
			itor = m_textList.erase(itor);
		}
		else
		{
			++itor;
		}
	}

}

void RedPacket::StartPlay()
{
	if (m_bIsActivity)
	{
		return;
	}
	SetCreatHF(true);
	m_bIsActivity = true;
	scheduleUpdate();
	schedule(schedule_selector(RedPacket::CreatPacket), PACKET_TIME);

}


void RedPacket::StopPlay()
{
	if (!m_bIsActivity)
	{
		return;
	}
	m_bIsActivity = false;
	//unscheduleUpdate();
	unschedule(schedule_selector(RedPacket::CreatPacket));
}

void RedPacket::BombGet(int &gold, float &hf)
{
	//m_colorLayer->stopAllActions();
	m_colorLayer->setOpacity(PACKET_TRANSPARENT);
	m_colorLayer->runAction(m_screenFadeout->clone());
	scheduleOnce(schedule_selector(RedPacket::UpdateColorLayer), PACKET_SCREEN_CLEAR);

	for (Vector<OnePacket*>::iterator itor = m_packetList.begin(); itor != m_packetList.end(); ++itor)
	{
		OnePacket *index = (OnePacket*)(*itor);
		EatPacket(index->m_num, index->m_type, index->getPosition());
		switch (index->m_type)
		{
		case RedPacket::OnePacket::PACKET_GOLD_SMALL:
		case RedPacket::OnePacket::PACKET_GOLD_MID:
		case RedPacket::OnePacket::PACKET_GOLD_BIG:
		case RedPacket::OnePacket::BAO:
			gold += (int)index->m_num;
			break;
		case RedPacket::OnePacket::PACKET_HF_SMALL:
		case RedPacket::OnePacket::PACKET_HF_BIG:
			hf += index->m_num;
			break;
		case RedPacket::OnePacket::BOMB_GET:
			break;
		case RedPacket::OnePacket::BOMB_DIE:
			break;
		default:
			break;
		}
		//不能清除加分文字
		removeChild(index);
	}
	m_packetList.clear();
}

void RedPacket::UpdateColorLayer(float t)
{
	m_colorLayer->stopAllActions();
	m_colorLayer->setOpacity(PACKET_TRANSPARENT);
	m_colorLayer->runAction(m_screenFadeout->clone());

}

void RedPacket::BombDie()
{
	m_packetList.clear();
	removeAllChildren();
	addChild(m_colorLayer);
	m_colorLayer->setOpacity(PACKET_TRANSPARENT);
	m_colorLayer->runAction(m_screenFadeout->clone());
	scheduleOnce(schedule_selector(RedPacket::UpdateColorLayer), PACKET_SCREEN_CLEAR);

}

void RedPacket::EatPacket(float t, OnePacket::enumType e, Vec2 v)
{
	PacketText *text = NULL;
	std::stringstream str;
	str << t;

	switch (e)
	{
	case RedPacket::OnePacket::PACKET_GOLD_SMALL:
	case RedPacket::OnePacket::PACKET_GOLD_MID:
	case RedPacket::OnePacket::PACKET_GOLD_BIG:
	case RedPacket::OnePacket::BAO:
		text = PacketText::create("+" + str.str(), TEXT_FONT, PACKET_TEXT_SIZE);
		text->setColor(PACKET_TEXT_COLOR0);
		text->time = _ZERO;
		text->setPosition(v);
		addChild(text);
		text->runAction(m_fadeout->clone());
		m_textList.pushBack(text);
		break;
	case RedPacket::OnePacket::PACKET_HF_SMALL:
	case RedPacket::OnePacket::PACKET_HF_BIG:
		text = PacketText::create("+" + str.str(), TEXT_FONT, PACKET_TEXT_SIZE);
		text->setColor(PACKET_TEXT_COLOR1);
		text->time = _ZERO;
		text->setPosition(v);
		addChild(text);
		text->runAction(m_fadeout->clone());
		m_textList.pushBack(text);
		break;
	case RedPacket::OnePacket::BOMB_GET:

		break;
	case RedPacket::OnePacket::BOMB_DIE:

		break;
	default:
		break;
	}
}


void RedPacket::CreatPacket(float t)
{
	OnePacket *g = NULL;;

	int x = random(0, 1000);
	//产生小红包
	if (x < 800)
	{
		g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(RED_PAKAGE2));
		g->m_type = OnePacket::PACKET_GOLD_SMALL;
		g->m_num = random(0, 100);
		g->m_vX = PACKET_V_MIN;
	}
	//产生中红包
	else if (x < 900)
	{
		g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(RED_PAKAGE1));
		g->m_type = OnePacket::PACKET_GOLD_MID;
		g->m_num = random(100, 500);
		g->m_vX = PACKET_V_MID;
	}
	//产生大红包
	else if (x < 910)
	{
		g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(RED_PAKAGE0));
		g->m_type = OnePacket::PACKET_GOLD_BIG;
		g->m_num = random(500, 2000);
		g->m_vX = PACKET_V_MAX;
	}
	
	//产生小话费
	else if (x < 915)
	{
		if (m_bIsCreatHF)
		{
			g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_TICKET0));
			g->m_type = OnePacket::PACKET_HF_SMALL;
			g->m_num = random(1,5);
			g->m_vX = PACKET_V_MAX;
		}

	}
	//产生宝箱
	else if (x < 916)
	{
		g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_BAO));
		g->m_type = OnePacket::BAO;

		g->m_num = random(10000, 12000);
		g->m_vX = PACKET_V_MAX;

	}
	//产生大话费
	else if (x < 917)
	{
		if (m_bIsCreatHF)
		{
			g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_TICKET0));
			g->m_type = OnePacket::PACKET_HF_BIG;

			g->m_num = random(5,10);
			g->m_vX = PACKET_V_MAX;
		}

	}
	//黑炸弹
	else if (x < 990)
	{
		g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_RBOMB1));
		g->m_type = OnePacket::BOMB_DIE;

		g->m_num = _ZERO;
		g->m_vX = PACKET_V_MID;
	}
	//红炸弹
	else
	{
		g = OnePacket::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_RBOMB0));
		g->m_type = OnePacket::BOMB_GET;

		g->m_num = _ZERO;
		g->m_vX = PACKET_V_MID;
	}

	//起始位置
	g->setPosition(_ZERO, VISIBLE_SIZE.y * random(0.3f, 0.85f));
	m_packetList.pushBack(g);
	g->setScale(ALL_SCALE);
	g->m_time = _ZERO;
	g->m_vY = PACKET_UP_DOWN_V;
	float angle = PACKET_ROTATE_ANGLE* random(0.5f, 1.0f);							//旋转范围
	g->runAction(RepeatForever::create(
		Sequence::create(
		CCRotateTo::create(PACKET_ROTATE_TIME, Vec3(_ZERO, _ZERO, angle)),
		CCRotateTo::create(PACKET_ROTATE_TIME, Vec3(_ZERO, _ZERO, -angle)),
		NULL)));
	addChild(g);

}