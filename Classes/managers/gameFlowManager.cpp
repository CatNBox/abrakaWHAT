#include "managers\gameFlowManager.h"
#include "gameObject\gameMetaData.h"
#include "mainMenuScene\mainMenuScene.h"
#include "gameRoomScene\gameRoomScene.h"
#include <random>
#include <cstdarg>

using namespace cocos2d;

gameFlowManager & gameFlowManager::getInstance()
{
	static gameFlowManager instance;
	return instance;
}

gameFlowManager::gameFlowManager()
{
	curSceneState = gameMetaData::curScene::setupScene;
	objSoundManager = new soundManager();
}

gameFlowManager::~gameFlowManager()
{
}

void gameFlowManager::changeScene2MainMenu()
{
	log("### gameFlowManager::changeScene2MainMenu in");
	auto mainMenuScene = TransitionFade::create(1.0f, mainMenuScene::createScene());
	Director::getInstance()->replaceScene(mainMenuScene);
	curSceneState = gameMetaData::curScene::mainMenuScene;
}

void gameFlowManager::changeScene2SingleMode()
{
	log("### gameFlowManager::changeScene2SingleMode in");

	//temporary count number. change later
	setPlayerCount(4); 
	setSecretCount(4);
	setYongyongCount(1);
	setBangrangCount(2);
	setWindCount(3);
	setBooungCount(4);
	setBunpokCount(5);
	setNungangCount(6);
	setBuljakCount(7);
	setPotionCount(8);
	setLifePoint(6);
	this->runningActionCnt = 0;

	auto singleModeScene = TransitionSlideInT::create(0.6f, gameRoomScene::createScene());
	Director::getInstance()->replaceScene(singleModeScene);
	curSceneState = gameMetaData::curScene::gameRoom;
}

void gameFlowManager::changeScene2HostMode()
{
}

void gameFlowManager::changeScene2JoinMode()
{
}

void gameFlowManager::preloadSprites()
{
	log("### preload sprite ###");
	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("magicStones.plist","magicStones.png");
	spriteCache->addSpriteFramesWithFile("seenChecker.plist", "seenChecker.png");
	
	auto tempSpr = Sprite::create("UISprite/spr_number.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "spr_number.png");

	tempSpr->initWithFile("lpToken/lpToken00.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken00.png");
	tempSpr->initWithFile("lpToken/lpToken01.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken01.png");
	tempSpr->initWithFile("lpToken/lpToken02.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken02.png");
	tempSpr->initWithFile("lpToken/lpToken03.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken03.png");
	tempSpr->initWithFile("lpToken/lpToken04.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken04.png");
	tempSpr->initWithFile("lpToken/lpToken05.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken05.png");

	tempSpr->initWithFile("msEffectSpr/yongyongRedScreen.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "yongyongRedScreen.png");

	tempSpr->initWithFile("UISprite/sprScore.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprScore.png");
	tempSpr->initWithFile("UISprite/sprPlayer.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprPlayer.png");

	tempSpr->initWithFile("popupLayerSpr/popupLayerBG.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popupLayerBG.png");
	tempSpr->initWithFile("popupLayerSpr/popup400500.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popup400500.png");
	tempSpr->initWithFile("popupLayerSpr/popup400245.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popup400245.png");
	tempSpr->initWithFile("UISprite/uiPopMessage.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "uiPopMessage.png");

	tempSpr->initWithFile("UISprite/sprCancel.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprCancel.png");
	tempSpr->initWithFile("UISprite/sprOk.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprOk.png");
	tempSpr->initWithFile("UISprite/sprLive.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprLive.png");
	tempSpr->initWithFile("UISprite/sprWin.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprWin.png");
	tempSpr->initWithFile("UISprite/sprBooung.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprBooung.png");
}

//Rect만 넘겨주도록 수정
cocos2d::Rect gameFlowManager::getNumSprRect(const int num)
{
	cocos2d::Rect tempRect;
	switch (num)
	{
	case 1:
		tempRect = cocos2d::Rect(11, 5, 34, 75);
		return tempRect;
	case 2:
		tempRect = cocos2d::Rect(51, 5, 47, 81);
		return tempRect;
	case 3:
		tempRect = cocos2d::Rect(100, 5, 45, 81);
		return tempRect;
	case 4:
		tempRect = cocos2d::Rect(147, 7, 46, 81);
		return tempRect;
	case 5:
		tempRect = cocos2d::Rect(194, 5, 45, 81);
		return tempRect;
	case 6:
		tempRect = cocos2d::Rect(242, 5, 43, 81);
		return tempRect;
	case 7:
		tempRect = cocos2d::Rect(289, 5, 45, 81);
		return tempRect;
	case 8:
		tempRect = cocos2d::Rect(336, 5, 45, 81);
		return tempRect;
	case 9:
		tempRect = cocos2d::Rect(381, 5, 47, 81);
		return tempRect;
	default:
		tempRect = cocos2d::Rect(430, 5, 46, 81);
		return tempRect;
	}
	return tempRect;
}

void gameFlowManager::setPlayerCount(int cnt)
{
	playerCount = cnt;
}

int gameFlowManager::getPlayerCount() const
{
	return playerCount;
}

void gameFlowManager::setYongyongCount(int cnt)
{
	yongyongCnt = cnt;
}

int gameFlowManager::getYongyongCount() const
{
	return yongyongCnt;
}

void gameFlowManager::setBangrangCount(int cnt)
{
	bangrangCnt = cnt;
}

int gameFlowManager::getBangrangCount() const
{
	return bangrangCnt;
}

void gameFlowManager::setWindCount(int cnt)
{
	windCnt = cnt;
}

int gameFlowManager::getWindCount() const
{
	return windCnt;
}

void gameFlowManager::setBooungCount(int cnt)
{
	booungCnt = cnt;
}

int gameFlowManager::getBooungCount() const
{
	return booungCnt;
}

void gameFlowManager::setBunpokCount(int cnt)
{
	bunpokCnt = cnt;
}

int gameFlowManager::getBunpokCount() const
{
	return bunpokCnt;
}

void gameFlowManager::setNungangCount(int cnt)
{
	nungangCnt = cnt;
}

int gameFlowManager::getNungangCount() const
{
	return nungangCnt;
}

void gameFlowManager::setBuljakCount(int cnt)
{
	buljakCnt = cnt;
}

int gameFlowManager::getBuljakCount() const
{
	return buljakCnt;
}

void gameFlowManager::setPotionCount(int cnt)
{
	potionCnt = cnt;
}

int gameFlowManager::getPotionCount() const
{
	return potionCnt;
}

void gameFlowManager::setSecretCount(int cnt)
{
	secretCnt = cnt;
}

int gameFlowManager::getSecretCount() const
{
	return secretCnt;
}

void gameFlowManager::setLifePoint(int cnt)
{
	maxLifePoint = cnt;
}

int gameFlowManager::getMaxLifePoint() const
{
	return maxLifePoint;
}

int gameFlowManager::getRandomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937_64 rnd(rd());

	std::uniform_int_distribution<int> range(min, max);

	return range(rnd);
}

soundManager * gameFlowManager::getSoundManager()
{
	return objSoundManager;
}

void gameFlowManager::endGame()
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void gameFlowManager::incRunningActionCnt()
{
	runningActionCnt++;
}

void gameFlowManager::decRunningActionCnt()
{
	runningActionCnt--;
}

int gameFlowManager::getRunningActionCnt()
{
	return runningActionCnt;
}

cocos2d::Sequence * gameFlowManager::wrapActions(cocos2d::FiniteTimeAction * action01, ...)
{
	va_list ap;
	va_start(ap, action01);

	cocos2d::FiniteTimeAction * now = action01;
	cocos2d::Vector<cocos2d::FiniteTimeAction*> vecAction;
	auto callNotifyActionStart = cocos2d::CallFunc::create([=]() {incRunningActionCnt(); });
	vecAction.pushBack(callNotifyActionStart);
	while (action01)
	{
		vecAction.pushBack(now);
		now = va_arg(ap, cocos2d::FiniteTimeAction*);
		if (now == NULL)
			break;
	}
	auto callNotifyActionEnd = cocos2d::CallFunc::create([=]() {decRunningActionCnt(); });
	vecAction.pushBack(callNotifyActionEnd);

	va_end(ap);

	return cocos2d::Sequence::create(vecAction);
}
