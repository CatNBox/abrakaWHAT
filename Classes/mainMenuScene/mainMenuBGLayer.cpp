#include "mainMenuScene\mainMenuBGLayer.h"

using namespace cocos2d;

bool mainMenuBGLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto BGSprite = Sprite::create("bg_logo.jpg");

	BGSprite->setAnchorPoint(Vec2(0, 0));
	BGSprite->setPosition(Vec2(0, 0));

	this->addChild(BGSprite);

	return true;
}
