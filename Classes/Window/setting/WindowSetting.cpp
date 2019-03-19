#include "WindowSetting.h"
#include "Sound/SoundManager.h"

#define  IMG_ON_PATH		"images/hall/settings/settings_on.png"
#define  IMG_OFF_PATH		"images/hall/settings/settings_off.png"

WindowSetting::WindowSetting()
{

}

WindowSetting::~WindowSetting()
{

}

void WindowSetting::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZSetting.csb");
	CCASSERT(pMainNode, "WindowSetting::setContent() layout/JZSetting.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	
	m_pEffectOnOff = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_effect_state"));
	m_pSoundOnOff = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "img_music_state"));
	m_pEffectOnOff->addClickEventListener(CC_CALLBACK_1(WindowSetting::_onEffectOnOffClick,this));
	m_pSoundOnOff->addClickEventListener(CC_CALLBACK_1(WindowSetting::_onSoundOnOffClick, this));

	m_pEffectOnOff->loadTexture(SoundManager::shared()->getEffectOn() ? IMG_ON_PATH : IMG_OFF_PATH, TextureResType::PLIST);
	m_pSoundOnOff->loadTexture(SoundManager::shared()->getSoundOn() ? IMG_ON_PATH : IMG_OFF_PATH, TextureResType::PLIST);

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}

void WindowSetting::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowSetting::_onEffectOnOffClick(Ref* pRef)
{
	bool bOn = SoundManager::shared()->getEffectOn();
	SoundManager::shared()->setEffectOn(!bOn);

	m_pEffectOnOff->loadTexture(!bOn ? IMG_ON_PATH : IMG_OFF_PATH, TextureResType::PLIST);

	playButtonClickSound();
}

void WindowSetting::_onSoundOnOffClick(Ref* pRef)
{
	bool bOn = SoundManager::shared()->getSoundOn();
	SoundManager::shared()->setSoundOn(!bOn);

	m_pSoundOnOff->loadTexture(!bOn ? IMG_ON_PATH : IMG_OFF_PATH, TextureResType::PLIST);

	playButtonClickSound();
}