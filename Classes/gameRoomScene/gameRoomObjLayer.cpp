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
	this->schedule(SEL_SCHEDULE(&gameRoomObjLayer::screenUpdate));

	return true;
}

void gameRoomObjLayer::screenUpdate(float d)
{
	//remove All
	this->removeAllChildren();
	//std::cout << "removeAllChildren before screenUpdate" << std::endl;

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
				tempSpr->setPosition(Vec2(defaultX , defaultY + recalibration));
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
	//205,400 / 16*32 / y+40 x+20

	auto seenMS5 = Sprite::createWithSpriteFrameName("ms5_seen.png");
	seenMS5->setPosition(Vec2(205, 400));
	this->addChild(seenMS5);

	auto seenMS5_1 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_1->setPosition(Vec2(205, 440));
	this->addChild(seenMS5_1);

	auto seenMS5_2 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_2->setPosition(Vec2(205, 480));
	this->addChild(seenMS5_2);

	auto seenMS5_3 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_3->setPosition(Vec2(205, 520));
	this->addChild(seenMS5_3);

	auto seenMS5_4 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_4->setPosition(Vec2(225, 440));
	this->addChild(seenMS5_4);

	auto seenMS5_5 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_5->setPosition(Vec2(245, 440));
	this->addChild(seenMS5_5);

	auto seenMS5_6 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_6->setPosition(Vec2(225, 480));
	this->addChild(seenMS5_6);

	auto seenMS5_7 = Sprite::createWithSpriteFrame(seenMS5->getSpriteFrame());
	seenMS5_7->setPosition(Vec2(225, 520));
	this->addChild(seenMS5_7);
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
