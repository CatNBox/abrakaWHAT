#include "gameRoomScene.h"
#include "gameRoomScene\gameRoomBGLayer.h"
#include "gameRoomScene\gameRoomObjLayer.h"
#include "gameRoomScene\gameRoomUILayer.h"

cocos2d::Scene * gameRoomScene::createScene()
{
	auto scene = gameRoomScene::create();

	auto layerBG = gameRoomBGLayer::create();
	auto layerObj = gameRoomObjLayer::create();
	auto layerUI = gameRoomUILayer::create();

	scene->addChild(layerBG, 0);
	scene->addChild(layerObj, 2);
	scene->addChild(layerUI, 1);

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
