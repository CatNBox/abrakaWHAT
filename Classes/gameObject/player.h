#pragma once
#include "cocos2d.h"
#include <list>

class magicStone;

class player
{
public:
	player();
	player(int seq);
	void init();

	void pushStone2List(magicStone* ms);

	int getStoneListSize();
	int getBooungListSize();

	magicStone* getBooungMS(const int idx);
	magicStone* getMagicStone(const int idx);

	bool checkOutMagic(const int magicEnum);

	int getCurLP();
	void decreaseLP(int varyValue);
	void increaseLP(int varyValue);

	bool doHaveThisMagic(const int magicNumber);
	bool isNPC() const;

	player* getNextPlayer() const;
	void setNextPlayer(player* next);
	player* getPrevPlayer() const;
	void setPrevPlayer(player* next);

	int getDefaultX() const;
	void setDefaultX(int posX);
	int getDefaultY() const;
	void setDefaultY(int posY);
	float getRotationValue() const;
	void setRotationValue(float rot);

protected:
	int defaultX = 384;
	int defaultY = 0;
	float rotationValue = 0;

	int roundLP = 5;

	bool flagNPC = false;
	char* playerIPaddr;

	player* nextPlayer = nullptr;
	player* prevPlayer = nullptr;

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