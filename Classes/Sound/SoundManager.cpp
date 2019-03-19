#include "SoundManager.h"
#include "Util/GameUtil.h"
#include "SimpleAudioEngine.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

#define EFFECT_FIELD_NAME		"GameEffectOn"
#define SOUND_FIELD_NAME		"GameSoundOn"

static SoundManager* _sharedSoundManager = nullptr;
SoundManager* SoundManager::shared()
{
	if(_sharedSoundManager == 0){
		_sharedSoundManager = new SoundManager();
		_sharedSoundManager->init();
	}
	return _sharedSoundManager;
}

void SoundManager::purge()
{
	CC_SAFE_RELEASE_NULL(_sharedSoundManager);
}

SoundManager::SoundManager()
: m_musicName("")
{
}

SoundManager::~SoundManager()
{
}

bool SoundManager::init()
{
	return true;
}

void SoundManager::initShuiHu()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	dict_music = CCDictionary::createWithContentsOfFile("shuihu/music_android.plist");
	CC_SAFE_RETAIN(dict_music);
#else 
	dict_music = CCDictionary::createWithContentsOfFile("shuihu/music.plist");
	CC_SAFE_RETAIN(dict_music);
#endif
	

	CCArray* key1 = dict_music->allKeys();

	for (int i = 0; i < key1->count(); i++){

		CCString* ckey = (CCString*)key1->objectAtIndex(i);
		CCString* cVal = (CCString*)dict_music->objectForKey(ckey->getCString());

		preloadMusic(cVal->getCString());

	}
	
	
}

void SoundManager::initBuYu()
{
 	////sounds
 //	m_SoundMap.clear();
 //#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
 //	m_SoundMap = FileUtils::getInstance()->getValueMapFromFile("buyu/sounds_ios.plist");
 //#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
 //	m_SoundMap = FileUtils::getInstance()->getValueMapFromFile("buyu/sounds_android.plist");
 //#else
 //	m_SoundMap = FileUtils::getInstance()->getValueMapFromFile("buyu/sounds.plist");
 //#endif
 //
 //	////music
 //	m_MusicMap.clear();
 //#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
 //	m_MusicMap = FileUtils::getInstance()->getValueMapFromFile("music_ios.plist");
 //#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
 //	m_MusicMap = FileUtils::getInstance()->getValueMapFromFile("music_android.plist");
 //#else
 //	m_MusicMap = FileUtils::getInstance()->getValueMapFromFile("music.plist");
 //#endif
 //
 //	if (m_MusicMap.empty())
 //		return;
 //
 //	AUDIO_PLAY("room", AUDIO_TYPE::MUSIC_TYPE);
}

void SoundManager::setEffectOn(bool bOn)
{
	GameUtil::setData(EFFECT_FIELD_NAME, bOn ? 1 : 0);

	if (!bOn)
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}
}

bool SoundManager::getEffectOn()
{
	return GameUtil::getData(EFFECT_FIELD_NAME, 1) != 0;
}

void SoundManager::setSoundOn(bool bOn)
{
	GameUtil::setData(SOUND_FIELD_NAME, bOn ? 1 : 0);

	log("-----------setSoundOn[%d]-----------", bOn);

	if (!bOn)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	else
	{
		if (!m_strLastMusic.empty())
		{
			playMusicNormal(m_strLastMusic.c_str(), true);
		}
	}
}

bool SoundManager::getSoundOn()
{
	return GameUtil::getData(SOUND_FIELD_NAME, 1) != 0;
}

void SoundManager::playEffectNormal(const char* sound)
{
	if (!getEffectOn())
	{
		return;
	}

	SimpleAudioEngine::sharedEngine()->playEffect(sound, false);
}

void SoundManager::playMusicNormal(const char* music, bool bLoop)
{
 	log("-----------playMusicNormal[%s,%d]-----------", music, bLoop);

	m_strLastMusic = music;

	if (!getSoundOn())
	{
		return;
	}

	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music, bLoop);
}

void SoundManager::stopMusic()
{
	log("-----------stopMusic-----------");
	m_strLastMusic.clear();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundManager::stopEffect()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

void SoundManager::stopAll()
{
	stopMusic();
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

std::string SoundManager::getHallMusicFileFullPath(const std::string& strPath)
{
	std::string strRetPath = strPath;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	strRetPath = "music/android/hall/" + strRetPath + ".ogg";
#else
	strRetPath = "music/ios/hall/" + strRetPath + ".mp3";
#endif

	return strRetPath;
}

void SoundManager::playSound(const char* sound, bool bLoop)
{
	if (!getEffectOn())
	{
		return;
	}

	const char* sound_file = SoundFromKey(sound);

	if (sound_file != 0 && sound_file[0] != 0)
	{
		//PLAZZ_PRINTF("play sound:%s", sound);
		if (bLoop)
		{
			stopSound(sound);

		}
		//SimpleAudioEngine::sharedEngine()->preloadEffect(sound_file);
		unsigned int effect = SimpleAudioEngine::sharedEngine()->playEffect(sound_file, bLoop);
		if (bLoop)
		{
			_dictSound[sound] = effect;
		}
	}
}

void SoundManager::stopSound(const char* sound)
{
	const char* sound_file = SoundFromKey(sound);

	if (sound_file != 0 && sound_file[0] != 0)
	{
		std::map<std::string, unsigned int>::iterator it = _dictSound.find(sound);
		if (it != _dictSound.end())
		{
			SimpleAudioEngine::sharedEngine()->stopEffect(it->second);
			_dictSound.erase(it);
		}
	}
}

void SoundManager::playMusic(const char* music)
{
	if (!getSoundOn())
	{
		return;
	}

	const char* sound_file = stringFromKey(music);

	if (sound_file != 0 && sound_file[0] != 0)
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
		//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(sound_file);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sound_file, true);
	}
}

//预加载水浒传音乐
void SoundManager::preloadMusic(const char* name)
{
	const char* sound_file = stringFromKey(name);
	if (sound_file != 0 && sound_file[0] != 0)
	{
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(sound_file);
	}
}
//预加载水浒传音效
void SoundManager::preloadEffect()
{

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	dict_sound = CCDictionary::createWithContentsOfFile("shuihu/sounds_android.plist");
	CC_SAFE_RETAIN(dict_sound);
#else 
	dict_sound = CCDictionary::createWithContentsOfFile("shuihu/sounds.plist");
	CC_SAFE_RETAIN(dict_sound);
#endif	

	CCArray* key = dict_sound->allKeys();
	const char* sound_file;
	for (int i = 0; i < key->count(); i++){

		CCString* ckey = (CCString*)key->objectAtIndex(i);
		CCString* cVal = (CCString*)dict_sound->objectForKey(ckey->getCString());
		//sound_file = ckey->getCString();
		sound_file = cVal->getCString();
		if (sound_file != 0 && sound_file[0] != 0)
		{
			SimpleAudioEngine::sharedEngine()->preloadEffect(sound_file);
		}

	}
	
	
}

void SoundManager::play(const std::string& key, const AUDIO_TYPE& soundType)
{
	bool bEnable = soundType == AUDIO_TYPE::EFFECT_TYPE ? getEffectOn() : getSoundOn();

	if (!bEnable)
	{
		return;
	}

    if(key.empty())
        return;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return;
#endif
    
    if(soundType == AUDIO_TYPE::EFFECT_TYPE)
    {
        unsigned int soundId = SimpleAudioEngine::getInstance()->playEffect(m_SoundMap[key].asString().c_str());
		//cocos2d::log("******************************** sound id %u(key %s, name %s) played\n", soundId, key.c_str(), m_SoundMap[key].asString().c_str());
    }
    else if(soundType == AUDIO_TYPE::MUSIC_TYPE)
    {
		if (m_musicName == m_MusicMap[key].asString())
		{
			return;
		}
		m_musicName = m_MusicMap[key].asString();
		stopMusic();
        SimpleAudioEngine::getInstance()->playBackgroundMusic(m_musicName.c_str(), true);
    }
}

void SoundManager::stopSound(const std::string& sound)
{
    if(sound.empty())
        return;
}

float SoundManager::getMusicVolume() const
{
	return mMusicVolume;
}

float SoundManager::getSoundVolume() const
{
	return mSoundVolume;
}

void SoundManager::setMusicVolume(float volume)
{
    mMusicVolume = volume;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(mMusicVolume);
}

void SoundManager::setSoundVolume(float volume)
{
    mSoundVolume = volume;
    SimpleAudioEngine::getInstance()->setEffectsVolume(mSoundVolume);
}

void SoundManager::saveSettings()
{
	UserDefault::getInstance()->setFloatForKey("music", mMusicVolume);
	UserDefault::getInstance()->setFloatForKey("sound", mSoundVolume);
}

void SoundManager::pause()
{
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundManager::resume()
{
    SimpleAudioEngine::getInstance()->resumeAllEffects();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

const char*	SoundManager::stringFromKey(const std::string& key)
{
	const CCString* str = dict_music->valueForKey(key);

	return str == 0 ? 0 : str->getCString();
}
const char*	SoundManager::SoundFromKey(const std::string& key)
{
	const CCString* str = dict_sound->valueForKey(key);

	return str == 0 ? 0 : str->getCString();
}

