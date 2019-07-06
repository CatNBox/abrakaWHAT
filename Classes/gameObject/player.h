#pragma once
#include <array>
#include <list>
#include <vector>
#include "cocos2d.h"
#include "gameObject\gameMetaData.h"

class magicStone;

class player
{
public:
	player();
	player(int playOrder);
	player(int playOrder, int netIndex);
	void init();

	void pushStone2List(magicStone* ms);
	void pushbooung2List(magicStone* ms);
	int getStoneListSize();
	int getBooungListSize();
	magicStone* getMagicStone(const int idx);

	bool checkOutMagic(const int magicEnum);

	int getCurLP();
	//--createLpSprite with player's default position
	cocos2d::Sprite* createLpObj();
	void initLpObj();
	void actionGainLp(const int msTypeEnum, const int gainLpNum = 1);
	void actionLostLp(const int lostValue);

	bool doHaveThisMagic(const int magicNumber);
	bool isNPC() const;

	player* getNextPlayer() const;
	void setNextPlayer(player* next);
	player* getPrevPlayer() const;
	void setPrevPlayer(player* next);
	const int getPlayOrder() const;
	void setPlayOrder(const int playOrder);
	const int getNetIndex() const;

	int getDefaultX() const;
	void setDefaultX(int posX);
	int getDefaultY() const;
	void setDefaultY(int posY);
	float getRotationValue() const;
	void setRotationValue(float rot);

	void setPlayIdx(int playerIdx);
	const int getPlayIdx();

protected:
	int defaultX = 384;
	int defaultY = 0;
	float rotationValue = 0;
	int myPlayerIdx = 0;
	int myPlayOrder = 0;
	int myNetIndex = 0;

	bool flagNPC = false;
	//char* playerIPaddr;

	player* nextPlayer = nullptr;
	player* prevPlayer = nullptr;

	std::list<magicStone*> stoneList;
	std::vector<magicStone*> booungList;
	std::vector<std::pair<cocos2d::Sprite*, bool>> lpSprList;
};

class npc : public player
{
public:
	npc();
	npc(int playOrder);
	npc(int playOrder, int netIdx);
	void initNpc();
	void npcProcess(gameMetaData::gameMode curMode);
	void npcTurnOn();
	void waitTurn();
	void choiceFail(const int magicEnum);
	void setDiscardCnt(const int magicEnum, const int cnt);
	void setNewHandCnt(const int cnt);

private:
	void countAnotherPlayerHand();
	void countMySecret();
	void duplArrDiscardCnt2ArrMsScore();
	void calcScoreMsList();
	int chooseMs();

	int prevChoice = gameMetaData::msType::yongyong;
	int newHandCnt = 0;

	std::array<std::pair<int, int>, gameMetaData::variableMaxCnt::msTypeCnt> arrMsScore;
	std::array<int, gameMetaData::variableMaxCnt::msTypeCnt> arrDiscardCnt;
	std::vector<int> arrPrevFailList;
	float thinkRowhenTime = 2.0f;
	gameMetaData::npcState state; // gameMetaData::npcState
};