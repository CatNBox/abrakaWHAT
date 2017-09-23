#include "managers\gameFlowManager.h"
#include "mainMenuScene\mainMenuScene.h"
#include "gameRoomScene\gameRoomScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

static gameFlowManager *theGameFlowManager = nullptr;

gameFlowManager * gameFlowManager::getInstance()
{
	if (!theGameFlowManager)
	{
		theGameFlowManager = new gameFlowManager();
	}

	return theGameFlowManager;
}

gameFlowManager::gameFlowManager()
{
	curSceneState = gameMetaData::curScene::setupScene;
}

void gameFlowManager::changeScene2MainMenu()
{
	log("### gameFlowManager::changeScene2MainMenu in");
	auto mainMenuScene = TransitionFade::create(2.0f, mainMenuScene::createScene());
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
	setPostionCount(8);

	auto singleModeScene = TransitionSlideInT::create(1.2f, gameRoomScene::createScene());
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
	//TextureCache::addImageAsync
	//SpriteFrame::
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

void gameFlowManager::setPostionCount(int cnt)
{
	postionCnt = cnt;
}

int gameFlowManager::getPostionCount() const
{
	return postionCnt;
}

void gameFlowManager::setSecretCount(int cnt)
{
	secretCnt = cnt;
}

int gameFlowManager::getSecretCount() const
{
	return secretCnt;
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
