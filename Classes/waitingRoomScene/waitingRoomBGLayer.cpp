#include "waitingRoomBGLayer.h"
#include "gameObject\gameMetaData.h"

bool waitingRoomBGLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	roomBgSpr = cocos2d::Sprite::create("bg_wait.jpg");
	if (roomBgSpr == nullptr)
	{
		return false;
	}

	roomBgSpr->setAnchorPoint(cocos2d::Vec2::ZERO);
	roomBgSpr->setPosition(cocos2d::Vec2::ZERO);
	this->addChild(roomBgSpr, gameMetaData::layerZOrder::backGroundZ);
	
	return true;
}
