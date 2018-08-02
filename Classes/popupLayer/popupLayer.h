#pragma once
#include "cocos2d.h"
#include <array>

class spriteManager;

namespace gameMetaData{
	enum class popupBoardSize;
	enum class warningCode;
};

class popupLayer : public cocos2d::Layer
{
public:
	popupLayer();
	~popupLayer();
	virtual bool init();

	CREATE_FUNC(popupLayer);

private:
	void callbackPopupClose();
	void callbackGameExit();
	void setBGSpr();
	void setPopupBoard(enum class gameMetaData::popupBoardSize popupSizeEnum);
	void setDisplayPlayer();
	void initSprCntdwnNum();
	cocos2d::Sequence* cntNumAction(int cntStartNum);
	void setEndGame(cocos2d::EventCustom* endGameEvent);
	void setEndRound(cocos2d::EventCustom* endRoundEvent);
	void setWarning(cocos2d::EventCustom* warningEvent);

	void setWarningLabel(enum class gameMetaData::warningCode warningCodeEnum);

	spriteManager* sprManager;

	cocos2d::Sprite* sprCntdwnNum;

	cocos2d::EventListenerCustom* eventListener;

	cocos2d::Menu* btnMenu;
	cocos2d::MenuItemImage* btnOk;
	cocos2d::MenuItemImage* btnCancle;

	std::vector<std::pair<cocos2d::Sprite*, cocos2d::Sprite*>> playerSpr;
	int playerCnt = 4;
	int roundWinnerIdx = 4;

	float playerSprStdYaxis;
	float popupNumStdScale;
	//cocos2d::MenuItemImage btnOk;
};

