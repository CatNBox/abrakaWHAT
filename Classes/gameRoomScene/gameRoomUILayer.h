#pragma once
#include "cocos2d.h"

class spriteManager;
class actionManager;

class gameRoomUILayer : public cocos2d::Layer
{
public:
	virtual bool init();

	CREATE_FUNC(gameRoomUILayer);
private:
	void settingEventListener();
	void setEnabledInputUI(const bool flag4KeyboardNselectUI, const bool flag4PassBtn);
	void checkMagic(const int magicStoneNumber);
	void returnMainmenu();
	void initUI();
	void setRound();
	bool checkRunningAction();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	cocos2d::EventListenerKeyboard* keyListener;
	cocos2d::EventListenerCustom* callBackListener;
	cocos2d::Menu* magicChoicer; //eash stone's front sprite and pass sprite
	
	spriteManager* sprManager;
	actionManager* actManager;
	
	int roundNum = 0;
	int lastChooseMs = 0;
	cocos2d::Sprite* roundSpr;
	cocos2d::Object* roundObj;

	std::vector<cocos2d::MenuItemImage*> arrBtnSelectStone;
};