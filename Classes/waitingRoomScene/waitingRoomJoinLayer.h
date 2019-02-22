#pragma once
#include <cocos2d.h>
#include "gameObject\gameMetaData.h"

class spriteManager;
class actionManager;
class networkManager;
class waitingRoomUILayer;

class waitingRoomJoinLayer : public cocos2d::Layer
{
public:
	virtual bool init(gameMetaData::gameMode modeFlag);
	static waitingRoomJoinLayer* createWithParam(gameMetaData::gameMode modeFlag);
	void update(float dTime);

private:
	void initUI();
	void ipAddrDigitUp(const int sprIndex);
	void ipAddrDigitDown(const int sprIndex);
	void displayIpAddrFromClip();
	void updateIpAddrDisplay();
	void convertIpAddrFormat();
	void join2IPAddr();

	void callWrongIpPopup();

	void updateNetworkInfo();
	void initTimeout(float time);
	float _timeout;
	
	void callbackJoinSuccess();
	
	//IP
	std::string ipAddrSprFormat;
	std::string ipAddrNetFormat;

	cocos2d::Sprite* ipAddrSpr[12];
	cocos2d::Menu* btnMenu;

	gameMetaData::gameMode curGameMode;

	spriteManager* sprManager;
	actionManager* actManager;
	networkManager* netManager;
};