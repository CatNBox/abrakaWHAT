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
	
	bool bPlayer = true;

	//create players
	arrPlayers.resize(playerCnt);
	for (auto &i : arrPlayers)
	{
		player* tempPlayer;
		if (bPlayer == true)
		{
			tempPlayer = new player;
			bPlayer = false;
		}
		else
		{
			tempPlayer = new npc;
		}
		i = tempPlayer;
	}
	for (int i = 0; i < playerCnt; i++)
	{
		int next = (i == playerCnt - 1) ? (0) : (i + 1);
		arrPlayers[i]->setNextPlayer(arrPlayers[next]);
	}

	//create magicStones
	arrStones.resize(stoneMaxCnt);
	arrStones[0] = new msYongyong;
	arrStones[yongyongCnt] = new msBangrang;
	arrStones[yongyongCnt + bangrangCnt] = new msWind;
	arrStones[yongyongCnt + bangrangCnt	+ windCnt] = new msBooung;
	arrStones[yongyongCnt + bangrangCnt + windCnt + booungCnt] = new msBunpok;
	arrStones[yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt] = new msNungang;
	arrStones[yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt + nungangCnt] = new msBuljak;
	arrStones[yongyongCnt + bangrangCnt + windCnt + booungCnt + bunpokCnt + nungangCnt + buljakCnt] = new msPostion;
	int prototypeIndex = 0;
	for (int i = 0; i < (int)arrStones.size(); i++)
	{
		if (arrStones[i] != nullptr)
		{
			prototypeIndex = i;
			continue;
		}
		arrStones[i] = arrStones[prototypeIndex]->clone();	//prototype pattern
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

	if (isChanged)
	{
		//remove All object
		this->removeAllChildren();

		//update screen
		seenCheckUpdate();
		stoneObjUpdate();
		curLPUpdate();

		isChanged = false;
	}
}

void gameRoomObjLayer::dataUpdate()
{
	//discard된 숫자만큼 해당 ms의 seenCnt증가
	for (int i = 0; i < 8; i++)
	{
		seenCnt[i] = 0;
	}
	for (auto &i : arrStones)
	{
		int magicEnum = i->getMagic() - 1;
		if (i->getState() == gameMetaData::stoneState::discard)
			seenCnt[magicEnum]++;
	}

	if (arrPlayers[curPlayerNum]->isNPC())
	{
		npc* tempNpc = (npc*)arrPlayers[curPlayerNum];
		tempNpc->npcProcess();
	}
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
	for (int msNum = 0; msNum < 8; msNum++)
	{
		//seenChecker 스프라이트들의 좌표계산용
		int defaultX = 205, defaultY = 400, recalibX = 23, recalibY = 40;
		if (msNum > 3)
			defaultX = 205 + recalibX * 3 + 35;

		//seenCnt의 값만큼 seen스프라이트 그림, 그 외에는 notSeen스프라이트
		for (int seenObjNum = 0; seenObjNum < msNum + 1; seenObjNum++)
		{
			if(seenObjNum < seenCnt[msNum])
				seenMs = Sprite::createWithSpriteFrameName(arrSeen[msNum]);
			else
				seenMs = Sprite::createWithSpriteFrameName(arrNotSeen[msNum]);

			seenMs->setPosition(Vec2(defaultX + seenObjNum * recalibX, defaultY + (msNum % 4) * recalibY));
			this->addChild(seenMs);
		}
	}
}

void gameRoomObjLayer::stoneObjUpdate()
{
	//플레이어 덱 검사후 출력설정
	for (int i = 0; i < (int)arrPlayers.size(); i++)
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

	//booung비밀의돌
	for (int i = 0; i < secretCnt; i++)
	{
		//비밀의 돌은 현재 클라이언트에 해당하는 플레이어의 List만 체크하면 되므로
		//기본적으로 magicStones::base (마법의돌 뒷면)을 출력
		auto tempSpr = getMSSprite(gameMetaData::magicStones::base);

		auto me = arrPlayers[myPlayerNum];
		int myBooungSize = me->getBooungListSize();
		if (i < myBooungSize)
		{
			auto tempMagic = me->getBooungMS(i);
			if (tempMagic != gameMetaData::magicStones::base)
			{
				tempSpr = getMSSprite(tempMagic);
			}
			else
			{
				//오류팝업
				std::cout << "booungList size error" << std::endl;
			}
		}

		//230 + 100, 600
		tempSpr->setPosition(Vec2(230 + i * 100, 600));
		this->addChild(tempSpr);
	}
}

void gameRoomObjLayer::curLPUpdate()
{
	//p0 370 330, p1 140 390, p2 380 620, p3 630 390

	for (int i = 0; i < (int)arrPlayers.size(); i++)
	{
		std::string tempStrLP = std::to_string(arrPlayers[i]->getCurLP());
		auto tempLabelLP = Label::createWithTTF(tempStrLP, "fonts/Marker Felt.ttf", 36);
		
		switch (i)
		{
		case 0:
			tempLabelLP->setPosition(Vec2(370, 330));
			break;
		case 1:
			tempLabelLP->setPosition(Vec2(140, 390));
			break;
		case 2:
			tempLabelLP->setPosition(Vec2(380, 620));
			break;
		case 3:
			tempLabelLP->setPosition(Vec2(630, 390));
		}
		this->addChild(tempLabelLP);
	}
}

//보유한 magicStone 체크이벤트
void gameRoomObjLayer::checkOwnedMagic(EventCustom* checkOwnedMagicEvent)
{
	//임시코드
	isChanged = true;

	std::cout << "check Event in" << std::endl;
	int magicEnum = (int)(checkOwnedMagicEvent->getUserData());
	std::cout << "checkOwnedMagic Event activate : " << magicEnum << std::endl;

	if (magicEnum == gameMetaData::magicStones::pass)
	{
		passTurn();
		std::cout << "passTurn : " << magicEnum << std::endl;
	}
	else if (arrPlayers[curPlayerNum]->checkOutMagic(magicEnum))
	{
		std::cout << "magic activate : " << magicEnum << std::endl;
		EventCustom callBackEvent("choicerUIEnabled");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&callBackEvent);
	}
	else
	{
		std::cout << "not exist : " << magicEnum << std::endl;
		passTurn();
	}
}

void gameRoomObjLayer::activeMagic(magicStone * activeStone)
{
}

void gameRoomObjLayer::passTurn()
{
	//임시코드
	isChanged = true;

	//current Player
	auto curPlayer = arrPlayers[curPlayerNum];

	//magicStone의 보유개수가 5개가 아니고 사용되지않은 magicStone이 있다면 보충한다
	//모두 사용되었다면 라운드를 종료한다
	if (isAllUsed())
	{
		//라운드종료
		//win(curPlayer);
		checkArrStones();
		std::cout << "All MagicStone is used" << std::endl;
	}
	else
	{
		//Draw magicStone
		int curPlayerReqCnt = 5 - curPlayer->getStoneListSize();
		for (int i = 0; i < curPlayerReqCnt; i++)
		{
			auto tempSelStone = pickAStone(gameMetaData::stoneState::notUse);
			if (tempSelStone == nullptr)
			{
				break;
			}
			else
			{
				curPlayer->pushStone2List(tempSelStone);
				tempSelStone->setState(gameMetaData::stoneState::owned);
			}
		}
	}


	//턴을 넘긴다
	if (curPlayer->isNPC())
	{
		((npc*)curPlayer)->waitTurn();
	}
	//std::cout << "curPlayerNum : " << curPlayerNum << std::endl;
	curPlayerNum = (curPlayerNum == playerCnt - 1) ? (0) : (curPlayerNum + 1);
	//std::cout << "curPlayerNum : " << curPlayerNum << std::endl;
	
	if (curPlayerNum != myPlayerNum)
	{
		if (arrPlayers[curPlayerNum]->isNPC())
		{
			((npc*)arrPlayers[curPlayerNum])->npcTurnOn();
		}
		EventCustom passTurnEvent("passTurn2NextUser");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);
	}
	else
	{
		EventCustom passTurnEvent("myTurn");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);
	}
}

void gameRoomObjLayer::initRound()
{
	//임시코드
	isChanged = true;

	//0.각 객체 초기화
	for (auto &i : arrStones)
	{
		i->init();
	}
	for (auto &i : arrPlayers)
	{
		i->init();
	}

	//라운드 표시
	EventCustom passTurnEvent("roundUp");
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);

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

	//3. 시작플레이어 결정
	starterNum = 0;
	curPlayerNum = 0;
	setStartOrder(); //starterNum setting

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
	starterNum = (rand() % playerCnt) + 1;
}

magicStone * gameRoomObjLayer::pickAStone(const int stateEnum)
{
	if (isAllUsed())
	{
		checkArrStones();
		std::cout << "All MagicStone is used" << std::endl;
		//뽑을 카드가 없을 경우 예외설정
		return nullptr;
	}

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
