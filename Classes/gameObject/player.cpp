#include "gameObject/player.h"
#include "gameObject/magicStone.h"
#include "managers/gameFlowManager.h"

using namespace cocos2d;

player::player()
{
}

player::player(int idx)
	:myIndex(idx)
{
}

void player::init()
{
	stoneList.clear();
	booungList.clear();
	if (!lpSprList.empty())
		initLpObj();
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
			//Activate magicStone
			i->actionActivated();
			stoneList.remove(i);
			return true;
		}
	}
	return false;
}

int player::getCurLP()
{
	int temp = 0;
	for (const auto i : lpSprList)
	{
		if (i.second == true)
			temp++;
	}
	return temp;
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

cocos2d::Sprite * player::createLpObj(int playerIdx)
{
	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(gameMetaData::arrLpTokenName[playerIdx]);
	std::pair<cocos2d::Sprite*, bool> tempPair(tempSpr, true);
	lpSprList.push_back(tempPair);
	tempSpr->setPosition(defaultX, defaultY);
	return tempSpr;
}

void player::initLpObj()
{
	for (auto &i : lpSprList)
	{
		i.second = false;
		i.first->stopAllActions();
		i.first->setOpacity(255);
		i.first->setScale(1);
		i.first->setVisible(false);
		i.first->setRotation(0);
		i.first->setPosition(defaultX, defaultY);
	}
	actionGainLp(gameFlowManager::getInstance()->getMaxLifePoint());
}

void player::actionGainLp(int gainNum)
{
	for (auto &i : lpSprList)
	{
		if (gainNum <= 0)
			break;

		if (i.second == false)
		{
			i.second = true;

			//±âÁØ 384,600 / 168,434 / 600,434 / 384,260
			int revisionX = gameFlowManager::getInstance()->getRandomInt(0, 50);
			int revisionY = gameFlowManager::getInstance()->getRandomInt(1, 51);

			int tempX = defaultX;
			int tempY = defaultY;

			if (tempX == 384)
			{
				tempX = tempX - 100 + revisionX * 4;
			}
			else if(tempX > 384)
			{
				tempX = tempX - 70 - revisionX;
			}
			else
			{
				tempX = tempX + 70 + revisionX;
			}

			if (tempY == 434)
			{
				tempY = tempY - 100 + revisionY * 4;
			}
			else if (tempY > 434)
			{
				tempY = tempY - 70 - revisionY;
			}
			else
			{
				tempY = tempY + 70 + revisionY;
			}

			Vec2 tempVec(tempX, tempY);

			auto showThis = cocos2d::Show::create();
			auto fadeIn = cocos2d::FadeIn::create(0.7f);
			auto moving = cocos2d::MoveTo::create(0.4f, tempVec);

			auto seq = cocos2d::Sequence::create(showThis, fadeIn, moving, NULL);

			i.first->runAction(seq);
			gainNum--;
		}
	}
}

void player::actionLostLp(int lostNum)
{
	for (auto &i : lpSprList)
	{
		if (lostNum <= 0)
			break;

		if (i.second == true)
		{
			i.second = false;

			auto scaling = cocos2d::ScaleTo::create(1.0f, 2.0f);
			auto fadeOut = cocos2d::FadeOut::create(1.0f);
			auto spawning = cocos2d::Spawn::create(scaling, fadeOut, NULL);
			
			i.first->runAction(spawning);
			lostNum--;
		}
	}
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

npc::npc(int idx)
	: player(idx)
{
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
