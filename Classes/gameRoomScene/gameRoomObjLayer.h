#pragma once
#include "cocos2d.h"
#include "gameObject/gameMetaData.h"
#include <array>

class magicStone;
class player;
class spriteManager;
class actionManager;

class gameRoomObjLayer : public cocos2d::Layer
{
public:
	virtual bool init(gameMetaData::gameMode modeFlag, int playerOrder[]);
	//CREATE_FUNC(gameRoomObjLayer);

	static gameRoomObjLayer* createWithParam(gameMetaData::gameMode modeFlag, int playerOrder[]);

	void initRound();

private:
	void delay01secCallWrapper(void (gameRoomObjLayer::*targetFunc)(void));
	void settingEventListener();
	void settingCntValues();
	void createPlayers(int playerOrder[]);
	void createMagicStones();
	void createSeenChecker();
	void createPlayerLpObj();
	void createScoreSpr();

	void startGameByNpc();

	void selSecretStone();
	void shareStone2Player();

	void checkOwnedMagic(cocos2d::EventCustom* checkOwnedMagicEvent);
	void activateMagic(const int magicEnum);
	void updateSeenChecker(const int magicEnum);
	void reorderPlayerHand();
	bool isRoundEnd();
	void calcScore();
	void callEndRoundEvent();
	void passTurn();

	cocos2d::Sprite* getMSSprite(const int magicEnum);
	void setStartOrder();

	void callNpcProcess();

	magicStone* pickAStone(const int stateEnum);
	bool isAllUsed() const; //check to discard all

	int getMsPosRevision(int msListSize, int msOrder);

	gameMetaData::gameMode curGameMode = gameMetaData::gameMode::unknown;

	int playerCnt = 0;
	int secretCnt = 0;
	int maxLifePoint = 0;
	int arrMsCnt[gameMetaData::variableMaxCnt::msTypeCnt] = { 0 };

	int stoneMinCnt = 0;
	int stoneMaxCnt = 0;

	int starterNum = 0;
	int curPlayerNum = 0;
	int myPlayerNum = 0;
	int roundWinPlayerNum = -1;

	bool isMyNumPlayer = true;
	bool abrakaWHAT = false;

	std::vector<magicStone*> arrStones;
	std::vector<player*> arrPlayers;
	std::vector<std::vector<std::pair<cocos2d::Sprite*, bool>>> seenChecker;
	std::vector<cocos2d::Sprite*> arrScoreSpr;
	std::array<int, 4> arrScore{ 0 };
	spriteManager* sprManager;
	actionManager* actManager;

	//init each round in initRound()
	//last elem is victoryPlayer's Index
	std::array<int, 5> buf4RoundEndPopUp{ 0 }; 

	cocos2d::EventListenerCustom* uiListener;

	void checkArrStones(); //for debug
	void checkArrPlayers(); //for debug

	short loopError = 0;

	//멀티에서는 서버에서만 세팅 및 드로우 동작
	//클라에서는 서버에서 출력해야할 정보만 얻어옴
};