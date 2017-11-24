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
	//유저순서
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
		}
		else
		{
			arrStones[i] = arrStones[prototypeIndex]->clone();	//prototype pattern
		}
		this->addChild(arrStones[i]);
	}

	std::cout << "arrStones size : " << arrStones.size() << std::endl;

	//init round setting
	initRound();
	
	//draw update set
	this->schedule(SEL_SCHEDULE(&gameRoomObjLayer::layerUpdate));

	return true;
}

void gameRoomObjLayer::initRound()
{
	//임시코드
	isChanged = true;

	//0.각 객체 초기화
	for (auto &i : arrStones)
	{
		i->initObjData();
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
		secretTemp = pickAStone(gameMetaData::msStatus::notUse);
		if (secretTemp == nullptr)
		{
			std::cout << "initRound()::secretTemp에서 돌이 모두 사용되어서 분배 불가능" << std::endl;
			break;
		}

		secretTemp->setStatus(gameMetaData::msStatus::secret);
	}

	//2. 돌분배
	int tempDrawCnt = playerCnt * 5;
	for (int i = 0; i < tempDrawCnt; i++)
	{
		magicStone *tempSelStone;
		tempSelStone = pickAStone(gameMetaData::msStatus::notUse);
		if (tempSelStone == nullptr)
		{
			std::cout << "initRound()::tempSelStone에서 돌이 모두 사용되어서 분배 불가능" << std::endl;
			break;
		}

		//플레이어 돌아가며 분배
		int tempCurPlayer = i%playerCnt;
		arrPlayers[tempCurPlayer]->pushStone2List(tempSelStone);
		tempSelStone->setStatus(gameMetaData::msStatus::owned);
	}

	//3. 시작플레이어 결정
	starterNum = 0;
	curPlayerNum = 0;
	setStartOrder(); //starterNum setting

	//checkArrStones();
	//checkArrPlayers();
}

void gameRoomObjLayer::setStartOrder()
{
	srand(time(nullptr));
	starterNum = (rand() % playerCnt) + 1;
}

void gameRoomObjLayer::layerUpdate(float d)
{
	//update object state
	dataUpdate();

	if (isChanged)
	{
		//remove All object
		//this->removeAllChildren();

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
		if (i->getStatus() == gameMetaData::msStatus::discard)
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
				seenMs = Sprite::createWithSpriteFrameName(gameMetaData::arrSeenSpriteName[msNum]);
			else
				seenMs = Sprite::createWithSpriteFrameName(gameMetaData::arrNotSeenSpriteName[msNum]);

			seenMs->setPosition(Vec2(defaultX + seenObjNum * recalibX, defaultY + (msNum % 4) * recalibY));
			this->addChild(seenMs);
		}
	}
}

void gameRoomObjLayer::stoneObjUpdate()
{
	//check player's deck then draw magicStone
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
			magicStone* ptempMS;
			ptempMS = curPlayer->getMagicStone(j);

			//set Texture
			//if (i == myPlayerNum) 
			//	ptempMS->setBaseSprite();
			//else 
				ptempMS->initMsTexture();

			int revisionValue = 38 * (stoneCnt - 1 - (j * 2));

			//setPosition and rotation
			if (i == 1) {//right player
				ptempMS->setRotation(90.0f);
				ptempMS->setPosition(Vec2(defaultX, defaultY + revisionValue));
			}
			else if (i == 3) {//left player
				ptempMS->setRotation(-90.0f);
				ptempMS->setPosition(Vec2(defaultX, defaultY + revisionValue));
			}
			else {
				ptempMS->setRotation(0);
				ptempMS->setPosition(Vec2(defaultX + revisionValue, defaultY));
			}
			ptempMS->setVisible(true);
		}
	}

	//draw secretStones
	int sceretRevisionValue = 0;
	for (auto &i : arrStones)
	{
		if (i->getStatus() != gameMetaData::msStatus::secret)
			continue;

		//230 + 100, 600
		i->setBaseSprite();
		i->setPosition(Vec2(230 + sceretRevisionValue * 100, 600));
		i->setVisible(true);
		sceretRevisionValue++;
	}
	//if a secretStone belong to me, initMsTexture to magic
	auto me = arrPlayers[myPlayerNum];
	int myBooungSize = me->getBooungListSize();
	for (int i = 0; i < myBooungSize; i++)
	{
		auto tempMS = me->getBooungMS(i);
		tempMS->initMsTexture();
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

	if (magicEnum == gameMetaData::msType::pass)
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
			auto tempSelStone = pickAStone(gameMetaData::msStatus::notUse);
			if (tempSelStone == nullptr)
			{
				break;
			}
			else
			{
				curPlayer->pushStone2List(tempSelStone);
				tempSelStone->setStatus(gameMetaData::msStatus::owned);
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

cocos2d::Sprite* gameRoomObjLayer::getMSSprite(const int magicEnum)
{
	return Sprite::createWithSpriteFrameName(gameMetaData::arrMsSpriteName[magicEnum]);
}

//random pick magicStone
magicStone * gameRoomObjLayer::pickAStone(const int stateEnum)
{
	if (isAllUsed())
	{
		checkArrStones();
		std::cout << "All MagicStone is used" << std::endl;
		//뽑을 카드가 없을 경우 예외설정
		//1. 모두 사용됬을 경우
		//2. 초기화가 안되어서 분배에 실패했을 경우
		return nullptr;
	}

	magicStone *ptempMS;
	int rndIndex = 0;
	while (true)
	{
		rndIndex = getRandomIndex();
		if (arrStones[rndIndex]->getStatus() == stateEnum)
		{
			ptempMS = arrStones[rndIndex];
			break;
		}
	}
	return ptempMS;
}

bool gameRoomObjLayer::isAllUsed() const
{
	for (auto const &i : arrStones)
	{
		if (i->getStatus() == gameMetaData::msStatus::notUse)
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
		std::cout << i->getMagic() << " : " << i->getStatus() << std::endl;
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
