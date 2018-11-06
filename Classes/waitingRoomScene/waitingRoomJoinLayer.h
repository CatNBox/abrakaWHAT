#pragma once
#include <cocos2d.h>
#include "gameObject\gameMetaData.h"

class spriteManager;
class actionManager;

class waitingRoomJoinLayer : public cocos2d::Layer
{
public:
	virtual bool init(gameMetaData::gameMode modeFlag);
	static waitingRoomJoinLayer* createWithParam(gameMetaData::gameMode modeFlag);

private:
	void initUI();
	void ipAddrDigitUp(const int sprIndex);
	void ipAddrDigitDown(const int sprIndex);
	void displayIpAddrFromClip();

	//IP
	std::string ipAddr;
	cocos2d::Sprite* ipAddrSpr[12];

	gameMetaData::gameMode curMode;

	spriteManager* sprManager;
	actionManager* actManager;
};