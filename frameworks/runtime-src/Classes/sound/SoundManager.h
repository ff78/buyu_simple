#ifndef _SoundManager_H_
#define _SoundManager_H_

#include "cocos2d.h"
USING_NS_CC;

//#include "plazz/df/types.h"

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
    void play(const std::string& key, const AUDIO_TYPE& soundType);
	void stopMusic();
    void stopSound(const std::string& sound);
	void stopAll();

	float getMusicVolume() const;
	float getSoundVolume() const;
	void  setMusicVolume(float volume);
	void  setSoundVolume(float volume);

	void saveSettings();
	void pause();
	void resume();
    
    ValueMap& getSoundMap(){return m_SoundMap;}
    ValueMap& getMusicMap(){return m_MusicMap;}

private:
	std::string			m_musicName;
    
    ValueMap            m_SoundMap;
    ValueMap            m_MusicMap;
	CC_SYNTHESIZE(bool, bMusic, MusicSwitch);
	CC_SYNTHESIZE(bool, bSound, SoundSwitch);
};

#define AUDIO_PLAY(key, type)           SoundManager::shared()->play(key, type)
#define AUDIO_PAUSE()                           SoundManager::shared()->pause()
#endif
