#include "gameObject/player.h"
#include "gameObject/magicStone.h"
#include <iostream>

using namespace cocos2d;

player::player()
{
}

player::player(int playOrder)
	:myPlayOrder(playOrder)
{
}

player::player(int playOrder, int netIndex)
	: myPlayOrder(playOrder),
	myNetIndex(netIndex)
{
}

void player::init()
{
	stoneList.clear();
	booungList.clear();
	if (!lpSprList.empty())
		initLpObj();

	if (flagNPC)
		((npc*)this)->initNpc();
}

void player::pushStone2List(magicStone* ms)
{
	stoneList.push_back(ms);
	ms->setState(gameMetaData::msState::owned);
}

void player::pushbooung2List(magicStone * ms)
{
	booungList.push_back(ms);
	ms->setState(gameMetaData::msState::owned);
}

int player::getStoneListSize()
{
	return stoneList.size();
}

int player::getBooungListSize()
{
	return booungList.size();
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
	for (auto &elemStone : stoneList)
	{
		if (magicEnum == elemStone->getMagic())
		{
			//Activate magicStone
			elemStone->actionActivated();
			stoneList.remove(elemStone);
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
	actionGainLp(gameMetaData::msType::base);
}

void player::actionGainLp(const int msTypeEnum)
{
	int gainNum = 1;
	if (msTypeEnum == gameMetaData::msType::base)
	{
		gainNum = GetPrivateProfileInt(L"RoundOption", L"maxLifePoint", gameMetaData::defaultMaxLifePoint, L"option.ini");
	}
	else if (msTypeEnum == gameMetaData::msType::wind)
	{
		gainNum = inlineFunc::getRandomInt(1, 3);
	}

	for (auto &i : lpSprList)
	{
		if (gainNum <= 0)
			break;

		if (i.second == false)
		{
			i.second = true;

			//기준 384,600 / 168,434 / 600,434 / 384,260
			int revisionX = inlineFunc::getRandomInt(10, 50);
			int revisionY = inlineFunc::getRandomInt(11, 51);

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
			auto fadeIn = cocos2d::FadeIn::create(0.5f);
			auto moving = cocos2d::MoveTo::create(0.3f, tempVec);
			auto spawning = cocos2d::Spawn::create(fadeIn, moving, NULL);

			auto seq = cocos2d::Sequence::create(showThis, spawning, NULL);

			i.first->runAction(seq);
			gainNum--;
		}
	}
}

void player::actionLostLp(int lostValue)
{
	for (auto &i : lpSprList)
	{
		if (lostValue <= 0)
			break;

		if (i.second == true)
		{
			i.second = false;

			auto scaling = cocos2d::ScaleTo::create(1.0f, 2.0f);
			auto fadeOut = cocos2d::FadeOut::create(1.0f);
			auto spawning = cocos2d::Spawn::create(scaling, fadeOut, NULL);
			auto setBack = cocos2d::CallFunc::create([=]() 
			{
				i.first->setScale(1.0f);
				i.first->setPosition(STDAXIS, STDAXIS);
			});
			auto seq = cocos2d::Sequence::create(spawning, setBack, NULL);
			
			i.first->runAction(seq);
			lostValue--;
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

const int player::getPlayOrder() const
{
	return myPlayOrder;
}

const int player::getNetIndex() const
{
	return myNetIndex;
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

/*-------------------------------------------------

			class NPC

--------------------------------------------------*/
npc::npc()
	:state(gameMetaData::npcState::npcWait)
{
	arrPrevFailList.reserve(gameMetaData::variableMaxCnt::msTypeCnt);
	initNpc();
}

npc::npc(int playOrder)
	: player(playOrder),
	 state(gameMetaData::npcState::npcWait)
{
	arrPrevFailList.reserve(gameMetaData::variableMaxCnt::msTypeCnt);
	initNpc();
}

npc::npc(int playOrder, int netIdx)
	:player(playOrder, netIdx),
	state(gameMetaData::npcState::npcWait)
{
	arrPrevFailList.reserve(gameMetaData::variableMaxCnt::msTypeCnt);
	initNpc();
}

void npc::initNpc()
{
	flagNPC = true;
	for (auto &i : arrMsScore)
	{
		i = std::make_pair(0, 0);
	}

	for (auto &i : arrDiscardCnt)
	{
		i = 0;
	}
	arrPrevFailList.clear();
}

void npc::npcProcess()
{
	if (state == gameMetaData::npcState::npcTurnOn)
	{
		if (newHandCnt > 0)
			arrPrevFailList.clear();

		duplArrDiscardCnt2ArrMsScore();
		countAnotherPlayerHand();
		countMySecret();
		calcScoreMsList();
		int targetMagic = chooseMs();
		std::cout << "선택된 번호 : " << targetMagic << std::endl;

		prevChoice = targetMagic;
		//계산후 이 부분의 선택한 magicStone값을 설정한 후 스케줄로 호출
		cocos2d::Director::getInstance()->getScheduler()->schedule(
			[targetMagic](float dt)
		{
			cocos2d::EventCustom checkEvent("checkOwnedMagic");
			checkEvent.setUserData((void*)targetMagic);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
		}, this, 0.0f, 0, thinkRowhenTime, false, "npcChoice");
	}
}

void npc::npcTurnOn()
{
	state = gameMetaData::npcState::npcTurnOn;
	prevChoice = gameMetaData::msType::yongyong;
	for (int msNum = gameMetaData::msType::yongyong; msNum < gameMetaData::variableMaxCnt::msTypeCnt; msNum++)
	{
		arrMsScore[msNum].first = arrDiscardCnt[msNum];
	}
}

void npc::waitTurn()
{
	state = gameMetaData::npcState::npcWait;
}

void npc::choiceFail(int magicEnum)
{
	arrPrevFailList.push_back(magicEnum);
}

void npc::countAnotherPlayerHand()
{
	auto playerCursor = this->nextPlayer;
	for (int anotherPlayerCnt = 0; anotherPlayerCnt < 3; anotherPlayerCnt++)
	{
		for (int msHandIdx = 0; msHandIdx < playerCursor->getStoneListSize(); msHandIdx++)
		{
			arrMsScore[playerCursor->getMagicStone(msHandIdx)->getMagic()].first++;
		}
		playerCursor = playerCursor->getNextPlayer();
	}
}

void npc::countMySecret()
{
	for (auto i : booungList)
	{
		arrMsScore[i->getMagic()].first++;
	}
}

void npc::setDiscardCnt(const int magicEnum, const int cnt)
{
	arrDiscardCnt[magicEnum] = cnt;
}

void npc::setNewHandCnt(const int cnt)
{
	newHandCnt = cnt;
}

void npc::duplArrDiscardCnt2ArrMsScore()
{
	for (int msNum = gameMetaData::msType::yongyong; msNum < gameMetaData::variableMaxCnt::msTypeCnt; msNum++)
	{
		arrMsScore[msNum].first = arrDiscardCnt[msNum];
	}
}

void npc::calcScoreMsList()
{
	int msIdx = gameMetaData::msType::base;
	for (auto &elemMsScore : arrMsScore)
	{
		if (msIdx > 0)
		{
			elemMsScore.second = (msIdx - elemMsScore.first) * 100 / msIdx;
			std::cout << myPlayOrder << "번 npc score 계산 " << msIdx << "번 magicStone : " << elemMsScore.second << std::endl;
		}
		msIdx++;
	}
}

int npc::chooseMs()
{
	//<msIdx, score>
	std::pair<int, int> maxScore = std::make_pair(gameMetaData::msType::pass, 0);
	
	std::cout << std::endl << "chooseMs prevChoice value : " << prevChoice <<  std::endl << std::endl;
	for (int msNum = prevChoice; msNum < gameMetaData::variableMaxCnt::msTypeCnt; msNum++)
	{
		if (std::find(arrPrevFailList.begin(), arrPrevFailList.end(), msNum) != arrPrevFailList.end())
			continue;

		if (arrMsScore[msNum].second > maxScore.second)
		{
			maxScore.first = msNum;
			maxScore.second = arrMsScore[msNum].second;
			if (msNum == gameMetaData::msType::yongyong)
				maxScore.second = 50 * inlineFunc::getRandomInt(0,2);
		}

		if (maxScore.second > 65)
			break;
	}
	//return maxScore.second < 50 ? gameMetaData::msType::pass : maxScore.first + 1;
	return maxScore.first;
}
