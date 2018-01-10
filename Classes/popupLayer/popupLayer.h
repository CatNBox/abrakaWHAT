#pragma once
#include "cocos2d.h"
#include <array>

namespace gameMetaData{
	enum class popupBoardSize;
};

class popupLayer : public cocos2d::Layer
{
public:
	popupLayer();
	~popupLayer();
	virtual bool init();

	CREATE_FUNC(popupLayer);

private:
	void setBGSpr();
	void setPopupBoard(enum class gameMetaData::popupBoardSize popupSizeEnum);
	void setDisplayPlayer();
	void initSprCntNum();
	cocos2d::Sequence* cntNumAction(int cntStartNum);
	void setEndGame(cocos2d::EventCustom* checkOwnedMagicEvent);
	void setEndRound(cocos2d::EventCustom* checkOwnedMagicEvent);
	void setWarning(cocos2d::EventCustom* checkOwnedMagicEvent);

	cocos2d::Sprite* sprCntNum;

	cocos2d::EventListenerCustom* eventListener;

	cocos2d::Menu* btnMenu;
	cocos2d::MenuItemImage* btnOk;
	cocos2d::MenuItemImage* btnCancle;

	std::vector<std::pair<cocos2d::Sprite*, cocos2d::Sprite*>> playerSpr;
	//cocos2d::MenuItemImage btnOk;
};

