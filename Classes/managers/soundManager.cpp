#include "managers\soundManager.h"
#include <SimpleAudioEngine.h>

soundManager::soundManager()
{
}

soundManager::~soundManager()
{
}

void soundManager::preLoadSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/msMoving00.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/buljak00.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/yongBreath00.flac");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/byTheLight.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/goodgame.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/greetings.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/theLightShallBringVictory.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/theLightShallBurnYou.ogg");
}

void soundManager::playSfx(const int sfxEnum)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("soundResource/msMoving00.wav", false, 0.3f, 0.0f, 0.7f);
}
