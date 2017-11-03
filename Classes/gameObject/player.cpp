#include "player.h"
#include "magicStone.h"

using namespace cocos2d;

player::player()
{
	//stoneList.reserve(5);
}

player::player(int seq)
{
	//stoneList.reserve(5);
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

int player::getBooungMS(const int idx)
{
	if (idx < getBooungListSize())
	{
		return booungList[idx]->getMagic();
	}

	return gameMetaData::magicStones::base;
}

int player::getMagic(const int idx)
{
	magicStone* magic;
	std::list<magicStone*>::iterator iter;
	iter = stoneList.begin();
	for (int i = 0; i < idx; i++)
		iter++;
	magic = *iter;
	return magic->getMagic();
}

bool player::checkOutMagic(const int magicEnum)
{
	for (auto &i : stoneList)
	{
		if (magicEnum == i->getMagic())
		{
			i->setState(gameMetaData::stoneState::discard);
			stoneList.remove(i);
			return true;
		}
	}
	return false;
}

int player::getCurLP()
{
	return roundLP;
}

void player::decreaseLP(int varyValue)
{
	roundLP -= varyValue;

	if (roundLP > 5)
		roundLP = 5;
}

void player::increaseLP(int varyValue)
{
	roundLP += varyValue;

	if (roundLP > 5)
		roundLP = 5;
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

player * player::getNextPlayer()
{
	return nextPlayer;
}

void player::setNextPlayer(player * next)
{
	nextPlayer = next;
}

npc::npc()
	:state(gameMetaData::npcState::wait)
{
	flagNPC = true;
}

void npc::npcProcess()
{
	if (state == gameMetaData::npcState::turnOn)
	{
		EventCustom checkEvent("checkOwnedMagic");
		checkEvent.setUserData((void*)gameMetaData::magicStones::pass);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
	}
}

void npc::npcTurnOn()
{
	state = gameMetaData::npcState::turnOn;
}

void npc::waitTurn()
{
	state = gameMetaData::npcState::wait;
}
