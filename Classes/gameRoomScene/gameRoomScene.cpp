#include "gameRoomScene.h"
#include "gameRoomScene\gameRoomBGLayer.h"
#include "gameRoomScene\gameRoomObjLayer.h"
#include "gameRoomScene\gameRoomUILayer.h"
#include "popupLayer\popupLayer.h"
#include "commonUILayer\commonUILayer.h"
#include "gameObject\gameMetaData.h"

cocos2d::Scene * gameRoomScene::createScene(gameMetaData::gameMode modeFlag, int playerTurnOrder[])
{
	auto scene = gameRoomScene::create();

	auto layerBG = gameRoomBGLayer::create();
	auto layerUI = gameRoomUILayer::create();
	auto layerPopup = popupLayer::create();
	auto layerCommonUI = commonUILayer::create();
	auto layerObj = gameRoomObjLayer::createWithParam(modeFlag, playerTurnOrder);

	scene->addChild(layerBG, gameMetaData::layerZOrder::backGroundZ);
	scene->addChild(layerUI, gameMetaData::layerZOrder::objZ0);
	scene->addChild(layerObj, gameMetaData::layerZOrder::objZ1);
	scene->addChild(layerCommonUI, gameMetaData::layerZOrder::objZ1);
	scene->addChild(layerPopup, gameMetaData::layerZOrder::objZ2);

	return scene;
}

bool gameRoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	return true;
}
