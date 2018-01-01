#pragma once
#include "cocos2d.h"

class popupLayer : public cocos2d::Layer
{
public:
	popupLayer();
	~popupLayer();
	virtual bool init();

	CREATE_FUNC(popupLayer);

private:
	void setBGSpr();
	void setEndGame(cocos2d::EventCustom* checkOwnedMagicEvent);
	void setEndRound(cocos2d::EventCustom* checkOwnedMagicEvent);
	void setWarning(cocos2d::EventCustom* checkOwnedMagicEvent);

	cocos2d::EventListenerCustom* eventListener;
};

