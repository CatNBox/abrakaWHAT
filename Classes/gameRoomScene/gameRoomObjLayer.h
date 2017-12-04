#pragma once
#include "cocos2d.h"
#include "Classes\gameObject\magicStone.h"
#include "Classes\gameObject\player.h"

class gameRoomObjLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	void initRound();

	CREATE_FUNC(gameRoomObjLayer);
private:
	void settingEventListener();
	void settingCntValues();
	void createPlayers();
	void createMagicStones();
	void createSeenChecker();
	void createPlayerLpObj();

	void selSecretStone();
	void shareStone2Player();

	void layerUpdate(float d);
	void dataUpdate();
	void curLPUpdate();

	void checkOwnedMagic(cocos2d::EventCustom* checkOwnedMagicEvent);
	void activeMagic(magicStone* activeStone);
	void passTurn();

	cocos2d::Sprite* getMSSprite(const int magicEnum);
	void setStartOrder();

	magicStone* pickAStone(const int stateEnum);
	bool isAllUsed() const; //check to discard all

	int getMsPosRevision(int msListSize, int msOrder);

	int playerCnt;
	int arrMsCnt[9] = { 0 };
	int secretCnt;

	int stoneMinCnt = 0;
	int stoneMaxCnt;

	int starterNum;
	int curPlayerNum;
	int myPlayerNum = 0;

	bool isChanged = true;

	std::vector<magicStone*> arrStones;
	std::vector<player*> arrPlayers;
	std::vector<std::vector<std::pair<cocos2d::Sprite*, bool>>> seenChecker;

	cocos2d::EventListenerCustom* uiListener;

	void checkArrStones(); //for debug
	void checkArrPlayers(); //for debug

	//멀티에서는 서버에서만 세팅 및 드로우 동작
	//클라에서는 서버에서 출력해야할 정보만 얻어옴
};