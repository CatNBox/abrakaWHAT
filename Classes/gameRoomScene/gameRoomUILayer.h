#pragma once
#include "cocos2d.h"

class gameRoomUILayer : public cocos2d::Layer
{
public:
	virtual bool init();

	CREATE_FUNC(gameRoomUILayer);
private:
	void checkMagic(const int magicStoneNumber);
	void returnMainMenu();
	void initUI();
	void setRound();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	cocos2d::EventListenerKeyboard* keyListener;
	cocos2d::EventListenerCustom* callBackListener;
	cocos2d::Menu* magicChoicer; //eash stone's front sprite and pass sprite
	cocos2d::Menu* etcUI; //exit
	
	int roundNum = 1;
	cocos2d::Sprite* roundSpr;
	cocos2d::Object* roundObj;
};