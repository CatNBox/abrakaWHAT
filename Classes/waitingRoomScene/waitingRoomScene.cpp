#include "waitingRoomScene.h"
#include "waitingRoomBGLayer.h"
#include "waitingRoomUILayer.h"
#include "waitingRoomJoinLayer.h"
#include "popupLayer\popupLayer.h"
#include "gameObject\gameMetaData.h"
#include "commonUILayer\commonUILayer.h"
#include "managers/networkManager.h"
#include <iostream>

cocos2d::Scene * waitingRoomScene::createScene(gameMetaData::gameMode modeFlag)
{
	auto scene = waitingRoomScene::create();
	
	auto layerBG = waitingRoomBGLayer::create();
	auto layerObj = waitingRoomUILayer::createWithParam(modeFlag);
	auto layerPopup = popupLayer::create();
	auto layerCommonUI = commonUILayer::create();

	scene->addChild(layerBG, gameMetaData::layerZOrder::backGroundZ);
	scene->addChild(layerObj, gameMetaData::layerZOrder::objZ0);
	if (modeFlag == gameMetaData::gameMode::guest)
	{
		auto layerJoin = waitingRoomJoinLayer::createWithParam(modeFlag);
		layerObj->setVisible(false);
		scene->addChild(layerJoin, gameMetaData::layerZOrder::objZ1);
	}
	scene->addChild(layerCommonUI, gameMetaData::layerZOrder::objZ1);
	scene->addChild(layerPopup,gameMetaData::layerZOrder::objZ2);

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
