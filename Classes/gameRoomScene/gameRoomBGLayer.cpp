#include "gameRoomBGLayer.h"

using namespace cocos2d;

bool gameRoomBGLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	roomBGSprite = Sprite::create("bg_wait.jpg");
	if (roomBGSprite != nullptr)
	{
		roomBGSprite->setAnchorPoint(Vec2::ZERO);
		roomBGSprite->setPosition(Vec2::ZERO);
		this->addChild(roomBGSprite);
	}

	return true;
}
