#pragma once
#include "cocos2d.h"
#include "gameObject\gameMetaData.h"

class spriteManager;
class actionManager;

class waitingRoomUILayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(waitingRoomUILayer);

	void setGameMode(gameMetaData::gameMode modeFlag);

private:
	void settingEventListener();
	void initUI();

	void initPlayerLabel();
	void addPlayerLabel();

	void returnMainmenuCallback();
	void startGameCallback();
	void orderingCallback();

	void setBtnDisabled();
	void setCpuSpr();
	void runGameScene();

	gameMetaData::gameMode curMode;
	spriteManager* sprManager;
	actionManager* actManager;

	int playerCnt;
	cocos2d::Menu* btnMenu;
	std::vector<cocos2d::Sprite*> playerList;
	int playerOrder[gameMetaData::defaultPlayerCnt];

	cocos2d::EventListenerCustom callBackListener;
};
