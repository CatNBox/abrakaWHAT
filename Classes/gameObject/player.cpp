#include "player.h"
#include "magicStone.h"

using namespace cocos2d;

player::player()
{
}

player::player(int seq)
{
}

void player::init()
{
	stoneList.clear();
	booungList.clear();
}

void player::pushStone2List(magicStone* ms)
{
	stoneList.push_back(ms);
}

int player::getStoneListSize()
{
	return stoneList.size();
}

int player::getBooungListSize()
{
	return booungList.size();
}

magicStone* player::getBooungMS(const int idx)
{
	if (idx < getBooungListSize())
	{
		return booungList[idx];
	}
}

magicStone* player::getMagicStone(const int idx)
{
	magicStone* magic;
	std::list<magicStone*>::iterator iter;
	iter = stoneList.begin();
	for (int i = 0; i < idx; i++)
	{
		iter++;
	}

	magic = *iter;
	return magic;
}

bool player::checkOutMagic(const int magicEnum)
{
	for (auto &i : stoneList)
	{
		if (magicEnum == i->getMagic())
		{
			i->setStatus(gameMetaData::msStatus::discard);
			i->setVisible(false);
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

bool player::isNPC() const
{
	return flagNPC;
}

player * player::getNextPlayer() const
{
	return nextPlayer;
}

void player::setNextPlayer(player * next)
{
	nextPlayer = next;
}

player * player::getPrevPlayer() const
{
	return prevPlayer;
}

void player::setPrevPlayer(player * next)
{
	prevPlayer = next;
}

int player::getDefaultX() const
{
	return defaultX;
}

void player::setDefaultX(int posX)
{
	defaultX = posX;
}

int player::getDefaultY() const
{
	return defaultY;
}

void player::setDefaultY(int posY)
{
	defaultY = posY;
}

float player::getRotationValue() const
{
	return rotationValue;
}

void player::setRotationValue(float rot)
{
	rotationValue = rot;
}

npc::npc()
	:state(gameMetaData::npcState::npcWait)
{
	flagNPC = true;
}

void npc::npcProcess()
{
	if (state == gameMetaData::npcState::npcTurnOn)
	{
		EventCustom checkEvent("checkOwnedMagic");
		checkEvent.setUserData((void*)gameMetaData::msType::pass);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
	}
}

void npc::npcTurnOn()
{
	state = gameMetaData::npcState::npcTurnOn;
}

void npc::waitTurn()
{
	state = gameMetaData::npcState::npcWait;
}
