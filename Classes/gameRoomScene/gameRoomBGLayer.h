#pragma once
#include "cocos2d.h"

class gameRoomBGLayer : public cocos2d::Layer
{
public:
	virtual bool init();

	CREATE_FUNC(gameRoomBGLayer);
private:
	cocos2d::Sprite* roomBGSprite;
};