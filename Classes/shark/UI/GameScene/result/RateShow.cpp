#include "RateShow.h"

RateShow::RateShow() : rateLabel(nullptr)
{

}
RateShow::~RateShow()
{

}
void RateShow::setContent()
{
	rateLabel = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "textBeiLv"));
	CC_ASSERT(rateLabel != nullptr);

	rateLabel->getParent()->setVisible(false);
}

//Æô¶¯ÉÁË¸
void RateShow::startPlay()
{
	rateLabel->getParent()->setVisible(true);
}
//¹Ø±ÕÉÁË¸
void RateShow::stopPlay()
{
	rateLabel->getParent()->setVisible(false);
}

void RateShow::setBeiLv(int index)
{
	rateLabel->setString(StringUtils::format("%d", index));
}