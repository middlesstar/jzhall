#include "ResRecord.h"

//节点下各个记录节点的逻辑标签 初始顺序
#define	RES_ARRAY		{70, 63, 64, 65, 66, 67, 68, 69}

//每个记录节点下ZHX的逻辑名称
#define ZHX_NMAE		"ZHX_Image"

ResRecord::ResRecord():
m_listAcIndex(_ZERO),								
m_ptEnd(_ZERO, _ZERO),
offsetY(_ZERO),
m_listRear(_ZERO),
m_bIsVisible(false)
{
	memset(m_record, NULL, sizeof(m_record));
}

ResRecord::~ResRecord()
{
}

void ResRecord::Start()
{
	//用ID关联资源图片 CJ XYCJ _4LF 都将处理为普通记录结果
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal0)]		= Path_SMALL_Red_Animal0;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal1)]		= Path_SMALL_Red_Animal1;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal2)]		= Path_SMALL_Red_Animal2;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal3)]		= Path_SMALL_Red_Animal3;

	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal0)]		= Path_SMALL_Green_Animal0;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal1)]		= Path_SMALL_Green_Animal1;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal2)]		= Path_SMALL_Green_Animal2;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal3)]		= Path_SMALL_Green_Animal3;

	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal0)]	= Path_SMALL_Yellow_Animal0;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal1)]	= Path_SMALL_Yellow_Animal1;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal2)]	= Path_SMALL_Yellow_Animal2;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal3)]	= Path_SMALL_Yellow_Animal3;

	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal0)]		= Path_SMALL_ALL_Animal0;
	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal1)]		= Path_SMALL_ALL_Animal1;
	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal2)]		= Path_SMALL_ALL_Animal2;
	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal3)]		= Path_SMALL_ALL_Animal3;

	m_map[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Red, enumAnimalType::AnimalMax)]		= Path_SMALL_Red_AnimalAll;
	m_map[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Green, enumAnimalType::AnimalMax)]		= Path_SMALL_Green_AnimalAll;
	m_map[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Yellow, enumAnimalType::AnimalMax)]		= Path_SMALL_Yellow_AnimalAll;

	m_map[_RES_KEY(enumSpecialRs::DMG, enumAnimalColor::ColorMax, enumAnimalType::AnimalMax)]	= Path_SMALL_All_AnimalAll;

	//关联节点
	int a[] = RES_ARRAY;
	for (size_t i = _ZERO; i < RECORD_BOARD_MAX; i++)
	{
		Node* n = m_node->getChildByTag(a[i]);
		m_record[i].animal = dynamic_cast<ImageView*>(n);
		CC_ASSERT(nullptr != m_record[i].animal);
		m_record[i].zhx = dynamic_cast<ImageView*>(n->getChildByName(ZHX_NMAE));
		CC_ASSERT(nullptr != m_record[i].zhx);
	}

	//指向队尾
	m_listRear = RECORD_BOARD_MAX - 1;
	//计算2个记录间的距离
	offsetY = m_record[1].animal->getPosition().y - m_record[0].animal->getPosition().y;
	//记录队尾坐标
	m_ptEnd = m_record[m_listRear].animal->getPosition();

	//完全透明
	m_node->setOpacity(_ZERO);
	//测试
	//Add(AnimalResult(DMG,ColorMax,AnimalMax, Zhuang));
}

void ResRecord::update(float t)
{
	int index = m_listRear;
	for (size_t i = _ZERO; i < RECORD_BOARD_MAX; i++)
	{
		float t = (float)m_listAcIndex / List_AC_MAX;

		m_record[index].animal->setPosition(Vec2(m_ptEnd.x, m_ptEnd.y - offsetY * (i + t)));
		
		//改变队尾透明度
		m_record[m_listRear].animal->setOpacity((1.0f - 0.4f * t) * TRANSPARENT_MAX);
		//从队尾向队头输出
		if (--index < _ZERO)
			index = RECORD_BOARD_MAX - 1;
	}


	//动画结束添加新结果到列表
	if (++m_listAcIndex > List_AC_MAX)
	{
		m_listAcIndex = _ZERO;
		//if (--m_listRear < 0)
		//    m_listRear = List_Max - 1;

		if (++m_listRear >= RECORD_BOARD_MAX)
			m_listRear = _ZERO;

		switch (m_animalRes.e1)
		{
		case D3Y:
			m_animalRes.e2 = enumAnimalColor::ColorMax;
			break;
		case D4X:
			m_animalRes.e3 = enumAnimalType::AnimalMax;
			break;
		case DMG:
			m_animalRes.e2 = enumAnimalColor::ColorMax;
			m_animalRes.e3 = enumAnimalType::AnimalMax;
			break;
		case _4LF:
		case CJ:
		case XYCJ:
		default:
			m_animalRes.e1 = Normal;
			break;
		}

		m_record[m_listRear].animal->loadTexture(m_map[_RES_KEY(m_animalRes.e1, m_animalRes.e2, m_animalRes.e3)], TextureResType::PLIST);
		SetZHXTex(m_listRear, m_animalRes.e4);

		m_record[m_listRear].animal->setPosition(m_ptEnd);
		m_record[m_listRear].animal->setOpacity(TRANSPARENT_MAX);

		//关闭定时器
		unscheduleUpdate();
	}

}

void ResRecord::setVisible(bool b)
{
	if (m_bIsVisible == b)
		return;

	m_bIsVisible = b;

	if (m_bIsVisible == true)
	{
		m_node->runAction(FadeIn::create(0.5f));
	}
	else
		m_node->setOpacity(_ZERO);

}

//初始化数据中 个位为ZHX  算法为ID = 类型*1000 + 颜色*100 + 动物 *10 + ZHX
void ResRecord::InitResList(int* a)
{
	if (NULL == m_node)
		return;

	for (size_t i = _ZERO; i < RECORD_BOARD_MAX; i++)
	{
		//记录进行转换
		AnimalResult e(a[i]);
		
		switch (e.e1)
		{
		case D3Y:
			e.e2 = enumAnimalColor::ColorMax;
			break;
		case D4X:
			e.e3 = enumAnimalType::AnimalMax;
			break;
		case DMG:
			e.e2 = enumAnimalColor::ColorMax;
			e.e3 = enumAnimalType::AnimalMax;
			break;
		case _4LF:
		case CJ:
		case XYCJ:
		default:
			e.e1 = Normal;
			break;
		}
		m_record[RECORD_BOARD_MAX - 1 - i].animal->loadTexture(m_map[_RES_KEY(e.e1, e.e2, e.e3)], TextureResType::PLIST);
		SetZHXTex(RECORD_BOARD_MAX - 1 - i,e.e4);
	}
}

void ResRecord::SetZHXTex(int index, enumGameZXH type)
{
	if (index < _ZERO || index >= RECORD_BOARD_MAX)
	{
		return;
	}

	switch (type)
	{
	case enumGameZXH::Zhuang:
		//庄
		m_record[index].zhx->loadTexture(Path_PNG_ZHUANG, TextureResType::PLIST);
		break;
	case enumGameZXH::He:
		//和
		m_record[index].zhx->loadTexture(Path_PNG_HE, TextureResType::PLIST);
		break;
	case enumGameZXH::Xian:
		//闲
		m_record[index].zhx->loadTexture(Path_PNG_XIAN, TextureResType::PLIST);
		break;
	default:
		CCLOG("SLWH: error ! InitResList\n");
		break;
	}
}

//队列从上往下移动 最下面的一个将出现在最上方
void ResRecord::Add(enumSpecialRs e1, enumAnimalColor e2, enumAnimalType e3, enumGameZXH e4)
{
	if (NULL == m_node)
		return;

	_PLAY_MUSCI_EFF(RES_ADD);
	m_animalRes.InitWith(e1, e2, e3, e4);

	//开启动画效果
	scheduleUpdate();
}

void ResRecord::Add(AnimalResult e)
{
	Add(e.e1, e.e2, e.e3, e.e4);
}

