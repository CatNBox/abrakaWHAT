#pragma once
#include <cocos2d.h>

class spriteManager;

class commonUILayer : public cocos2d::Layer
{
public:
	virtual bool init();

	CREATE_FUNC(commonUILayer);
private:
	//btnExit
	void setExitBtn2MainMenu();
	void returnMainmenuCallback();

	spriteManager* sprManager;
	cocos2d::Menu* commonUIMenu;
};