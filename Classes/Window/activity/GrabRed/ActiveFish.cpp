#include "ActiveFish.h"
#include "ActiveConfig.h"


float ActiveFish::JUMP_MAX = FISH_JUMP_V;
float ActiveFish::_G = FISH_G;
float ActiveFish::DIVE_G = FISH_DIVE_V;
ActiveFish::ActiveFish() :
m_vX(FISH_MOVE_X_V),
m_fish(NULL),
m_bIsActivity(false),
m_bIsLeft(false),
m_vY(_ZERO),
m_fishState(NORMAL)
{
	//memset(m_tex[2], NULL, sizeof(m_tex));

}

ActiveFish::~ActiveFish()
{
}

//溅起水花
void ActiveFish::PushLiquid(float force)
{

	
	//MusciManager::GetInstance()->PlayMusic(MusciManager::enumMUSIC_FISH_JUMP);
	_PLAY_MUSCI_EFF(MUSIC_FISH_JUMP);

	int num = (int)(force * 20) + 5;
	Vec2 v2 = m_fish->getPosition();
	for (size_t i = _ZERO; i < num; i++)
	{
		Liquid* l = Liquid::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(RP_DROPS));
		l->setPosition(v2);
		l->m_rotate = random(-force * 400 - 400, force * 400 + 400);
		l->m_vX = random(-force * 200 - 100, force * 200 + 100);
		l->m_vY = random(force * 400, force * 800);
		l->m_vZ = random(-force * 200 - 100, force * 200 + 100);
		l->setScale(ALL_SCALE);
		addChild(l);
		m_liquidList.pushBack(l);
		l->runAction(CCFadeOut::create(l->m_vY / force / 600));						//让水滴有层次感消失
	}
}

void ActiveFish::OnInit()
{

	m_fish = Sprite::createWithSpriteFrameName(_FISH0);
	CC_ASSERT(NULL != m_fish);
	m_fish->setPosition(Vec2(_ZERO, FISH_MIN_HEIGHT));

	//主管要求 说鱼是图片的0.5 很好看
	m_fish->setScale(0.5);
	addChild(m_fish);


	//进行高度适配
	/*Vec2 visible = Director::getInstance()->getVisibleSize();
	float scale = visible.y / VISIBLE_SIZE.y;
	JUMP_MAX *= scale;
	_G *= scale;
	DIVE_G *= scale;*/


}



void ActiveFish::StartPlay()
{
	if (m_bIsActivity)
	{
		return;
	}
	m_bIsActivity = true;
	m_bIsLeft = false;
	scheduleUpdate();
}

Rect ActiveFish::GetRect()
{
	if (!m_bIsActivity)
	{
		return Rect();
	}
	return m_fish->boundingBox();
}


void ActiveFish::StopPlay()
{
	if (!m_bIsActivity)
	{
		return;
	}

	m_bIsActivity = false;
	unscheduleUpdate();
}

void ActiveFish::Jump()
{
	if (!m_bIsActivity)
	{
		return;
	}

	if (m_fishState == NORMAL)
	{
		//鱼跳起音乐
		_PLAY_MUSCI_EFF(MUSIC_FISH_JUMP);
		m_fishState = JUMP_1;
		m_fish->initWithSpriteFrameName(_FISH1);
		m_vY = FISH_JUMP_FORCE1 * JUMP_MAX;

	}
	else if (m_fishState == JUMP_1)
	{
		//鱼跳起音乐
		_PLAY_MUSCI_EFF(MUSIC_FISH_JUMP);
		m_fishState = JUMP_2;
		if (m_vY < _ZERO)
		{
			m_vY = _ZERO;
		}
		else	
			m_vY *= FISH_JUMP_LOSE;
		m_vY += FISH_JUMP_FORCE2 * JUMP_MAX;
	}
	
	
}



void ActiveFish::update(float t)
{

	Vec3 v3;
	for (Vector<Liquid*>::iterator itor = m_liquidList.begin(); itor != m_liquidList.end(); /*++itor*/)
	{
		Liquid *index = (Liquid*)(*itor);
		v3 = index->getPosition3D();
		v3.x += index->m_vX * t;
		v3.y += index->m_vY * t;
		v3.z += index->m_vZ * t;
		index->setPosition3D(v3);
		index->setRotation(index->getRotation() + index->m_rotate * t);
		index->m_vY -= _G * t;
		if (v3.y < _ZERO)
		{
			index->removeFromParent();
			itor = m_liquidList.erase(itor);
		}
		else
		{
			++itor;
		}
	}

	Vec2 v = m_fish->getPosition();
	if (m_bIsLeft)
	{
		v -= Vec2(m_vX * t, _ZERO);
		if (v.x < FISH_OFFSET_X)
		{
			m_bIsLeft = false;
			m_fish->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
		}
	}

	else
	{
		v += Vec2(m_vX * t, _ZERO);
		if (v.x > VISIBLE_SIZE.x - FISH_OFFSET_X)
		{
			m_bIsLeft = true;
			m_fish->setRotation3D(Vec3(_ZERO, 180, _ZERO));		//反向
		}
	}

	switch (m_fishState)
	{
	case NORMAL:
		//打个酱油
		break;
	case JUMP_1:
	case JUMP_2:
		v += Vec2(_ZERO, m_vY * t);
		m_vY -= _G*t;

		if (m_bIsLeft)
		{
			if (m_vY > _ZERO)
			{
				m_fish->setRotation(360 - atan(-m_vY / m_vX / FISH_JUMP_SLIDER) / _PI * 180);

			}
			else
				m_fish->setRotation(atan(m_vY / m_vX / FISH_JUMP_SLIDER) / _PI * 180);

		}
		else
		{
			if (m_vY > _ZERO)
			{
				m_fish->setRotation(atan(-m_vY / m_vX / FISH_JUMP_SLIDER) / _PI * 180);

			}
			else
				m_fish->setRotation(360 - atan(m_vY / m_vX / FISH_JUMP_SLIDER) / _PI * 180);

		}

		if (v.y < FISH_MIN_HEIGHT)
		{
			m_fishState = DIVE;
			m_fish->setRotation(_ZERO);
			PushLiquid(fabs(m_vY) / 1200);
			m_vY *= FISH_DIVE_LOSE;
		}

		break;
	case DIVE:
		m_fish->initWithSpriteFrameName(_FISH0);
		m_vY += DIVE_G *t;
		v += Vec2(_ZERO, m_vY * t);

		if (v.y >= FISH_MIN_HEIGHT)
		{
			m_fishState = NORMAL;
			v.y = FISH_MIN_HEIGHT;
			//m_energyFront->setScaleX(m_jumpForce);
		}
		break;
	default:
		break;
	}

	m_fish->setPosition(v);

}

