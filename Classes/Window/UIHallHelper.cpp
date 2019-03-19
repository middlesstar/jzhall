#include "UIHallHelper.h"

cocos2d::ui::EditBox* UIHallHelper::createAccountEditBox(TextField* pRawInput, int nMaxLength, const std::string& strBgImg, 
	int nType/* = EEBT_NUM*/, cocos2d::ui::EditBoxDelegate* pDelegate/* = nullptr*/)
{
	if (!pRawInput || !pRawInput->getParent())
	{
		return nullptr;
	}

	Size size = pRawInput->getParent()->getContentSize();
	cocos2d::ui::EditBox *pEditBox = cocos2d::ui::EditBox::create(size, cocos2d::ui::Scale9Sprite::create(strBgImg));
	pEditBox->setFontSize(pRawInput->getFontSize());
	pEditBox->setFontColor(pRawInput->getColor());
	pEditBox->setPlaceholderFontName(pRawInput->getFontName().c_str());
	pEditBox->setMaxLength(nMaxLength);
	pEditBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	pEditBox->setTag(nType);
	pEditBox->setDelegate(pDelegate);

	pEditBox->setAnchorPoint(pRawInput->getParent()->getAnchorPoint());
	pEditBox->setPosition(pRawInput->getParent()->getPosition());

	pRawInput->getParent()->getParent()->addChild(pEditBox);
	pRawInput->getParent()->removeFromParent();

	switch (nType)
	{
	case EEBT_NUM:
		pEditBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
		break;
	case EEBT_TEXT:
		pEditBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
		break;
	}

	return pEditBox;
}
