#include "mainMenuScene\mainMenuScene.h"
#include "mainMenuScene\mainMenuBGLayer.h"
#include "mainMenuScene\mainMenuObjLayer.h"

using namespace cocos2d;

cocos2d::Scene * mainMenuScene::createScene()
{
	auto scene = mainMenuScene::create();
	auto BGLayer = mainMenuBGLayer::create();
	auto ObjLayer = mainMenuObjLayer::create();

	scene->addChild(BGLayer, 0);
	scene->addChild(ObjLayer, 1);

	return scene;
}

bool mainMenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	return true;
}
