#pragma once
#include "cocos2d.h"

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
	void popStoneFromList(magicStone* ms);

	int getStoneListSize();
	int getBooungListSize();

	int getMagic(const int idx);

	bool doHaveThisMagic(const int magicNumber);
	bool isNPC();

protected:
	int leftPlayer = 0;
	int rightPlayer = 0;

	bool flagNPC = false;

	char* playerIPaddr;

	std::vector<magicStone*> stoneList;
	std::vector<magicStone*> booungList;
};