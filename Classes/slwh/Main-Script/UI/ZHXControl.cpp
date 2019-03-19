#include "ZHXControl.h"

#define ZHX_DOWN	50
#define ZHX_UP		61



const float	ZHXControl::Speed_Add = 80.0f;               
const float	ZHXControl::Speed_Sub = 75.0f;               
const float	ZHXControl::Speed_MinSub = 20.0f;            			
ZHXControl::ZHXControl():
m_zhxUp(NULL),
m_zhxDown(NULL),
m_downPt(_ZERO,_ZERO),
m_upPt(_ZERO, _ZERO),
m_speed(_ZERO),
m_time(_ZERO),
m_nowZHX(InvalidZHX)
{
}


ZHXControl::~ZHXControl()
{
	if (NULL != m_zhxUp)
	{
		m_zhxUp->release();
		m_zhxUp = NULL;
	}

	if (NULL != m_zhxDown)
	{
		m_zhxDown->release();
		m_zhxDown = NULL;
	}
}


void ZHXControl::Start()
{

	m_zhxUp = dynamic_cast<ImageView*>(m_node->getChildByTag(ZHX_UP));
	CC_ASSERT(NULL != m_zhxUp);
	m_zhxUp->retain();


	m_zhxDown = dynamic_cast<ImageView*>(m_node->getChildByTag(ZHX_DOWN));
	CC_ASSERT(NULL != m_zhxDown);
	m_zhxDown->retain();


	//位移量
	//像素宽高为56
	PixelH = m_zhxUp->getPositionY() - m_zhxDown->getPositionY();

	//循环索引
	m_ZHXMap[Zhuang] = He;
	m_ZHXMap[He] = Xian;
	m_ZHXMap[Xian] = Zhuang;

	//索引资源
	m_ZHXTex[Zhuang] = Path_PNG_ZHUANG;
	m_ZHXTex[He] = Path_PNG_HE;
	m_ZHXTex[Xian] = Path_PNG_XIAN;

	//记录初始位置
	m_downPt = m_zhxDown->getPosition();
	m_upPt = m_zhxUp->getPosition();

	//绘制裁剪区域
	CCDrawNode* shap = CCDrawNode::create();
	Vec2 v(PixelH / 2, PixelH / 2);
	shap->drawSolidRect(m_downPt - v, m_downPt + v, ccc4f(_ORIGINAL, _ORIGINAL, _ORIGINAL, _ORIGINAL));
	CCClippingNode* cliper = CCClippingNode::create();
	cliper->setStencil(shap);
	cliper->setAnchorPoint(Vec2(.5f, .5f));
	cliper->setPosition(Vec2(_ZERO,_ZERO));

	m_node->addChild(cliper);
	
	m_zhxUp->removeFromParent();
	m_zhxDown->removeFromParent();
	cliper->addChild(m_zhxDown);
	cliper->addChild(m_zhxUp);

	//初始化信息
	m_nowZHX = Zhuang;
	m_zhxDown->loadTexture(m_ZHXTex[m_nowZHX], TextureResType::PLIST);
	m_zhxUp->loadTexture(m_ZHXTex[m_ZHXMap[m_nowZHX]], TextureResType::PLIST);


	//测试
	//StartPlay(enumGameZXH::Xian);
}

// Update is called once per frame
void ZHXControl::update(float time)
{
	if (m_bIsRunning)
	{
		switch (m_enumState)
		{
		case ZHXNormal:
			//这里是打酱油的
			break;
		case ZHXSpeedAdd:
			//加速度
			m_speed += Speed_Add * time;

			if (m_time >= (float)ZHXSpeedAdd)
			{
				m_enumState = ZHXSpeedMax;
			}
			break;
		case ZHXSpeedMax:
			//匀速

			if (m_time >= (float)ZHXSpeedMax)
			{
				m_enumState = ZHXSpeedSub;
			}
			break;
		case ZHXSpeedSub:
			//减速
			m_speed -= Speed_Sub * time;


			if (m_time >= (float)ZHXSpeedSub)
			{
				m_enumState = ZHXSpeedMin;
			}
			break;
		case ZHXSpeedMin:
			//确定结果
			m_speed -= Speed_MinSub * time;


			break;
		default:
			break;
		}

		m_time += time;
		RoundOne(time);
	}
}



//滚动一次动画
void ZHXControl::RoundOne(float time)
{
	//zhxDown.rectTransform.position = new Vector3(downPt.x, downPt.y + offset * PixelH, 0.0f);
	Move(m_zhxDown, Vec2(_ZERO, -m_speed * time));
	m_zhxUp->setPosition(m_zhxDown->getPosition() + Vec2(_ZERO, PixelH));

	float offset = (m_downPt.y - m_zhxDown->getPositionY()) / PixelH;


	/*m_zhxDown.fillAmount = 1.0f - offset;
	m_zhxUp.fillAmount = offset;*/

	//一次动画播放结束
	if (offset >= 1.0f)
	{
		//交换上下
		ImageView* swap = m_zhxDown;
		m_zhxDown = m_zhxUp;
		m_zhxUp = swap;
		
		m_zhxUp->setPosition(m_upPt);
		m_nowZHX = m_ZHXMap[m_nowZHX];
		m_zhxDown->loadTexture(m_ZHXTex[m_nowZHX], TextureResType::PLIST);
		m_zhxUp->loadTexture(m_ZHXTex[m_ZHXMap[m_nowZHX]], TextureResType::PLIST);


		//如果进入最终减速阶段 且结果正确
		if (m_enumState == ZHXSpeedMin && m_nowZHX == m_rsZHX)
		{

			m_zhxDown->setPosition(m_downPt);
			m_zhxUp->setPosition(m_upPt);
			//播放音乐
			switch (m_rsZHX)
			{
			case Zhuang:
				_PLAY_MUSCI_EFF(_ZHUANG);
				break;
			case He:
				_PLAY_MUSCI_EFF(_HE);
				break;
			case Xian:
				_PLAY_MUSCI_EFF(_XIAN);
				break;
			default:
				break;
			}
			
			OnInit();
			unscheduleUpdate();

		}
	}
}

//初始化一局游戏
void ZHXControl::OnInit()
{
	m_speed = _ZERO;
	m_time = _ZERO;
	m_bIsRunning = false;
	m_enumState = ZHXNormal;
}


void ZHXControl::StartPlay(int index)
{
	StartPlay(static_cast<enumGameZXH>(index));
}


//开始转动接口
void ZHXControl::StartPlay(enumGameZXH e)
{
	if (NULL == m_node)
		return;

	if (m_bIsRunning)
		return;

	if ((int)e < _ZERO || (int)e >= (int)enumGameZXH::ZXHmax)
	{
		log("------------error： ZHXControl::StartPlay %d------------------", (int)e);
		return;
	}

	//开始加速
	m_enumState = ZHXSpeedAdd;
	m_rsZHX = e;
	m_bIsRunning = true;
	scheduleUpdate();
}
