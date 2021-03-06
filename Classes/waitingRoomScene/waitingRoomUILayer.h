#pragma once
#include "cocos2d.h"
#include "gameObject\gameMetaData.h"

class spriteManager;
class actionManager;
class networkManager;

class waitingRoomUILayer : public cocos2d::Layer
{
public:
	virtual bool init(gameMetaData::gameMode modeFlag);
	//CREATE_FUNC(waitingRoomUILayer);
	static waitingRoomUILayer* createWithParam(gameMetaData::gameMode modeFlag);
	void update(float dTime);

private:
	void settingEventListener();
	void initUI();

	void updateIpAddr();
	//void updatePlayerLabel(cocos2d::EventCustom* playerCntEvent);
	void updatePlayerLabel();
	void addPlayerLabel();

	void startGameCallback();
	bool setBufTurnOrder();
	void setPlayerTurnOrder();
	void showSpriteTurnOrder();

	void initIpAddrSpr();
	void errorWrongIpFormat();
	void setIpAddrSpr();
	void copy2Clipboard();

	void setBtnDisabled();
	void setCpuSpr();
	void runGameScene();

	gameMetaData::gameMode curGameMode;
	spriteManager* sprManager;
	actionManager* actManager;
	networkManager* netManager;

	std::string hostAddr;
	cocos2d::Sprite* ipAddrSpr[12];

	int playerCnt;
	int NPCCnt;
	cocos2d::Menu* btnMenu;
	std::vector<cocos2d::Sprite*> playerList;
	int playerTurnOrder[gameMetaData::defaultPlayerCnt];
	int bufTurnOrder[gameMetaData::defaultPlayerCnt];
	gameMetaData::gameProgressStage curProgressStage;

	cocos2d::EventListenerCustom* eventListener;
};
