#pragma once
#include "cocos2d.h"
#include "gameObject/gameMetaData.h"
#include <array>

class magicStone;
class player;
class spriteManager;
class actionManager;
class networkManager;

class gameRoomObjLayer : public cocos2d::Layer
{
public:
	virtual bool init(gameMetaData::gameMode modeFlag, int playerTurnOrder[]);
	//CREATE_FUNC(gameRoomObjLayer);

	static gameRoomObjLayer* createWithParam(gameMetaData::gameMode modeFlag, int playerTurnOrder[]);

	void initRound();

private:
	void delay01secCallWrapper(void (gameRoomObjLayer::*targetFunc)(void));
	void settingEventListener();
	void settingCntValues();
	void createPlayers();
	void createMagicStones();
	void createSeenChecker();
	void createPlayerLpObj();
	void createScoreSpr();
	void createTurnMarker();

	void update(float dTime);
	void runUpdate();

	void startGameByNpc();

	void selectSecretStone();
	void distributeStone2Player();

	void selectSecretStonebyNetData();
	void distributeStoneByNetdata();

	void checkOwnedMagic(cocos2d::EventCustom* checkOwnedMagicEvent);
	void requestCheckOwnedMagic(cocos2d::EventCustom* checkOwnedMagicEvent);
	void netmodeCheckOwnedMagic();
	void activateMagic(const int magicEnum, const int damage = 1);
	void updateSeenChecker(const int magicEnum);
	void refillPlayerHand(const int curIdx);
	void reorderPlayerHand();
	bool isRoundEnd();
	void calcScore();
	void callEndRoundEvent();
	void callEndGameEvent();
	void passTurn();
	void netmodePassTurn();
	void requestRefillHand(player* curPlayer);

	cocos2d::Sprite* getMSSprite(const int magicEnum);

	void callNpcProcess();

	magicStone* pickAStone(const int stateEnum);
	magicStone* pickAStone(const int stateEnum, const short magicEnum);
	short netmodePickAStone();
	bool isAllUsed() const; //check to discard all

	int getMsPosRevision(int msListSize, int msOrder);

	//call when change curPlayerIdx value : initRound, passTurn, netmodePassTurn, isRoundEnd
	void curTurnMarkerToggle();

	//nullptr error log write
	void writeErrLog(std::string logMsg);
	
	gameMetaData::gameMode curGameMode;
	gameMetaData::gameProgressStage curProgressStage;

	int playerCnt = 0;
	int secretCnt = 0;
	int maxLifePoint = 0;
	int arrMsCnt[gameMetaData::variableMaxCnt::msTypeCnt] = { 0 };

	int stoneMinCnt = 0;
	int stoneMaxCnt = 0;

	int starterIdx = 0;
	int curPlayerIdx = 0;
	int curOrder = 0;
	int myPlayerIdx = 0;
	int myTurnOrder = 0;
	int roundWinPlayerNum = -1;

	bool isMyNumPlayer = true;
	bool abrakaWHAT = false;

	std::vector<magicStone*> arrStones;
	std::vector<std::vector<std::pair<cocos2d::Sprite*, bool>>> seenChecker;

	//vector[playerIndex] = score number sprite
	std::vector<cocos2d::Sprite*> arrScoreSpr;
	//array[playerIndex] = score
	std::array<int, gameMetaData::defaultPlayerCnt> arrScore{ 0 };

	//vector[index] = player
	std::vector<player*> playersList;

	//array[order] = playerIndex #order range 0~playerCnt
	std::array<int, gameMetaData::defaultPlayerCnt> arrPlayerIdxInOrder{ 0 };

	spriteManager* sprManager;
	actionManager* actManager;
	networkManager* netManager;
	cocos2d::Director* cocosDirector;

	//init each round in initRound()
	//last elem is victoryPlayer's Index
	std::array<int, 5> buf4RoundEndPopUp{ 0 }; 

	//vector[playerOrder] = *turnMarkerSprite
	std::vector<cocos2d::Sprite*> arrTurnMarkerSpr;

	cocos2d::EventListenerCustom* uiListener;

	void checkArrStones(); //for debug
	void checkArrPlayers(); //for debug

	short loopError = 0;

	//for network
	short bufSecretDeck[4];
	short bufPlayer1Hand[5];
	short bufPlayer2Hand[5];
	short bufPlayer3Hand[5];
	short bufPlayer4Hand[5];
	short bufRefillHand[5];

	short bufDrawCnt = 0;
	int bufCurPlayerIdx = 0;
	
	//멀티에서는 서버에서만 세팅 및 드로우 동작
	//클라에서는 서버에서 출력해야할 정보만 얻어옴
};