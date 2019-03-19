#include "XYCJRes.h"

#define RES1			"res1"
#define RES2			"res2"
#define RES3			"res3"
#define YUANBAO_IMG		"ybimg"
#define CP_IMG			"cpimg"

#define XYCJ_PATH_ROOT						"game/game_results/gr_prize_1%02d.png"
#define	Path_XYCJ_Gold_YB					"game/game_results/gr_prize_goldyb.png"
#define	Path_XYCJ_Gold_Ticket				"game/game_results/gr_prize_goldcp.png"

#define ZP_MAX		16

XYCJRes::XYCJRes():
m_yaunbao(nullptr),
m_cpNum(nullptr),
m_yuanbaoImg(nullptr),
m_cpImg(nullptr)
{
	memset(m_res,NULL,sizeof(m_res));
}

XYCJRes::~XYCJRes()
{
}

void XYCJRes::Start()
{
	m_res[0] = dynamic_cast<ImageView*>(m_node->getChildByName(RES1));
	CC_ASSERT(m_res[0] != nullptr);

	m_res[1] = dynamic_cast<ImageView*>(m_node->getChildByName(RES2));
	CC_ASSERT(m_res[1] != nullptr);

	m_res[2] = dynamic_cast<ImageView*>(m_node->getChildByName(RES3));
	CC_ASSERT(m_res[2] != nullptr);

	m_yuanbaoImg = dynamic_cast<ImageView*>(m_node->getChildByName(YUANBAO_IMG));
	CC_ASSERT(m_yuanbaoImg != nullptr);

	m_yaunbao = dynamic_cast<TextAtlas*>(m_yuanbaoImg->getChildByName("num"));
	CC_ASSERT(m_yaunbao != nullptr);

	m_cpImg = dynamic_cast<ImageView*>(m_node->getChildByName(CP_IMG));
	CC_ASSERT(m_cpImg != nullptr);

	m_cpNum = dynamic_cast<TextAtlas*>(m_cpImg->getChildByName("num"));
	CC_ASSERT(m_cpNum != nullptr);

	
	m_node->setVisible(false);
}


void XYCJRes::setRes(int aType,int* a, int yuanbao /*= 0*/, int cp /*= 0*/)
{

	//转到了LUCK 
	if (a[0] == 1)
	{
		for (size_t i = _ZERO; i < 3; i++)
		{

			//取接下来的3个结果
			m_res[i]->setVisible(true);

			if (a[i + 1] == 0 || a[i + 1] == 8)						//A区间
			{
				aType == 1 ? m_res[i]->loadTexture(Path_XYCJ_Gold_YB, TextureResType::PLIST) : m_res[i]->loadTexture(Path_XYCJ_Gold_Ticket, TextureResType::PLIST);
			}
			else if (a[i + 1] == 5 || a[i + 1] == 13)				//副对角线
			{
				m_res[i]->loadTexture(Path_XYCJ_Gold_YB, TextureResType::PLIST);
			}
			else
			{
				char bufpath[256];
				sprintf(bufpath, XYCJ_PATH_ROOT, a[i + 1]);
				m_res[i]->loadTexture(bufpath, TextureResType::PLIST);
			}	
		}
	}
	else
	{
		//不是LUCK 只取第一个
		m_res[0]->setVisible(true);
		m_res[1]->setVisible(false);
		m_res[2]->setVisible(false);


		if (a[0] == 0 || a[0] == 8)						//A区间
		{
			aType == 1 ? m_res[0]->loadTexture(Path_XYCJ_Gold_YB, TextureResType::PLIST) : m_res[0]->loadTexture(Path_XYCJ_Gold_Ticket, TextureResType::PLIST);
		}
		else if (a[0] == 5 || a[0] == 13)				//副对角线
		{
			m_res[0]->loadTexture(Path_XYCJ_Gold_YB, TextureResType::PLIST);
		}
		else
		{
			char bufpath[256];
			sprintf(bufpath, XYCJ_PATH_ROOT, a[0]);
			m_res[0]->loadTexture(bufpath, TextureResType::PLIST);
		}
	}

	m_yuanbaoImg->setVisible(true);
	m_cpImg->setVisible(true);

	//有元宝就显示 没有就GG
	yuanbao > _ZERO ? setYBui(yuanbao) : m_yuanbaoImg->setVisible(false);

	//有船票就显示 没有就GG
	cp > _ZERO ? setCPui(cp) : m_cpImg->setVisible(false);
	
}

//设置元宝数字UI
void XYCJRes::setYBui(int g)
{
	std::stringstream str;
	str << g;
	m_yaunbao->setString(str.str());
}

//设置船票数字UI
void XYCJRes::setCPui(int g)
{
	std::stringstream str;
	str << g;
	m_cpNum->setString(str.str());
}