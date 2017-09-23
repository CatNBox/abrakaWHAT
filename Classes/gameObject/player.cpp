#include "player.h"
#include "magicStone.h"

player::player()
{
	stoneList.reserve(5);
}

player::player(int seq)
{
	stoneList.reserve(5);
}

void player::init()
{
	stoneList.clear();
	booungList.clear();
}

int player::getLeftPlayer()
{
	return leftPlayer;
}

int player::getRightPlayer()
{
	return rightPlayer;
}

void player::pushStone2List(magicStone* ms)
{
	stoneList.push_back(ms);
}

void player::popStoneFromList(magicStone* ms)
{
}

int player::getStoneListSize()
{
	return stoneList.size();
}

int player::getBooungListSize()
{
	return booungList.size();
}

int player::getMagic(const int idx)
{
	return stoneList[idx]->getMagic();
}

bool player::doHaveThisMagic(const int magicNumber)
{
	for (auto const i : stoneList)
	{
		if (i->getMagic() == magicNumber)
		{
			return true;
		}
	}
	return false;
}

bool player::isNPC()
{
	return flagNPC;
}
