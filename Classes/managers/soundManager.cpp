#include "managers\soundManager.h"
#include "managers\gameFlowManager.h"
#include "gameObject\gameMetaData.h"
#include <SimpleAudioEngine.h>

soundManager::soundManager()
{
	preLoadSound();
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
	switch (sfxEnum)
	{
	case gameMetaData::sfxName::msMoving00:
	{
		CocosDenshion::SimpleAudioEngine::getInstance()
			->playEffect(gameMetaData::arrSfxPath.at(sfxEnum), false, 0.3f, 0.0f, 0.7f);
		return;
	}
	default:
	{
		return;
	}
	}
}

void soundManager::playNpcSound()
{
	if (prevNpcSoundID > 0)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()
			->stopEffect(prevNpcSoundID);
	}
	int rInt = gameFlowManager::getInstance()->getRandomInt(0, 4);
	prevNpcSoundID = CocosDenshion::SimpleAudioEngine::getInstance()
		->playEffect(gameMetaData::arrNpcSound.at(rInt));
}
