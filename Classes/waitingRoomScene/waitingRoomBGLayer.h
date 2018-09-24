#pragma once
#include "cocos2d.h"

class waitingRoomBGLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(waitingRoomBGLayer);
private:
	cocos2d::Sprite* roomBgSpr;
};