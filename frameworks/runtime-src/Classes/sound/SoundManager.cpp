#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

void threadWorker()
{
    ////sounds
    auto &soundMap = SoundManager::shared()->getSoundMap();
    soundMap.clear();
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    soundMap = FileUtils::getInstance()->getValueMapFromFile("sounds_ios.plist");
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    soundMap = FileUtils::getInstance()->getValueMapFromFile("sounds_android.plist");
//#else
//    soundMap = FileUtils::getInstance()->getValueMapFromFile("sounds.plist");
//#endif
    
	/*
    if(soundMap.empty())
        return;
    
    for(auto mapItr = soundMap.begin(); mapItr!=soundMap.end(); mapItr++)
    {
        std::string soundPath = mapItr->second.asString();
        SimpleAudioEngine::getInstance()->preloadEffect(soundPath.c_str());
    }
	*/
    
    ////music
    auto &musicMap = SoundManager::shared()->getMusicMap();
    musicMap.clear();
    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	musicMap = FileUtils::getInstance()->getValueMapFromFile("music_ios.plist");
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	musicMap = FileUtils::getInstance()->getValueMapFromFile("music_android.plist");
//#else
//	musicMap = FileUtils::getInstance()->getValueMapFromFile("music.plist");
//#endif
    
    if(musicMap.empty())
        return;
    
    for(auto mapItr = musicMap.begin(); mapItr!=musicMap.end(); mapItr++)
    {
        std::string soundPath = mapItr->second.asString();
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(soundPath.c_str());
    }
    
    AUDIO_PLAY("room", AUDIO_TYPE::MUSIC_TYPE);
}

void threadPreLoad()
{
    thread th(threadWorker);
    th.detach();
}

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
    //threadPreLoad();
	threadWorker();

    
    setMusicVolume(1);
    setSoundVolume(1);
    setMusicSwitch(UserDefault::getInstance()->getBoolForKey("music", true));
    setSoundSwitch(UserDefault::getInstance()->getBoolForKey("sound", true));
	return true;
}

void SoundManager::saveSettings()
{
	UserDefault::getInstance()->setBoolForKey("music", bMusic);
	UserDefault::getInstance()->setBoolForKey("sound", bSound);
}

void SoundManager::play(const std::string& key, const AUDIO_TYPE& soundType)
{
    if(key.empty())
        return;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return;
#endif
    
    if(soundType == AUDIO_TYPE::EFFECT_TYPE)
    {
        if (!bSound) {
            return;
        }
        unsigned int soundId = SimpleAudioEngine::getInstance()->playEffect(m_SoundMap[key].asString().c_str());
		//cocos2d::log("******************************** sound id %u(key %s, name %s) played\n", soundId, key.c_str(), m_SoundMap[key].asString().c_str());
    }
    else if(soundType == AUDIO_TYPE::MUSIC_TYPE)
    {
        if(!bMusic)
        {
            return;
        }
		if (m_musicName == m_MusicMap[key].asString())
		{
			return;
		}
		m_musicName = m_MusicMap[key].asString();
//		stopMusic();
        SimpleAudioEngine::getInstance()->playBackgroundMusic(m_musicName.c_str(), true);
    }
}

void SoundManager::stopSound(const std::string& sound)
{
    if(sound.empty())
        return;
}

void SoundManager::stopMusic()
{
    m_musicName = "";
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundManager::stopAll()
{
    stopMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

float SoundManager::getMusicVolume() const
{
	return 1;
}

float SoundManager::getSoundVolume() const
{
	return 1;
}

void SoundManager::setMusicVolume(float volume)
{
//    mMusicVolume = volume;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
}

void SoundManager::setSoundVolume(float volume)
{
//    mSoundVolume = volume;
    SimpleAudioEngine::getInstance()->setEffectsVolume(1);
}

void SoundManager::pause()
{
    if (bSound) {
        SimpleAudioEngine::getInstance()->pauseAllEffects();
    }
    if (bMusic) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void SoundManager::resume()
{
    if (bSound) {
        SimpleAudioEngine::getInstance()->resumeAllEffects();
    }
    if (bMusic) {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    
}




