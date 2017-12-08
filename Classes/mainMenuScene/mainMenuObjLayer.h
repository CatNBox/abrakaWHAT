#pragma once
#include "cocos2d.h"

class mainMenuObjLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(mainMenuObjLayer);
private:
	void exitBtnCallback();
	void hostGameBtnCallback();
	void joinGameBtnCallback();
	void singleGameBtnCallback();
	void openBuyURL();
};