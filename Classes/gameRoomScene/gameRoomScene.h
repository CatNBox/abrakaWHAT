#pragma once
#include "cocos2d.h"

class gameRoomScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(gameRoomScene);
private:
};