#pragma once
#include "cocos2d.h"

class setupLodingLayer : public cocos2d::Layer
{
public:

	virtual bool init();

	CREATE_FUNC(setupLodingLayer);

private:
	void menuCloseCallback(cocos2d::Ref * pSender);
	void callChangeScene2MainMenu(float d);


};