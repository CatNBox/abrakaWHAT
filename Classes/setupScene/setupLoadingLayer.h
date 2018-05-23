#pragma once
#include "cocos2d.h"

class spriteManager;

class setupLodingLayer : public cocos2d::Layer
{
public:

	virtual bool init();

	CREATE_FUNC(setupLodingLayer);

private:
	void menuCloseCallback(cocos2d::Ref * pSender);
	void changeScene2MainMenu(float d);
	bool checkExistINI();
	void setINI();

	spriteManager* sprManager;
};