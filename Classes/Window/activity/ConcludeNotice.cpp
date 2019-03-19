#include "ConcludeNotice.h"
#include "LogicActivity.h"
#include "LanguageConfig.h"
#include "Player/Player.h"
#include "Util/GameUtil.h"




ConcludeNotice::ConcludeNotice() :
m_yuanbao(0),
m_score(0),
m_yuanbaoNum(nullptr),
m_scoreNum(nullptr),
m_text(nullptr)
{
}

ConcludeNotice::~ConcludeNotice()
{
}


void ConcludeNotice::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZActiveGrabRed.csb");
	CCASSERT(pMainNode, "ConcludeNotice::setContent() layout/JZActiveGrabRed.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_yuanbaoNum = dynamic_cast<TextAtlas*>(_seekNodeByName(pMainNode, "yuanbaoscore"));
	CC_ASSERT(m_yuanbaoNum != nullptr);

	m_scoreNum = dynamic_cast<TextAtlas*>(_seekNodeByName(pMainNode, "score"));
	CC_ASSERT(m_scoreNum != nullptr);

	m_text = dynamic_cast<Text*>(_seekNodeByName(pMainNode, "text2"));
	CC_ASSERT(m_text != nullptr);

	//设置按钮(OK)关闭事件
	_addButtonClickEvent(pMainNode, "ok_bt", [this](Ref* sender)
	{
		Player::getInstance()->addCoin(m_score);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);

		Player::getInstance()->addGold(m_yuanbao);
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);

		//提交数据
		LogicActivity::getInstance()->sendGrabRedData(m_score, m_yuanbao);	//由于是由话费版抢红包修改而来（保留原接口不变）  话费改成元宝直接取整
		this->close();
	});
}

void ConcludeNotice::onEnter()
{
	WindowBase::onEnter();
	ScaleIn();
}

void ConcludeNotice::updateWindow(int nCode, void* pData)
{
	//抢红包数据来了
	if (nCode == 0)
	{
		int* data = (int*)pData;
		m_yuanbao = (*data) % 100;
		m_score = (*data) / 100;
		//如果是VIP则金币翻倍
		if (Player::getInstance()->getHallData().dwVipLevel > 0)
		{
			m_score *= 2;
		}

		//刷新界面
		SetYunBaoNum(m_yuanbao);
		SetScoreNum(m_score);
		if (m_score >= 20000)
		{
			SetText(GETSTRING(1306));
		}
		else if (m_score >= 10000 && m_score < 20000)
		{
			SetText(GETSTRING(1307));
		}
		else if (m_score >= 3000 && m_score < 10000)
		{
			SetText(GETSTRING(1308));
		}
		else if (m_score >= 0 && m_score < 3000)
		{
			SetText(GETSTRING(1309));
		}

	}
}

void ConcludeNotice::updateWindow(int nCode)
{
}

void ConcludeNotice::SetYunBaoNum(int g)
{
	std::stringstream str;
	str << g;
	m_yuanbaoNum->setString(str.str());
}

void ConcludeNotice::SetScoreNum(int g)
{
	std::stringstream str;
	str << g;
	m_scoreNum->setString(str.str());

}


void ConcludeNotice::SetText(const std::string &str)
{
	m_text->setString(str);
}