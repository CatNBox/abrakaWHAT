#include "waitingRoomScene.h"
#include "waitingRoomBGLayer.h"
#include "waitingRoomUILayer.h"
#include "popupLayer\popupLayer.h"
#include "gameObject\gameMetaData.h"

cocos2d::Scene * waitingRoomScene::createScene(gameMetaData::gameMode modeFlag)
{
	auto scene = waitingRoomScene::create();
	
	auto layerBG = waitingRoomBGLayer::create();
	auto layerObj = waitingRoomUILayer::create();
	layerObj->setGameMode(modeFlag);
	auto layerPopup = popupLayer::create();

	scene->addChild(layerBG);
	scene->addChild(layerObj);
	scene->addChild(layerPopup);
	
	return scene;
}

bool waitingRoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	return true;
}
