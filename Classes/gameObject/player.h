#pragma once
#include "cocos2d.h"
#include <list>

class magicStone;

class player : public cocos2d::Object
{
public:
	player();
	player(int seq);
	void init();

	int getLeftPlayer();
	int getRightPlayer();

	void pushStone2List(magicStone* ms);
	void popStoneFromList(magicStone* ms);	//Á¦°Å

	int getStoneListSize();
	int getBooungListSize();

	magicStone* getBooungMS(const int idx);
	magicStone* getMagicStone(const int idx);

	bool checkOutMagic(const int magicEnum);

	int getCurLP();
	void decreaseLP(int varyValue);
	void increaseLP(int varyValue);

	bool doHaveThisMagic(const int magicNumber);
	bool isNPC();

	player* getNextPlayer();
	void setNextPlayer(player* next);


protected:
	int leftPlayer = 0;
	int rightPlayer = 0;
	int roundLP = 5;

	bool flagNPC = false;

	char* playerIPaddr;

	player* nextPlayer = nullptr;

	std::list<magicStone*> stoneList;
	//std::vector<magicStone*> stoneList;
	std::vector<magicStone*> booungList;
};

class npc : public player
{
public:
	npc();
	void npcProcess();
	void npcTurnOn();
	void waitTurn();
private:

	int state; // gameMetaData::npcState
};