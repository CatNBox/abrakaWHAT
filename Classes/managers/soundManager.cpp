#include "managers\soundManager.h"
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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/activateMagic00.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/anduinEng/byTheLight.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/anduinEng/goodgame.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/anduinEng/greetings.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/anduinEng/theLightShallBringVictory.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("soundResource/tempSound/anduinEng/theLightShallBurnYou.wav");
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
	case gameMetaData::sfxName::activateMagic00:
	{
		CocosDenshion::SimpleAudioEngine::getInstance()
			->playEffect(gameMetaData::arrSfxPath.at(sfxEnum), false);
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
	int rInt = inlineFunc::getRandomInt(0, 15);
	prevNpcSoundID = CocosDenshion::SimpleAudioEngine::getInstance()
		->playEffect(gameMetaData::arrNpcSound.at(rInt));
}
