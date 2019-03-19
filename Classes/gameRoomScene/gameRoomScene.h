#pragma once
#include "cocos2d.h"

namespace gameMetaData
{
	enum class gameMode;
}

class gameRoomScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(gameMetaData::gameMode modeFlag, int playerTurnOrder[]);
	virtual bool init();

	CREATE_FUNC(gameRoomScene);
private:
};