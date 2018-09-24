#pragma once
#include "cocos2d.h"

namespace gameMetaData
{
	enum class gameMode;
}

class waitingRoomScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(gameMetaData::gameMode modeFlag);
	virtual bool init();

	CREATE_FUNC(waitingRoomScene);
private:
};