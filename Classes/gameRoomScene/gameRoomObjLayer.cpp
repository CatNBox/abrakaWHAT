#include "gameRoomObjLayer.h"
#include "managers\gameFlowManager.h"
#include <iostream>
#include <random>

using namespace cocos2d;

bool gameRoomObjLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//eventListener setting
	uiListener = EventListenerCustom::create("initRound", 
		CC_CALLBACK_0(gameRoomObjLayer::initRound,this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);
	
	uiListener = EventListenerCustom::create("checkOwnedMagic",
		CC_CALLBACK_1(gameRoomObjLayer::checkOwnedMagic, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);

	//count option value load
	playerCnt = gameFlowManager::getInstance()->getPlayerCount();
	secretCnt = gameFlowManager::getInstance()->getSecretCount();
	yongyongCnt = gameFlowManager::getInstance()->getYongyongCount();
	bangrangCnt = gameFlowManager::getInstance()->getBangrangCount();
	windCnt = gameFlowManager::getInstance()->getWindCount();
	booungCnt = gameFlowManager::getInstance()->getBooungCount();
	bunpokCnt = gameFlowManager::getInstance()->getBunpokCount();
	nungangCnt = gameFlowManager::getInstance()->getNungangCount();
	buljakCnt = gameFlowManager::getInstance()->getBuljakCount();
	postionCnt = gameFlowManager::getInstance()->getPostionCount();
	stoneMaxCnt = yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt + nungangCnt + buljakCnt + postionCnt;
	

	//create players
	arrPlayers.resize(playerCnt);
	for (auto &i : arrPlayers)
	{
		auto *tempPlayer = new player;
		i = tempPlayer;
	}

	//create magicStones
	arrStones.resize(stoneMaxCnt);
	for (int i = 0; i < arrStones.size(); i++)
	{
		magicStone *temp;
		if (i < yongyongCnt)
		{
			temp = new ms_yongyong;
		}
		else if (i < yongyongCnt 
			+ bangrangCnt)
		{
			temp = new ms_bangrang;
		}
		else if (i < yongyongCnt + bangrangCnt 
			+ windCnt)
		{
			temp = new ms_wind;
		}
		else if (i < yongyongCnt + bangrangCnt + windCnt 
			+ booungCnt)
		{
			temp = new ms_booung;
		}
		else if (i < yongyongCnt + bangrangCnt + windCnt + booungCnt 
			+ bunpokCnt)
		{
			temp = new ms_bunpok;
		}
		else if (i < yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt
			+ nungangCnt)
		{
			temp = new ms_nungang;
		}
		else if (i < yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt + nungangCnt
			+ buljakCnt)
		{
			temp = new ms_buljak;
		}
		else if (i < yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt + nungangCnt + buljakCnt
			+ postionCnt)
		{
			temp = new ms_postion;
		}

		arrStones[i] = temp;
	}

	std::cout << "arrStones size : " << arrStones.size() << std::endl;

	//init round setting
	initRound();
	
	//draw update set
	this->schedule(SEL_SCHEDULE(&gameRoomObjLayer::layerUpdate));

	return true;
}

void gameRoomObjLayer::layerUpdate(float d)
{
	//update object state
	dataUpdate();

	//remove All object
	this->removeAllChildren();

	//update screen
	seenCheckUpdate();
	stoneObjUpdate();
}

void gameRoomObjLayer::dataUpdate()
{
}

void gameRoomObjLayer::seenCheckUpdate()
{
	//205,400 / 16*32 / y+40 x+23 / +35
	std::string arrNotSeen[] = { 
		"ms1_notSeen.png",
		"ms2_notSeen.png",
		"ms3_notSeen.png",
		"ms4_notSeen.png",
		"ms5_notSeen.png",
		"ms6_notSeen.png",
		"ms7_notSeen.png",
		"ms8_notSeen.png"
	};
	std::string arrSeen[] = {
		"ms1_seen.png",
		"ms2_seen.png",
		"ms3_seen.png",
		"ms4_seen.png",
		"ms5_seen.png",
		"ms6_seen.png",
		"ms7_seen.png",
		"ms8_seen.png"
	};
	
	Sprite* seenMs;
	int seenObjNum = 0;
	for (auto &i : arrStones)
	{
		int msNum = i->getMagic() - 1;

		int defaultX = 205, defaultY = 400, recalibX = 23, recalibY = 40;
		if (msNum > 3)
			defaultX = 205 + recalibX * 3 + 35;

		if (seenObjNum > msNum)
			seenObjNum = 0;

		if (i->getState() == gameMetaData::stoneState::discard)
			seenMs = Sprite::createWithSpriteFrameName(arrSeen[msNum]);
		else
			seenMs = Sprite::createWithSpriteFrameName(arrNotSeen[msNum]);

		seenMs->setPosition(Vec2(defaultX + seenObjNum * recalibX, defaultY + (msNum % 4) * recalibY));
		this->addChild(seenMs);

		seenObjNum++;
	}
}

void gameRoomObjLayer::stoneObjUpdate()
{
	//플레이어 덱 검사후 출력설정
	for (int i = 0; i < arrPlayers.size(); i++)
	{
		//basic screen size 768
		auto curPlayer = arrPlayers[i];
		int defaultX = 384;
		int defaultY = 0;
		if (i == 0)
		{
			defaultY = 160;
		}
		else if (i == 1)
		{
			defaultX = 700;
			defaultY = 434;
		}
		else if (i == 2)
		{
			defaultY = 700;
		}
		else if (i == 3)
		{
			defaultX = 68;
			defaultY = 434;
		}

		int stoneCnt = curPlayer->getStoneListSize();
		for (int j = 0; j < stoneCnt; j++)
		{
			int tempMagic = 0;
			if (i == 0) {
				tempMagic = gameMetaData::magicStones::base;
			}
			else {
				tempMagic = curPlayer->getMagic(j);
			}

			auto tempSpr = getMSSprite(tempMagic);
			int recalibration = 38 * (stoneCnt - 1 - (j * 2));

			//setPosition and rotation
			if (i == 1) {
				tempSpr->setRotation(90.0f);
				tempSpr->setPosition(Vec2(defaultX, defaultY + recalibration));
			}
			else if (i == 3) {
				tempSpr->setRotation(-90.0f);
				tempSpr->setPosition(Vec2(defaultX, defaultY + recalibration));
			}
			else {
				tempSpr->setPosition(Vec2(defaultX + recalibration, defaultY));
			}

			this->addChild(tempSpr);
		}
	}
}

void gameRoomObjLayer::checkOwnedMagic(EventCustom* checkOwnedMagicEvent)
{
	int magicEnum = (int)(checkOwnedMagicEvent->getUserData());
	std::cout << "checkOwnedMagic Event activate : " << magicEnum << std::endl;
}

void gameRoomObjLayer::activeMagic(magicStone * activeStone)
{
}

void gameRoomObjLayer::initRound()
{
	//0.각 객체 초기화
	for (auto &i : arrStones)
	{
		i->init();
	}
	for (auto &i : arrPlayers)
	{
		i->init();
	}

	//1. 비밀의 돌 먼저 선택
	for (int i = 0; i < secretCnt; i++)
	{
		magicStone *secretTemp;
		secretTemp = pickAStone(gameMetaData::stoneState::notUse);
		secretTemp->setState(gameMetaData::stoneState::secret);
	}
	//2. 돌분배

	int tempDrawCnt = playerCnt * 5;
	for (int i = 0; i < tempDrawCnt; i++)
	{
		magicStone *tempSelStone;
		tempSelStone = pickAStone(gameMetaData::stoneState::notUse);
		int tempCurPlayer = i%playerCnt;
		arrPlayers[tempCurPlayer]->pushStone2List(tempSelStone);
		tempSelStone->setState(gameMetaData::stoneState::owned);
	}

	//checkArrStones();
	//checkArrPlayers();
}

cocos2d::Sprite* gameRoomObjLayer::getMSSprite(const int magicEnum)
{
	switch (magicEnum)
	{
	case gameMetaData::magicStones::yongyong :
		return Sprite::createWithSpriteFrameName("ms1_yongyong.png");
	case gameMetaData::magicStones::bangrang :
		return Sprite::createWithSpriteFrameName("ms2_bangrang.png");
	case gameMetaData::magicStones::wind :
		return Sprite::createWithSpriteFrameName("ms3_wind.png");
	case gameMetaData::magicStones::booung :
		return Sprite::createWithSpriteFrameName("ms4_booung.png");
	case gameMetaData::magicStones::bunpok :
		return Sprite::createWithSpriteFrameName("ms5_bunpok.png");
	case gameMetaData::magicStones::nungang :
		return Sprite::createWithSpriteFrameName("ms6_nungang.png");
	case gameMetaData::magicStones::buljak :
		return Sprite::createWithSpriteFrameName("ms7_buljack.png");
	case gameMetaData::magicStones::postion :
		return Sprite::createWithSpriteFrameName("ms8_postion.png");
	case gameMetaData::magicStones::base :
	default:
		return Sprite::createWithSpriteFrameName("ms_bg.png");
	}

	//return nullptr;
}

void gameRoomObjLayer::setStartOrder()
{
	srand(time(nullptr));
	starter = (rand() % playerCnt) + 1;
}

magicStone * gameRoomObjLayer::pickAStone(const int stateEnum)
{
	if (isAllUsed()) return nullptr;

	magicStone *temp;
	int rndIndex = 0;
	while (true)
	{
		rndIndex = getRandomIndex();
		if (arrStones[rndIndex]->getState() == stateEnum)
		{
			temp = arrStones[rndIndex];
			break;
		}
	}
	return temp;
}

bool gameRoomObjLayer::isAllUsed() const
{
	for (auto const &i : arrStones)
	{
		if (i->getState() == gameMetaData::stoneState::notUse)
		{
			return false;
		}
	}
	return true;
}

int gameRoomObjLayer::getRandomIndex()
{
	std::random_device rd;
	std::mt19937_64 rnd(rd());

	std::uniform_int_distribution<int> range(stoneMinCnt, stoneMaxCnt-1);

	return range(rnd);
}

void gameRoomObjLayer::checkArrStones()
{
	std::cout << "---Check arrStones---" << std::endl;
	for (auto const &i : arrStones)
	{
		std::cout << i->getMagic() << " : " << i->getState() << std::endl;
	}
	std::cout << "---------------------" << std::endl;
}

void gameRoomObjLayer::checkArrPlayers()
{
	for (auto const &i : arrPlayers)
	{
		std::cout << "StoneList size : " << i->getStoneListSize() << std::endl;
		std::cout << "BooungList size : " << i->getBooungListSize() << std::endl;
	}
}
