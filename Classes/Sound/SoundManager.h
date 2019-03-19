#ifndef _SoundManager_H_
#define _SoundManager_H_

#include "cocos2d.h"
USING_NS_CC;

enum class AUDIO_TYPE
{
    EFFECT_TYPE = 0,
    MUSIC_TYPE,
};
class SoundManager : public Ref
{
public:
	static SoundManager* shared();
	static void purge();

CC_CONSTRUCTOR_ACCESS:
    SoundManager();
    ~SoundManager();
    bool init();
    
public:
	void initShuiHu();
	void initBuYu();

	void setEffectOn(bool bOn);
	bool getEffectOn();
	void setSoundOn(bool bOn);
	bool getSoundOn();

	void playEffectNormal(const char* sound);
	void playMusicNormal(const char* music, bool bLoop);

	void stopMusic();
	void stopEffect();
	void stopAll();

	std::string getHallMusicFileFullPath(const std::string& strPath);

	/*----------以下为水浒传方法----------*/
	void playSound(const char* sound, bool bLoop = false);
	void stopSound(const char* sound);
	void playMusic(const char* music);
	void preloadEffect();
	void preloadMusic(const char* name);

	/*----------以下为捕鱼方法----------*/
    void play(const std::string& key, const AUDIO_TYPE& soundType);
	
    void stopSound(const std::string& sound);
	

	float getMusicVolume() const;
	float getSoundVolume() const;
	void  setMusicVolume(float volume);
	void  setSoundVolume(float volume);

	void saveSettings();
	void pause();
	void resume();


protected:
	const char*	stringFromKey(const std::string& key);
	const char* SoundFromKey(const std::string& key);

private:
	/*---------------下面是捕鱼的数据-------------*/
	std::string			m_musicName;
	float               mMusicVolume;
	float               mSoundVolume;
    
    ValueMap            m_SoundMap;
    ValueMap            m_MusicMap;

	/*---------------下面是水浒传的数据--------------*/
	cocos2d::Dictionary*   dict_sound;
	cocos2d::Dictionary*   dict_music;
	std::map<std::string, unsigned int> _dictSound;
	std::map<std::string, unsigned int> _dictMusic;

	std::string m_strLastMusic;

};

#define AUDIO_PLAY(key, type)           SoundManager::shared()->play(key, type)
#define AUDIO_PAUSE()                           SoundManager::shared()->pause()

typedef SoundManager SHZSoundManager;

#endif
