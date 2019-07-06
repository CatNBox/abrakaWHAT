#include "gameRoomObjLayer.h"
#include "gameObject\magicStone.h"
#include "gameObject\player.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"
#include "managers/networkManager.h"
#include <iostream>

using namespace cocos2d;

bool gameRoomObjLayer::init(gameMetaData::gameMode modeFlag, int playerTurnOrder[])
{
	if (!Layer::init())
	{
		return false;
	}

	curGameMode = modeFlag;
	curProgressStage = gameMetaData::gameProgressStage::loadingGameRoomScene;
	std::cout << "******" << std::endl
		<< "init() curProgressStage changed loadingGameRoomScene" << std::endl
		<< "******" << std::endl;

	sprManager = new spriteManager;
	actManager = actionManager::getInstance();
	actManager->initCnt();
	netManager = networkManager::getInstance();
	cocosDirector = Director::getInstance();

	//set idx in order to array e.g : array[order] = idx
	for (int i = 0; i < gameMetaData::defaultPlayerCnt; i++)
	{
		int bufIdx = playerTurnOrder[i] - 1;
		arrPlayerIdxInOrder[bufIdx] = i;
	}

	if (curGameMode == gameMetaData::gameMode::single)
	{
		myPlayerIdx = 0;
	}
	else
	{
		auto myId = netManager->getMyClientId();
		myPlayerIdx = myId;
	}
	myTurnOrder = playerTurnOrder[myPlayerIdx] - 1;
	curOrder = 0;
	starterIdx = arrPlayerIdxInOrder[curOrder];

	//eventListener setting
	settingEventListener();

	//countOption value setting
	settingCntValues();

	//create scoreSprite
	createScoreSpr();

	//create players
	createPlayers();

	//create magicStones
	createMagicStones();

	//create seenCheck
	createSeenChecker();

	//create playerLpObj
	createPlayerLpObj();

	//init round setting
	if (curGameMode == gameMetaData::gameMode::single)
	{
		this->scheduleOnce([=](float d) {initRound(); }, 1.0f, "schedulerKey");
	}
	else
	{
		netManager->requestGameRoomSceneReady();
		//this->scheduleUpdate();
		runUpdate();
	}

	return true;
}

void gameRoomObjLayer::delay01secCallWrapper(void(gameRoomObjLayer::* targetFunc)(void))
{
	this->scheduleOnce([=](float d)
	{
		(this->*targetFunc)();
	}, 0.1f, "delayedCallObjLayer");
}

void gameRoomObjLayer::settingEventListener()
{
	uiListener = EventListenerCustom::create("initRound",
		CC_CALLBACK_0(gameRoomObjLayer::initRound, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);

	uiListener = EventListenerCustom::create("checkOwnedMagic",
		CC_CALLBACK_1(gameRoomObjLayer::checkOwnedMagic, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);

	uiListener = EventListenerCustom::create("requestCheckOwnedMagic",
		CC_CALLBACK_1(gameRoomObjLayer::requestCheckOwnedMagic, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);

	uiListener = EventListenerCustom::create("netmodeCheckOwnedMagic",
		CC_CALLBACK_0(gameRoomObjLayer::netmodeCheckOwnedMagic, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);

	uiListener = EventListenerCustom::create("netmodePassTurn",
		CC_CALLBACK_0(gameRoomObjLayer::netmodePassTurn, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(uiListener, this);
}

void gameRoomObjLayer::settingCntValues()
{
	playerCnt = GetPrivateProfileInt(L"RoundOption", L"playerCnt", gameMetaData::defaultPlayerCnt, L".\\option.ini");
	secretCnt = GetPrivateProfileInt(L"RoundOption", L"secretCnt", gameMetaData::defaultSecretCnt, L".\\option.ini");
	maxLifePoint = GetPrivateProfileInt(L"RoundOption", L"maxLifePoint", gameMetaData::defaultMaxLifePoint, L".\\option.ini");

	arrMsCnt[gameMetaData::msType::yongyong] =	GetPrivateProfileInt(L"RoundOption", L"yongCnt", gameMetaData::defaultYongCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::bangrang] =	GetPrivateProfileInt(L"RoundOption", L"bangrangCnt", gameMetaData::defaultBangrangCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::wind]=		GetPrivateProfileInt(L"RoundOption", L"windCnt", gameMetaData::defaultWindCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::booung] =	GetPrivateProfileInt(L"RoundOption", L"booungCnt", gameMetaData::defaultBooungCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::bunpok] =	GetPrivateProfileInt(L"RoundOption", L"bunpokCnt", gameMetaData::defaultBunpokCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::nungang] =	GetPrivateProfileInt(L"RoundOption", L"nungangCnt", gameMetaData::defaultNungangCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::buljak] =	GetPrivateProfileInt(L"RoundOption", L"buljakCnt", gameMetaData::defaultBuljakCnt, L".\\option.ini");
	arrMsCnt[gameMetaData::msType::potion] =	GetPrivateProfileInt(L"RoundOption", L"potionCnt", gameMetaData::defaultPotionCnt, L".\\option.ini");

	stoneMaxCnt = 
		arrMsCnt[gameMetaData::msType::yongyong]	//yongyong 
		+ arrMsCnt[gameMetaData::msType::bangrang]	//bangrang
		+ arrMsCnt[gameMetaData::msType::wind]		//wind
		+ arrMsCnt[gameMetaData::msType::booung]	//booung
		+ arrMsCnt[gameMetaData::msType::bunpok]	//bunpok
		+ arrMsCnt[gameMetaData::msType::nungang]	//nungang
		+ arrMsCnt[gameMetaData::msType::buljak]	//buljak
		+ arrMsCnt[gameMetaData::msType::potion];	//potion

	arrScoreSpr.resize(playerCnt);
	playersList.resize(playerCnt);
	arrStones.resize(stoneMaxCnt);
}

void gameRoomObjLayer::createPlayers()
{
	for (int idx = 0; idx < playersList.size(); idx++)
	{
		player* tempPlayer;
		if (curGameMode == gameMetaData::gameMode::single)
		{
			//single mode
			if (idx == myPlayerIdx)
			{
				tempPlayer = new player();
			}
			else
			{
				tempPlayer = new npc();
			}
		}
		else
		{
			//host/guest mode
			gameMetaData::netPlayerState netPlayerState = netManager->getPlayerConnectionState(idx);
			if (netPlayerState == gameMetaData::netPlayerState::connected)
			{
				tempPlayer = new player();
			}
			else if (netPlayerState == gameMetaData::netPlayerState::guestNPC
				|| netPlayerState == gameMetaData::netPlayerState::hostNPC)
			{
				tempPlayer = new npc();
			}
			else
			{
				//error handling
				assert(true && !"createPlayer()���� netPlayerState�� ���� disconnect �Ǵ� unknown�̾���");
			}
		}
		playersList[idx] = tempPlayer;
	}

	//all npc mode
	if (playersList[myPlayerIdx]->isNPC())
	{
		isMyNumPlayer = false;
	}

	//Each player pointed next/prevPlayer and DefaultPosition setting
	for (int numInOrder = 0; numInOrder < playerCnt; numInOrder++)
	{
		//next/prevPlayer setting
		int next = (numInOrder == playerCnt - 1) ? (0) : (numInOrder + 1);
		int prev = (numInOrder == 0) ? (playerCnt - 1) : (numInOrder - 1);

		int curPlayerIdxOfOrder = arrPlayerIdxInOrder[numInOrder];
		int nextIdxOfOrder = arrPlayerIdxInOrder[next];
		int prevIdxOfOrder = arrPlayerIdxInOrder[prev];

		playersList[curPlayerIdxOfOrder]->setPlayIdx(curPlayerIdxOfOrder);
		playersList[curPlayerIdxOfOrder]->setPlayOrder(numInOrder);
		playersList[curPlayerIdxOfOrder]->setNextPlayer(playersList[nextIdxOfOrder]);
		playersList[curPlayerIdxOfOrder]->setPrevPlayer(playersList[prevIdxOfOrder]);

		//defaultPos setting
		int defaultX = 384;
		int defaultY = 0;
		float rotValue = 0;

		//comp order to setting pos
		if (numInOrder == myTurnOrder)	//when multiPlay, it's center Position
		{
			defaultY = 160;
			arrScoreSpr.at(curPlayerIdxOfOrder)->setPosition(Vec2(660, 130));
		}
		else if (numInOrder == (myTurnOrder + 1) % playerCnt)
		{
			defaultX = 700;
			defaultY = 454;
			rotValue = 90.0f;
			arrScoreSpr.at(curPlayerIdxOfOrder)->setPosition(Vec2(700, 150));
		}
		else if (numInOrder == (myTurnOrder + 2) % playerCnt)
		{
			defaultY = 700;
			arrScoreSpr.at(curPlayerIdxOfOrder)->setPosition(Vec2(660, 170));
		}
		else if (numInOrder == (myTurnOrder + 3) % playerCnt)
		{
			defaultX = 68;
			defaultY = 454;
			rotValue = -90.0f;
			arrScoreSpr.at(curPlayerIdxOfOrder)->setPosition(Vec2(620, 150));
		}
		else
		{
			assert(true && !"createPlayer()���� �߸��� PlayerCnt���� ����");
		}

		std::cout << "player Index : " << curPlayerIdxOfOrder << std::endl
			<< "   default x : " << defaultX << std::endl
			<< "   default y : " << defaultY << std::endl;
		playersList[curPlayerIdxOfOrder]->setDefaultX(defaultX);
		playersList[curPlayerIdxOfOrder]->setDefaultY(defaultY);
		playersList[curPlayerIdxOfOrder]->setRotationValue(rotValue);
	}

	//scoreSpr 620 660 700 130 150 170
}

void gameRoomObjLayer::createMagicStones()
{
	arrStones[0] = new magicStone(gameMetaData::msType::yongyong);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong]] 
		= new magicStone(gameMetaData::msType::bangrang);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]] 
		= new magicStone(gameMetaData::msType::wind);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang] 
		+ arrMsCnt[gameMetaData::msType::wind]] 
		= new magicStone(gameMetaData::msType::booung);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang] 
		+ arrMsCnt[gameMetaData::msType::wind] 
		+ arrMsCnt[gameMetaData::msType::booung]] 
		= new magicStone(gameMetaData::msType::bunpok);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]
		+ arrMsCnt[gameMetaData::msType::wind]
		+ arrMsCnt[gameMetaData::msType::booung]
		+ arrMsCnt[gameMetaData::msType::bunpok]] 
		= new magicStone(gameMetaData::msType::nungang);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]
		+ arrMsCnt[gameMetaData::msType::wind]
		+ arrMsCnt[gameMetaData::msType::booung]
		+ arrMsCnt[gameMetaData::msType::bunpok]
		+ arrMsCnt[gameMetaData::msType::nungang]] 
		= new magicStone(gameMetaData::msType::buljak);

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]
		+ arrMsCnt[gameMetaData::msType::wind]
		+ arrMsCnt[gameMetaData::msType::booung]
		+ arrMsCnt[gameMetaData::msType::bunpok]
		+ arrMsCnt[gameMetaData::msType::nungang]
		+ arrMsCnt[gameMetaData::msType::buljak]] 
		= new magicStone(gameMetaData::msType::potion);

	int prototypeIndex = 0;
	for (int i = 0; i < (int)arrStones.size(); i++)
	{
		if (arrStones[i] != nullptr)
		{
			prototypeIndex = i;
		}
		else
		{
			arrStones[i] = arrStones[prototypeIndex]->clone();
		}
		this->addChild(arrStones[i], gameMetaData::layerZOrder::objZ2);
	}
}

void gameRoomObjLayer::createSeenChecker()
{
	seenChecker.resize(gameMetaData::variableMaxCnt::msTypeCnt);
	//205,400 / 16*32 / y+40 x+23 / +35
	Sprite* seenMs;
	for (int msNum = gameMetaData::msType::yongyong; msNum < gameMetaData::variableMaxCnt::msTypeCnt; msNum++)
	{
		//seenChecker default axis
		int defaultX = 245, defaultY = 360, recalibX = 23, recalibY = 40;
		if (msNum > 4)
			defaultX = defaultX + recalibX * 3 + 35;

		std::vector<std::pair<Sprite*,bool>> tempVector;

		//create seenSprite and addChild, push to vector
		for (int seenObjNum = 0; seenObjNum < arrMsCnt[msNum]; seenObjNum++)
		{
			seenMs = Sprite::createWithSpriteFrameName(gameMetaData::arrNotSeenSpriteName[msNum]);

			seenMs->setPosition(Vec2(defaultX + seenObjNum * recalibX, defaultY + ((msNum-1) % 4) * recalibY));
			this->addChild(seenMs,gameMetaData::layerZOrder::objZ0);
			std::pair<Sprite*, bool> tempPair(seenMs,false);
			tempVector.push_back(tempPair);
		}
		seenChecker[msNum] = tempVector;
	}
}

void gameRoomObjLayer::createPlayerLpObj()
{
	//player have info of LpSprite
	for (auto &i : playersList)
	{
		for (int j = 0; j < maxLifePoint; j++)
		{
			auto tempLpObj = i->createLpObj();
			this->addChild(tempLpObj, gameMetaData::layerZOrder::objZ1);
		}
	}
}

void gameRoomObjLayer::createScoreSpr()
{
	for (int i = 0; i < 4; i++)
	{
		auto tempSpr = Sprite::createWithSpriteFrameName("spr_number.png");
		tempSpr->setVisible(false);
		tempSpr->setScale(SCORE_BASESCALE);
		arrScoreSpr.at(i) = tempSpr;
		this->addChild(tempSpr, gameMetaData::layerZOrder::objZ1);
	}
}

void gameRoomObjLayer::update(float dTime)
{
	//update function only run on multiplay guestMode/hostMode
	//check networkManager stage
	if (curProgressStage == gameMetaData::gameProgressStage::loadingGameRoomScene)
	{
		if (netManager->isAllPlayerReady())
		{
			initRound();
		}
	}
	else if (curProgressStage == gameMetaData::gameProgressStage::waitRoundNetData)
	{
		std::cout << "in waitRoundNetData" << std::endl;
		if (netManager->getNetworkProgressStage() == gameMetaData::gameProgressStage::roundNetDataReady)
		{
			std::cout << "in roundNetDataReady" << std::endl;
			netManager->getInitRoundData(bufSecretDeck, bufPlayer1Hand, bufPlayer2Hand, bufPlayer3Hand, bufPlayer4Hand);

			selectSecretStonebyNetData();
			distributeStoneByNetdata();

			curProgressStage = gameMetaData::gameProgressStage::roundSetReady;
			std::cout << "******" << std::endl
				<< "update() curProgressStage changed roundSetReady" << std::endl
				<< "******" << std::endl;

			if (curGameMode == gameMetaData::gameMode::host && playersList[curPlayerIdx]->isNPC())
			{
				std::cout << "**** start by npc : " << curPlayerIdx << std::endl;
				this->scheduleOnce([=](float d)
				{
					startGameByNpc();
				}, 7.0f, "starterIsNpc");
			}
			else if (curPlayerIdx == myPlayerIdx)
			{
				EventCustom myTurnEvent("myTurn");
				Director::getInstance()->getEventDispatcher()->dispatchEvent(&myTurnEvent);
			}
			else
			{
				curProgressStage = gameMetaData::gameProgressStage::requestCheckOwnedMagic;
				std::cout << "******" << std::endl
					<< "update() curProgressStage changed requestCheckOwnedMagic" << std::endl
					<< "******" << std::endl;
			}
		}
	}
	else if (curProgressStage == gameMetaData::gameProgressStage::requestCheckOwnedMagic)
	{
		if (netManager->getNetworkProgressStage() == gameMetaData::gameProgressStage::requestCheckOwnedMagic)
		{
			curProgressStage = gameMetaData::gameProgressStage::takeMagicResult;
			std::cout << "******" << std::endl
				<< "update() curProgressStage changed takeMagicResult" << std::endl
				<< "******" << std::endl;
			netmodeCheckOwnedMagic();
		}
	}
	else if (curProgressStage == gameMetaData::gameProgressStage::requestRefillHand)
	{
		if (netManager->getNetworkProgressStage() == gameMetaData::gameProgressStage::requestRefillHand)
		{
			curProgressStage = gameMetaData::gameProgressStage::takeRefillResult;
			std::cout << "******" << std::endl
				<< "update() curProgressStage changed takeRefillResult" << std::endl
				<< "******" << std::endl;
			netmodePassTurn();
		}
	}
}

void gameRoomObjLayer::runUpdate()
{
	this->schedule(schedule_selector(gameRoomObjLayer::update), 0.2f, CC_REPEAT_FOREVER, 1.0f);
}

void gameRoomObjLayer::startGameByNpc()
{
	auto actCnt = actManager->getRunningActionCnt();
	std::cout << "actCnt : " << actCnt << std::endl;
	if (actCnt == 0)
	{
		((npc*)playersList[starterIdx])->npcTurnOn();
		callNpcProcess();
	}
	else
	{
		this->scheduleOnce([=](float d)
		{
			startGameByNpc();
		}, 0.5f, "starterIsNpc");
	}
}

gameRoomObjLayer * gameRoomObjLayer::createWithParam(gameMetaData::gameMode modeFlag, int playerTurnOrder[])
{
	gameRoomObjLayer *pRet = new(std::nothrow) gameRoomObjLayer();
	if (pRet && pRet->init(modeFlag, playerTurnOrder))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void gameRoomObjLayer::initRound()
{
	//0.init each Object
	//init magicStones
	for (auto &msObj : arrStones)
	{
		msObj->initObjData();
	}
	//init Players and update Score Sprite
	for (int playerIdx = 0; playerIdx < playerCnt; playerIdx++)
	{
		playersList[playerIdx]->init();
		arrScoreSpr.at(playerIdx)->setTextureRect(sprManager->getNumSprRect(arrScore.at(playerIdx)));
		arrScoreSpr.at(playerIdx)->setVisible(true);
	}
	//init seenChecker
	for (int msNum = 1; msNum < gameMetaData::variableMaxCnt::msTypeCnt; msNum++)
	{
		for (auto &elemSeenChecker : seenChecker[msNum])
		{
			elemSeenChecker.first->initWithSpriteFrameName(gameMetaData::arrNotSeenSpriteName[msNum]);
			elemSeenChecker.second = false;
		}
	}
	//init buf4RoundEndPopUp
	buf4RoundEndPopUp.fill(0);
	
	//display RoundCnt
	EventCustom roundUpEvent("roundUp");
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&roundUpEvent);

	//first player setting
	curOrder = 0;
	curPlayerIdx = arrPlayerIdxInOrder[curOrder];

	//init magicStone by mode
	if (curGameMode == gameMetaData::gameMode::single)
	{
		//1. select Secret Stone
		selectSecretStone();

		//2. share magicStones to each player
		distributeStone2Player();

		if (playersList[curPlayerIdx]->isNPC())
		{
			std::cout << "**** start by npc : " << curPlayerIdx << std::endl;
			this->scheduleOnce([=](float d)
			{
				startGameByNpc();
			}, 7.0f, "starterIsNpc");
		}
		else if (curPlayerIdx == myPlayerIdx)
		{
			EventCustom myTurnEvent("myTurn");
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&myTurnEvent);
		}
	}
	else if (curGameMode == gameMetaData::gameMode::host)
	{
		//1. select secret stone and set buffer for packet to server
		for (int deckIdx = 0; deckIdx < secretCnt; deckIdx++)
		{
			//get enum picked stone
			bufSecretDeck[deckIdx] = netmodePickAStone();
		}

		//2. select player hand stone and set buffer for packet to server
		for (int handIdx = 0; handIdx < 5; handIdx++)
		{
			//get enum picked stone
			bufPlayer1Hand[handIdx] = netmodePickAStone();
			bufPlayer2Hand[handIdx] = netmodePickAStone();
			bufPlayer3Hand[handIdx] = netmodePickAStone();
			bufPlayer4Hand[handIdx] = netmodePickAStone();
		}

		//3. post stone list to server
		netManager->requestHostSetRound(bufSecretDeck, bufPlayer1Hand, bufPlayer2Hand, bufPlayer3Hand, bufPlayer4Hand);

		//4. start loop update function
		curProgressStage = gameMetaData::gameProgressStage::waitRoundNetData;
		std::cout << "******" << std::endl
			<< "initRound() curProgressStage changed waitRoundNetData" << std::endl
			<< "******" << std::endl;
	}
	else if (curGameMode == gameMetaData::gameMode::guest)
	{
		//start loop update function for get network data
		curProgressStage = gameMetaData::gameProgressStage::waitRoundNetData;
		std::cout << "******" << std::endl
			<< "initRound() curProgressStage changed waitRoundNetData" << std::endl
			<< "******" << std::endl;
	}
	else
	{
		//error handling
	}
}

void gameRoomObjLayer::selectSecretStone()
{
	//add secretStone to list
	int preDelayCnt = 0;
	for (int i = 0; i < secretCnt; i++)
	{
		magicStone *secretTemp;
		secretTemp = pickAStone(gameMetaData::msState::notUse);
		if (secretTemp == nullptr)
		{
			//����â �˾� �� ����
			break;
		}

		secretTemp->setState(gameMetaData::msState::secret);
		secretTemp->setBaseSprite();
		//Draw and MoveAction
		auto targetPos = Vec2(310 + i * 50, 530);
		secretTemp->setScale(SECRETSTONE_BASESCALE);
		secretTemp->setLocalZOrder(gameMetaData::layerZOrder::objZ0);
		secretTemp->actionMove(0.2f * preDelayCnt, targetPos, gameMetaData::msMovement::sharing);
		preDelayCnt++;
	}
}

void gameRoomObjLayer::distributeStone2Player()
{
	int preDelayCnt = 0;
	int tempDrawCnt = playerCnt * 5;
	for (int i = 0; i < tempDrawCnt; i++)
	{
		magicStone *tempSelStone;
		//pick magic stone
		tempSelStone = pickAStone(gameMetaData::msState::notUse);
		if (tempSelStone == nullptr)
		{
			//����â �˾� �� ����
			break;
		}

		//Distributing by player order
		int curOrder = i % playerCnt;
		int tempCurPlayerIdx = arrPlayerIdxInOrder[curOrder];
		playersList[tempCurPlayerIdx]->pushStone2List(tempSelStone);
		if ((isMyNumPlayer) && (tempCurPlayerIdx == myPlayerIdx))
		{
			tempSelStone->setBaseSprite();
		}
		//Draw and MoveAction
		int defaultX = playersList[tempCurPlayerIdx]->getDefaultX();
		int defaultY = playersList[tempCurPlayerIdx]->getDefaultY();

		int revisionValue = getMsPosRevision(5, i / 4);
		float rotValue = playersList[tempCurPlayerIdx]->getRotationValue();
		auto tempTargetVec = Vec2(defaultX + revisionValue, defaultY);
		auto tempStandardValue = (myTurnOrder + 1) % 2;
		if (i % 2 == tempStandardValue)
		{
			tempTargetVec.setPoint(defaultX, defaultY + revisionValue);
		}
		tempSelStone->setRotation(rotValue);
		tempSelStone->actionMove(0.2f * (secretCnt + preDelayCnt), tempTargetVec, gameMetaData::msMovement::sharing);
		preDelayCnt++;
	}
}

void gameRoomObjLayer::selectSecretStonebyNetData()
{
	//add secretStone to list
	int preDelayCnt = 0;
	for (int i = 0; i < secretCnt; i++)
	{
		magicStone *secretTemp;
		gameMetaData::msState targetState;
		if (curGameMode == gameMetaData::gameMode::host)
		{
			targetState = gameMetaData::msState::preserve;
		}
		else
		{
			targetState = gameMetaData::msState::notUse;
		}
		secretTemp = pickAStone(targetState, bufSecretDeck[i]);
		if (secretTemp == nullptr)
		{
			//����â �˾� �� ����
			break;
		}

		secretTemp->setState(gameMetaData::msState::secret);
		secretTemp->setBaseSprite();
		//Draw and MoveAction
		auto targetPos = Vec2(310 + i * 50, 530);
		secretTemp->setScale(SECRETSTONE_BASESCALE);
		secretTemp->setLocalZOrder(gameMetaData::layerZOrder::objZ0);
		secretTemp->actionMove(0.2f * preDelayCnt, targetPos, gameMetaData::msMovement::sharing);
		preDelayCnt++;
	}
}

void gameRoomObjLayer::distributeStoneByNetdata()
{
	//pick target State
	gameMetaData::msState targetState;
	if (curGameMode == gameMetaData::gameMode::host)
	{
		targetState = gameMetaData::msState::preserve;
	}
	else
	{
		targetState = gameMetaData::msState::notUse;
	}

	int preDelayCnt = 0;
	int tempDrawCnt = playerCnt * gameMetaData::defaultHandCnt;
	for (int i = 0; i < tempDrawCnt; i++)
	{
		short *pHandArrPointer;
		int curOrder = i % playerCnt;
		if (curOrder == 1)
			pHandArrPointer = bufPlayer1Hand;
		else if (curOrder == 2)
			pHandArrPointer = bufPlayer2Hand;
		else if (curOrder == 3)
			pHandArrPointer = bufPlayer3Hand;
		else if (curOrder == 0)
			pHandArrPointer = bufPlayer4Hand;

		magicStone *tempSelStone;
		//pick magic stone
		int tempHandIdx = i / 4;
		tempSelStone = pickAStone(targetState, pHandArrPointer[tempHandIdx]);
		if (tempSelStone == nullptr)
		{
			//����â �˾� �� ����
			break;
		}

		//Distributing by player order
		int tempCurPlayerIdx = arrPlayerIdxInOrder[curOrder];
		playersList[tempCurPlayerIdx]->pushStone2List(tempSelStone);
		if ((isMyNumPlayer) && (tempCurPlayerIdx == myPlayerIdx))
		{
			tempSelStone->setBaseSprite();
		}
		//Draw and MoveAction
		int defaultX = playersList[tempCurPlayerIdx]->getDefaultX();
		int defaultY = playersList[tempCurPlayerIdx]->getDefaultY();

		int revisionValue = getMsPosRevision(5, i / 4);
		float rotValue = playersList[tempCurPlayerIdx]->getRotationValue();
		auto tempTargetVec = Vec2(defaultX + revisionValue, defaultY);
		auto tempStandardValue = (myTurnOrder + 1) % 2;
		if (i % 2 == tempStandardValue)
		{
			tempTargetVec.setPoint(defaultX, defaultY + revisionValue);
		}
		tempSelStone->setRotation(rotValue);
		tempSelStone->actionMove(0.2f * (secretCnt + preDelayCnt), tempTargetVec, gameMetaData::msMovement::sharing);
		preDelayCnt++;
	}
}

void gameRoomObjLayer::callNpcProcess()
{
	for (int msNum = gameMetaData::msType::yongyong; msNum < gameMetaData::variableMaxCnt::msTypeCnt; msNum++)
	{
		int discardCnt = 0;
		for (auto seenObj : seenChecker[msNum])
		{
			if (seenObj.second == true)
				discardCnt++;
		}
		((npc*)playersList[curPlayerIdx])->setDiscardCnt(msNum, discardCnt);
	}
	auto tempGameMode = curGameMode;
	this->schedule([=](float d) {
		this->unscheduleAllCallbacks();
		((npc*)playersList[curPlayerIdx])->npcProcess(tempGameMode);
	}, 0.0f, 0, 1.0f, "npcPass");
}

//----checkEvent owned magicStone
void gameRoomObjLayer::checkOwnedMagic(EventCustom* checkOwnedMagicEvent)
{
	//std::cout << "check Event in" << std::endl;
	const int magicEnum = (int)(checkOwnedMagicEvent->getUserData());
	//std::cout << "checkOwnedMagic Event activate : " << magicEnum << std::endl;

	//----current player pointer
	auto curPlayer = playersList[curPlayerIdx];
	bool isCorrectChoice = false;

	//when choose pass
	if (magicEnum == gameMetaData::msType::pass)
	{
		//std::cout << "passTurn : " << magicEnum << std::endl;
		passTurn();
		return;
	}
	//check and activate selected magicStone
	else if (curPlayer->checkOutMagic(magicEnum))
	{
		//std::cout << "magic activate : " << magicEnum << std::endl;
		isCorrectChoice = true;

		//----activateMagic
		int damage = 1;
		if (magicEnum == gameMetaData::msType::yongyong || magicEnum == gameMetaData::msType::wind)
		{
			damage = inlineFunc::getRandomInt(1, 3);
		}
		activateMagic(magicEnum, damage);

		//----seenChecker update and effect action
		updateSeenChecker(magicEnum);
		
		//----reorder player's hand
		reorderPlayerHand();
	}
	//not exist selected magicStone
	else
	{
		//std::cout << "not exist : " << magicEnum << std::endl;
		isCorrectChoice = false;

		//----action checkFail
		int damage = 1;
		if (magicEnum == gameMetaData::msType::yongyong)
			damage = inlineFunc::getRandomInt(1,3);
		curPlayer->actionLostLp(damage);
		if (curPlayer->isNPC())
		{
			((npc*)curPlayer)->choiceFail(magicEnum);
		}
	}

	if (isRoundEnd())
	{
		//calc score ending round
		calcScore();

		//checkEndGame - call gameEnd popup eventName : popupEndGame
		for (const auto elemScore : arrScore)
		{
			if (elemScore >= 8)
			{
				EventCustom popupEndRound("popupEndGame");
				popupEndRound.setUserData(&arrScore);
				Director::getInstance()->getEventDispatcher()->dispatchEvent(&popupEndRound);
				return;
			}
		}

		//call roundEnd popup eventName : popupEndRound
		delay01secCallWrapper(&gameRoomObjLayer::callEndRoundEvent);

		return;
	}
	
	if(isCorrectChoice)
	{
		if (curPlayer->isNPC())
		{
			callNpcProcess();
		}
		else //not NPC
		{
			EventCustom callBackEvent("choicerUIEnabled");
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&callBackEvent);
		}
	}
	else
	{
		passTurn();
	}
}

void gameRoomObjLayer::requestCheckOwnedMagic(cocos2d::EventCustom * checkOwnedMagicEvent)
{
	//call request to server
	const short magicEnum = (short)(checkOwnedMagicEvent->getUserData());
	int damageValue = 1;
	if (magicEnum == gameMetaData::msType::yongyong || magicEnum == gameMetaData::msType::wind)
	{
		damageValue = inlineFunc::getRandomInt(1, 3);
	}

	curProgressStage = gameMetaData::gameProgressStage::requestCheckOwnedMagic;
	std::cout << "******" << std::endl
		<< "requestCheckOwnedMagic() curProgressStage changed requestCheckOwnedMagic" << std::endl
		<< "******" << std::endl;
	netManager->requestCheckOwnedMagic(magicEnum, curPlayerIdx, damageValue);
	runUpdate();
}

void gameRoomObjLayer::netmodeCheckOwnedMagic()
{
	short pickedMagicEnum = 9;
	int curTurnPlayerIdx = -1;
	int damageValue = 1;

	netManager->getPickedMagicNetData(&pickedMagicEnum, &curTurnPlayerIdx, &damageValue);
	if (curTurnPlayerIdx > 3 && curTurnPlayerIdx < 0)
	{
		//error handling
		return;
	}
	else if (curTurnPlayerIdx != curPlayerIdx)
	{
		//error handling
		return;
	}

	//----current player pointer
	auto curPlayer = playersList[curPlayerIdx];
	bool isCorrectChoice = false;

	//when choose pass
	if (pickedMagicEnum == gameMetaData::msType::pass)
	{
		if (curPlayerIdx == myPlayerIdx)
		{
			requestRefillHand(curPlayer);
		}
		else
		{
			curProgressStage = gameMetaData::gameProgressStage::requestRefillHand;
			std::cout << "******" << std::endl
				<< "netmodeCheckOwnedMagic() curProgressStage changed requestRefillHand" << std::endl
				<< "******" << std::endl;
		}
		return;
	}
	//check and activate selected magicStone
	else if (curPlayer->checkOutMagic((int)pickedMagicEnum))
	{
		//std::cout << "magic activate : " << magicEnum << std::endl;
		isCorrectChoice = true;

		//----activateMagic
		activateMagic((int)pickedMagicEnum, damageValue);

		//----seenChecker update and effect action
		updateSeenChecker((int)pickedMagicEnum);

		//----reorder player's hand
		reorderPlayerHand();
	}
	//not exist selected magicStone
	else
	{
		//std::cout << "not exist : " << magicEnum << std::endl;
		isCorrectChoice = false;

		//----action checkFail
		if (pickedMagicEnum == gameMetaData::msType::wind)
		{
			damageValue = 1;
		}
		curPlayer->actionLostLp(damageValue);

		//npc control
		if (curGameMode == gameMetaData::gameMode::host)
		{
			if (curPlayer->isNPC())
			{
				((npc*)curPlayer)->choiceFail((int)pickedMagicEnum);
			}
		}
	}

	if (isRoundEnd())
	{
		//calc score ending round
		calcScore();

		//checkEndGame - call gameEnd popup eventName : popupEndGame
		for (const auto elemScore : arrScore)
		{
			if (elemScore >= 8)
			{
				EventCustom popupEndRound("popupEndGame");
				popupEndRound.setUserData(&arrScore);
				Director::getInstance()->getEventDispatcher()->dispatchEvent(&popupEndRound);
				return;
			}
		}

		//call roundEnd popup eventName : popupEndRound
		delay01secCallWrapper(&gameRoomObjLayer::callEndRoundEvent);

		return;
	}

	if (isCorrectChoice)
	{
		if (curPlayer->isNPC() && curGameMode == gameMetaData::gameMode::host)
		{
			callNpcProcess();
		}
		else if (curPlayerIdx == myPlayerIdx)
		{
			EventCustom callBackEvent("choicerUIEnabled");
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&callBackEvent);
		}
		else// if (curGameMode == gameMetaData::gameMode::guest)
		{
			curProgressStage = gameMetaData::gameProgressStage::requestCheckOwnedMagic;
			std::cout << "******" << std::endl
				<< "netmodeCheckOwnedMagic() curProgressStage changed requestCheckOwnedMagic" << std::endl
				<< "******" << std::endl;
		}
	}
	else
	{
		if (curPlayer->isNPC())
		{
			if (curGameMode == gameMetaData::gameMode::host)
			{
				requestRefillHand(curPlayer);
			}
			else if(curGameMode == gameMetaData::gameMode::guest)
			{
				curProgressStage = gameMetaData::gameProgressStage::requestRefillHand;
				std::cout << "******" << std::endl
					<< "netmodeCheckOwnedMagic() curProgressStage changed requestRefillHand" << std::endl
					<< "******" << std::endl;
			}
		}
		else
		{
			if (curPlayerIdx == myPlayerIdx)
			{
				requestRefillHand(curPlayer);
			}
			else
			{
				curProgressStage = gameMetaData::gameProgressStage::requestRefillHand;
				std::cout << "******" << std::endl
					<< "netmodeCheckOwnedMagic() curProgressStage changed requestRefillHand" << std::endl
					<< "******" << std::endl;
			}
		}
	}
}

void gameRoomObjLayer::activateMagic(const int magicEnum, const int damage)
{
	auto currentPlayer = playersList[curPlayerIdx];
	switch (magicEnum)
	{
	case gameMetaData::msType::yongyong:
	{
		auto redScreenSpr = cocos2d::Sprite::createWithSpriteFrameName("yongyongRedScreen.png");
		redScreenSpr->setScale(1.1f);
		redScreenSpr->setPosition(STDAXIS, STDAXIS);
		addChild(redScreenSpr, gameMetaData::layerZOrder::effectZ);
		auto blinking = cocos2d::Blink::create(2.0f, 8);
		auto callred = cocos2d::CallFunc::create([=]() {redScreenSpr->setVisible(false);});
		auto seqRed = actManager->wrapActions4Cnt(blinking, callred, NULL);
		redScreenSpr->runAction(seqRed);

		auto shaking01 = cocos2d::MoveBy::create(0.1f, Vec2(10, 10));	//15,15
		auto shaking02 = cocos2d::MoveBy::create(0.1f, Vec2(-20, -10));	//-15,0
		auto shaking03 = cocos2d::MoveBy::create(0.1f, Vec2(20, -10));	//15,-15
		auto shaking04 = cocos2d::MoveBy::create(0.1f, Vec2(-10, 20));	//0,15
		auto shaking05 = cocos2d::MoveBy::create(0.1f, Vec2(-10, -20));	//-15,-15
		auto shaking06 = cocos2d::MoveBy::create(0.1f, Vec2(20, 10));	//15,0
		auto shaking07 = cocos2d::MoveBy::create(0.1f, Vec2(-20, 10));	//-15,15
		auto shaking08 = cocos2d::MoveBy::create(0.1f, Vec2(10, -10));	//0,-15
		auto shaking09 = cocos2d::MoveBy::create(0.1f, Vec2(0, 10)); //0, 0
		auto seq01 = cocos2d::Sequence::create(shaking01, shaking02, shaking03, 
			shaking04, shaking05, shaking06, shaking07, shaking08, shaking09, NULL);
		auto reverseAction = seq01->reverse();
		//auto seq02 = cocos2d::Sequence::create(seq01, reverseAction, NULL);
		auto seqEarthQuake = actManager->wrapActions4Cnt(seq01, reverseAction, NULL);
		this->runAction(seqEarthQuake);
	}
	case gameMetaData::msType::bangrang:
	{
		//----yongyong && bangrang damage action
		for (auto &i : playersList)
		{
			if (i == currentPlayer)
				continue;

			i->actionLostLp(damage);
		}
		if (magicEnum == gameMetaData::msType::bangrang)
			currentPlayer->actionGainLp(magicEnum);
		return;
	}
	case gameMetaData::msType::bunpok:
	case gameMetaData::msType::nungang:
	case gameMetaData::msType::buljak:
	{
		//-----bunpok, nungang, buljak damage action
		if (magicEnum != gameMetaData::msType::buljak)
			currentPlayer->getPrevPlayer()->actionLostLp(damage);

		if (magicEnum != gameMetaData::msType::nungang)
			currentPlayer->getNextPlayer()->actionLostLp(damage);

		return;
	}
	case gameMetaData::msType::wind:
	case gameMetaData::msType::potion:
	{
		currentPlayer->actionGainLp(magicEnum, damage);
		return;
	}
	case gameMetaData::msType::booung:
	{
		auto tempMs = pickAStone(gameMetaData::msState::secret);
		if (tempMs == nullptr)
		{
			//�˾�â ����ó��
		}
		currentPlayer->pushbooung2List(tempMs);
		if (curPlayerIdx == myPlayerIdx)
			tempMs->actionRevealedSecret();
		return;
	}
	default:
		assert(magicEnum || " : �߸��� magicEnum�� activateMagic�� �Էµ�");
		return;
	}
}

void gameRoomObjLayer::updateSeenChecker(const int magicEnum)
{
	for (auto &i : seenChecker[magicEnum])
	{
		if (i.second == false)
		{
			auto seenCheckEffect = cocos2d::ParticleSystemQuad::create("particle4SeenCheck.plist");
			seenCheckEffect->setPosition(i.first->getPosition());
			this->addChild(seenCheckEffect, gameMetaData::layerZOrder::effectZ);
			i.first->initWithSpriteFrameName(gameMetaData::arrSeenSpriteName[magicEnum]);
			i.second = true;
			break;
		}
	}
}

void gameRoomObjLayer::refillPlayerHand(const int curIdx)
{
	//short drawCnt = 0;
	//int tempCurPlayerIdx = 0;
	netManager->getRefillNetData(bufRefillHand, &bufDrawCnt, &bufCurPlayerIdx);
	
	if (curIdx != bufCurPlayerIdx)
	{
		//error handling
		return;
	}

	auto curPlayer = playersList[curIdx];

	auto tempMsState = gameMetaData::msState::notUse;
	if (curIdx == myPlayerIdx 
		|| (curGameMode == gameMetaData::gameMode::host && curPlayer->isNPC())
		)
	{
		tempMsState = gameMetaData::msState::preserve;
	}

	//refill magicStone
	for (int drawIdx = 0; drawIdx < bufDrawCnt; drawIdx++)
	{
		auto tempMagicStone = pickAStone(tempMsState, bufRefillHand[drawIdx]);
		if (tempMagicStone == nullptr)
		{
			//error handling
			//drawCnt = drawIdx;
			//break;
			return;
		}
		else
		{
			curPlayer->pushStone2List(tempMagicStone);
		}
	}

	if (curPlayer->isNPC())
	{
		((npc&)curPlayer).setNewHandCnt((int)bufDrawCnt);
	}
}

void gameRoomObjLayer::reorderPlayerHand()
{
	auto curPlayer = playersList[curPlayerIdx];
	int playerMsListSize = curPlayer->getStoneListSize();
	for (int i = 0; i < playerMsListSize; i++)
	{
		auto tempMagicStone = curPlayer->getMagicStone(i);
		if ((isMyNumPlayer) && (curPlayerIdx == myPlayerIdx))
		{
			tempMagicStone->setBaseSprite();
		}
		int defaultX = curPlayer->getDefaultX();
		int defaultY = curPlayer->getDefaultY();
		int revisionValue = getMsPosRevision(playerMsListSize, i);
		float rotValue = curPlayer->getRotationValue();
		tempMagicStone->setRotation(rotValue);
		Vec2 tempVec2(defaultX + revisionValue, defaultY);
		if (rotValue != 0)
		{
			tempVec2.setPoint(defaultX, defaultY + revisionValue);
		}
		tempMagicStone->actionMove(0.1f, tempVec2, gameMetaData::msMovement::reordering);
	}
}

bool gameRoomObjLayer::isRoundEnd()
{
	//players's life check. if anyone have zero LPCnt, roundEnd is True
	for (const auto &playerPointer : playersList)
	{
		if (playerPointer->getCurLP() == 0)
			return true;
	}

	//count curPlayer's rest hand
	if (playersList[curPlayerIdx]->getStoneListSize() == 0)
	{
		abrakaWHAT = true;
		return true;
	}

	return false;
}

void gameRoomObjLayer::calcScore()
{
	//roundfinisher
	auto curPlayerOrder = playersList[curPlayerIdx]->getPlayOrder();
	buf4RoundEndPopUp.at(4) = curPlayerOrder;

	//check lp / check booung / if curPlayer's lp is zero, no point
	if (playersList[curPlayerIdx]->getCurLP() > 0)
	{
		arrScore.at(curPlayerIdx) += 2;
		buf4RoundEndPopUp.at(curPlayerOrder) += 2;
	}

	if (abrakaWHAT)
	{
		arrScore.at(curPlayerIdx) += 1;
		buf4RoundEndPopUp.at(curPlayerOrder) += 1;
		abrakaWHAT = false;
		return;
	}

	for (const auto &elemPlayer : playersList)
	{
		//check Lp
		if (elemPlayer->getCurLP() > 0)
		{
			//at least have 1 Lp, get 1 point
			arrScore.at(arrPlayerIdxInOrder[elemPlayer->getPlayOrder()]) += 1;
			buf4RoundEndPopUp.at(elemPlayer->getPlayOrder()) += 1;

			//check booung
			arrScore.at(arrPlayerIdxInOrder[elemPlayer->getPlayOrder()]) += elemPlayer->getBooungListSize();
			buf4RoundEndPopUp.at(elemPlayer->getPlayOrder()) += elemPlayer->getBooungListSize();
		}
	}
}

void gameRoomObjLayer::callEndRoundEvent()
{
	int checkActionCnt = actManager->getRunningActionCnt();
	if (checkActionCnt > 0)
	{
		loopError++;
		if (loopError > 50)
		{
			cocos2d::EventCustom errorWarning("popupWarning");
			errorWarning.setUserData((void*)gameMetaData::warningCode::infinityLoopWarning);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&errorWarning);
			return;
		}
		delay01secCallWrapper(&gameRoomObjLayer::callEndRoundEvent);
	}
	else
	{
		loopError = 0;
		EventCustom popupEndRound("popupEndRound");
		popupEndRound.setUserData(&buf4RoundEndPopUp);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&popupEndRound);
	}
}

void gameRoomObjLayer::passTurn()
{
	//current Player
	auto curPlayer = playersList[curPlayerIdx];

	//magicStone�� ���������� 5���� �ƴϰ� ���������� magicStone�� �ִٸ� �����Ѵ�
	//Draw magicStone
	int curPlayerReqCnt = 5 - curPlayer->getStoneListSize();
	int drawCnt = 0;
	for (int drawIdx = 0; drawIdx < curPlayerReqCnt; drawIdx++)
	{
		auto tempSelStone = pickAStone(gameMetaData::msState::notUse);
		if (tempSelStone == nullptr)
		{
			//not enough notUseMagicStone then just break
			break;
		}
		else
		{
			curPlayer->pushStone2List(tempSelStone);
			drawCnt++;
		}
	}
	if (curPlayer->isNPC())
		((npc*)curPlayer)->setNewHandCnt(drawCnt);

	reorderPlayerHand();

	//update curPlayerNum
	if (curPlayer->isNPC())
	{
		((npc*)curPlayer)->waitTurn();
	}
	curOrder = (curOrder == playerCnt - 1) ? (0) : (curOrder + 1);
	curPlayerIdx = arrPlayerIdxInOrder[curOrder];

	if ((!isMyNumPlayer) || (curPlayerIdx != myPlayerIdx))
	{
		if (playersList[curPlayerIdx]->isNPC())
		{
			((npc*)playersList[curPlayerIdx])->npcTurnOn();
			callNpcProcess();
		}
		EventCustom passTurnEvent("turnOverUIDisabled");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);
	}
	else
	{
		EventCustom passTurnEvent("myTurn");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);
	}
}

void gameRoomObjLayer::netmodePassTurn()
{
	//this function called after networkManager::setRefillNetData

	//current Player
	auto curPlayer = playersList[curPlayerIdx];

	refillPlayerHand(curPlayerIdx);
	reorderPlayerHand();

	//update curPlayerNum
	if ((curGameMode == gameMetaData::gameMode::host) && (curPlayer->isNPC()))
	{
		((npc*)curPlayer)->waitTurn();
	}
	curOrder = (curOrder == playerCnt - 1) ? (0) : (curOrder + 1);
	curPlayerIdx = arrPlayerIdxInOrder[curOrder];

	//next turn
	if ((!isMyNumPlayer) || (curPlayerIdx != myPlayerIdx))
	{
		if (playersList[curPlayerIdx]->isNPC() && curGameMode == gameMetaData::gameMode::host)
		{
			((npc*)playersList[curPlayerIdx])->npcTurnOn();
			callNpcProcess();
		}
		else
		{
			curProgressStage = gameMetaData::gameProgressStage::requestCheckOwnedMagic;
			std::cout << "******" << std::endl
				<< "netmodePassTurn() curProgressStage changed requestCheckOwnedMagic" << std::endl
				<< "******" << std::endl;
		}
		EventCustom passTurnEvent("turnOverUIDisabled");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);
	}
	else
	{
		EventCustom passTurnEvent("myTurn");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);
	}
}

void gameRoomObjLayer::requestRefillHand(player* curPlayer)
{
	//init buffer
	std::fill_n(bufRefillHand, 5, 0);

	//magicStone�� ���������� 5���� �ƴϰ� ���������� magicStone�� �ִٸ� �����Ѵ�
	//Draw magicStone
	int curPlayerReqCnt = 5 - curPlayer->getStoneListSize();
	short drawCnt = 0;
	for (int drawIdx = 0; drawIdx < curPlayerReqCnt; drawIdx++)
	{
		bufRefillHand[drawIdx] = netmodePickAStone();
		if (bufRefillHand[drawIdx] != 0)
		{
			//0 is mean All magicStone is used
			drawCnt++;
		}
	}

	curProgressStage = gameMetaData::gameProgressStage::requestRefillHand;
	std::cout << "******" << std::endl
		<< "requestRefillHand() curProgressStage changed requestRefillHand" << std::endl
		<< "******" << std::endl;
	netManager->requestRefillHand(bufRefillHand, drawCnt, curPlayerIdx);
}

cocos2d::Sprite* gameRoomObjLayer::getMSSprite(const int magicEnum)
{
	return Sprite::createWithSpriteFrameName(gameMetaData::arrMsSpriteName[magicEnum]);
}

//random pick magicStone
magicStone * gameRoomObjLayer::pickAStone(const int stateEnum)
{
	if ((stateEnum == gameMetaData::msState::notUse) && isAllUsed())
	{
		//checkArrStones();
		//std::cout << "All MagicStone is used" << std::endl;
		//���� ī�尡 ���� ��� ���ܼ���
		//1. ��� ������ ���
		//2. �ʱ�ȭ�� �ȵǾ �й迡 �������� ���
		return nullptr;
	}

	magicStone *pTempMS;
	int rndIndex = 0;
	while (true)
	{
		rndIndex = inlineFunc::getRandomInt(stoneMinCnt, stoneMaxCnt - 1);
		if (arrStones[rndIndex]->getStatus() == stateEnum)
		{
			pTempMS = arrStones[rndIndex];
			break;
		}
	}
	return pTempMS;
}

magicStone * gameRoomObjLayer::pickAStone(const int stateEnum, const short magicEnum)
{
	if ((stateEnum == gameMetaData::msState::notUse) && isAllUsed())
	{
		//checkArrStones();
		//std::cout << "All MagicStone is used" << std::endl;
		//���� ī�尡 ���� ��� ���ܼ���
		//1. ��� ������ ���
		//2. �ʱ�ȭ�� �ȵǾ �й迡 �������� ���
		return nullptr;
	}
	/*
	else if ((magicEnum == gameMetaData::msType::base) || (magicEnum == gameMetaData::msType::pass))
	{
		return nullptr;
	}
	*/

	magicStone *pTempMS;
	int rndIndex = 0;
	int magicMax = 0;
	for (int i = 1; i <= magicEnum; i++)
	{
		magicMax += arrMsCnt[i];
	}
	int magicMin = magicMax - magicEnum;
	while (true)
	{
		rndIndex = inlineFunc::getRandomInt(magicMin, magicMax - 1);
		if (arrStones[rndIndex]->getStatus() == stateEnum)
		{
			pTempMS = arrStones[rndIndex];
			break;
		}
	}
	return pTempMS;
}

short gameRoomObjLayer::netmodePickAStone()
{
	magicStone *tempStone;
	tempStone = pickAStone(gameMetaData::msState::notUse);
	if (tempStone == nullptr)
	{
		//error handling
		return gameMetaData::msType::base;
	}
	tempStone->setState(gameMetaData::msState::preserve);

	return tempStone->getMagic();
}

bool gameRoomObjLayer::isAllUsed() const
{
	int secretCnt = 0;
	int ownedCnt = 0;
	for (auto const &elemMagicStone : arrStones)
	{
		auto elemStatus = elemMagicStone->getStatus();
		if (elemStatus == gameMetaData::msState::notUse)
		{
			return false;
		}
	}
	return true;
}

int gameRoomObjLayer::getMsPosRevision(int msListSize, int msOrder)
{
	return 38 * (msListSize - 1 - (msOrder * 2));
}

void gameRoomObjLayer::checkArrStones()
{
	//std::cout << "---Check arrStones---" << std::endl;
	for (auto const &i : arrStones)
	{
		//std::cout << i->getMagic() << " : " << i->getStatus() << std::endl;
	}
	//std::cout << "---------------------" << std::endl;
}

void gameRoomObjLayer::checkArrPlayers()
{
	for (auto const &i : playersList)
	{
		//std::cout << "StoneList size : " << i->getStoneListSize() << std::endl;
		//std::cout << "BooungList size : " << i->getBooungListSize() << std::endl;
	}
}
