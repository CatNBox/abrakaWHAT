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
	void initSprCntNum();
	cocos2d::Sequence* cntNumAction(int cntStartNum);
	void setEndGame(cocos2d::EventCustom* checkOwnedMagicEvent);
	void setEndRound(cocos2d::EventCustom* checkOwnedMagicEvent);
	void setWarning(cocos2d::EventCustom* checkOwnedMagicEvent);

	//void settingPopupBoard

	cocos2d::Sprite* sprCntNum;

	cocos2d::EventListenerCustom* eventListener;

	cocos2d::Menu* btnMenu;
	cocos2d::MenuItemImage* btnOk;
	cocos2d::MenuItemImage* btnCancle;
	//cocos2d::MenuItemImage btnOk;
};

