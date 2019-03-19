#include "gameRoomObjLayer.h"
#include "gameObject\magicStone.h"
#include "gameObject\player.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"
//#include <iostream>

using namespace cocos2d;

bool gameRoomObjLayer::init(gameMetaData::gameMode modeFlag, int playerTurnOrder[])
{
	if (!Layer::init())
	{
		return false;
	}

	curGameMode = modeFlag;
	myPlayerNum = playerTurnOrder[0] - 1;

	sprManager = new spriteManager;
	actManager = actionManager::getInstance();
	actManager->initCnt();

	//eventListener setting
	settingEventListener();

	//countOption value setting
	settingCntValues();

	//create scoreSprite
	createScoreSpr();

	//create players
	createPlayers(playerTurnOrder);

	//create magicStones
	createMagicStones();

	//create seenCheck
	createSeenChecker();

	//create playerLpObj
	createPlayerLpObj();

	//init round setting
	this->scheduleOnce([=](float d) {initRound();}, 0.7f, "schedulerKey");
	//this->schedule([=](float d) {initRound(); },10.0f,CC_REPEAT_FOREVER,10.0f,"initRepeat");


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
	arrPlayers.resize(playerCnt);
	arrStones.resize(stoneMaxCnt);
}

void gameRoomObjLayer::createPlayers(int playerTurnOrder[])
{
	//현재 내 플레이어 번호를 기준으로 할 것 - 차후 멀티모드 수정
	bool bPlayer = true;
	int idx = 0;
	for (auto &i : arrPlayers)
	{
		player* tempPlayer;
		if (idx == myPlayerNum)
		{
			tempPlayer = new player(idx);
			bPlayer = false;
		}
		else
		{
			tempPlayer = new npc(idx);
		}
		i = tempPlayer;
		idx++;
	}
	if (arrPlayers[myPlayerNum]->isNPC())
	{
		isMyNumPlayer = false;
	}

	//Each player pointed next/prevPlayer and DefaultPosition setting
	for (int i = 0; i < playerCnt; i++)
	{
		//next/prevPlayer setting
		int next = (i == playerCnt - 1) ? (0) : (i + 1);
		arrPlayers[i]->setNextPlayer(arrPlayers[next]);
		int prev = (i == 0) ? (playerCnt - 1) : (i - 1);
		arrPlayers[i]->setPrevPlayer(arrPlayers[prev]);

		//defaultPos setting
		int defaultX = 384;
		int defaultY = 0;
		float rotValue = 0;
		if (i == myPlayerNum)	//when multiPlay, it's center Position
		{
			defaultY = 160;
			arrScoreSpr.at(i)->setPosition(Vec2(660, 130));
		}
		else if (i == (myPlayerNum + 1) % playerCnt)
		{
			defaultX = 700;
			defaultY = 454;
			rotValue = 90.0f;
			arrScoreSpr.at(i)->setPosition(Vec2(700, 150));
		}
		else if (i == (myPlayerNum + 2) % playerCnt)
		{
			defaultY = 700;
			arrScoreSpr.at(i)->setPosition(Vec2(660, 170));
		}
		else if (i == (myPlayerNum + 3) % playerCnt)
		{
			defaultX = 68;
			defaultY = 454;
			rotValue = -90.0f;
			arrScoreSpr.at(i)->setPosition(Vec2(620, 150));
		}
		else
		{
			assert(true && !"createPlayer()에서 잘못된 PlayerCnt값이 들어옴");
		}
		arrPlayers[i]->setDefaultX(defaultX);
		arrPlayers[i]->setDefaultY(defaultY);
		arrPlayers[i]->setRotationValue(rotValue);
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
	int idx = 0;
	for (auto &i : arrPlayers)
	{
		for (int j = 0; j < maxLifePoint; j++)
		{
			auto temp = i->createLpObj(idx + 1);
			this->addChild(temp, gameMetaData::layerZOrder::objZ1);
		}
		idx++;
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

void gameRoomObjLayer::startGameByNpc()
{
	auto actCnt = actManager->getRunningActionCnt();
	if (actCnt == 0)
	{
		((npc*)arrPlayers[starterNum])->npcTurnOn();
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
	for (auto &i : arrStones)
	{
		i->initObjData();
	}
	//init Players and update Score Sprite
	for (auto &i : arrPlayers)
	{
		i->init();
		auto playerIdx = i->getIndex();
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
	EventCustom passTurnEvent("roundUp");
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);

	//1. select Secret Stone
	selSecretStone();

	//2. share magicStones to each player
	shareStone2Player();

	//3. setting firstTurn Player
	//setNextRoundStater();
	starterNum = 0;
	curPlayerNum = 0;

	if (arrPlayers[starterNum]->isNPC())
	{
		this->scheduleOnce([=](float d)
		{
			startGameByNpc();
		}, 8.0f, "starterIsNpc");
	}
	else if(starterNum == myPlayerNum)
	{
		EventCustom myTurnEvent("myTurn");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&myTurnEvent);
	}
}

void gameRoomObjLayer::selSecretStone()
{
	//add secretStone to list
	int preDelayCnt = 0;
	for (int i = 0; i < secretCnt; i++)
	{
		magicStone *secretTemp;
		secretTemp = pickAStone(gameMetaData::msStatus::notUse);
		if (secretTemp == nullptr)
		{
			//오류창 팝업 및 종료
			break;
		}

		secretTemp->setStatus(gameMetaData::msStatus::secret);
		secretTemp->setBaseSprite();
		//Draw and MoveAction
		auto targetPos = Vec2(310 + i * 50, 530);
		secretTemp->setScale(SECRETSTONE_BASESCALE);
		secretTemp->setLocalZOrder(gameMetaData::layerZOrder::objZ0);
		secretTemp->actionMove(0.2f * preDelayCnt, targetPos, gameMetaData::msMovement::sharing);
		preDelayCnt++;
	}
}

void gameRoomObjLayer::shareStone2Player()
{
	int preDelayCnt = 0;
	int tempDrawCnt = playerCnt * 5;
	for (int i = 0; i < tempDrawCnt; i++)
	{
		magicStone *tempSelStone;
		tempSelStone = pickAStone(gameMetaData::msStatus::notUse);
		if (tempSelStone == nullptr)
		{
			//오류창 팝업 및 종료
			break;
		}

		//Sharing by player order
		int tempCurPlayerIdx = i%playerCnt;
		arrPlayers[tempCurPlayerIdx]->pushStone2List(tempSelStone);
		if ((isMyNumPlayer) && (tempCurPlayerIdx == myPlayerNum))
		{
			tempSelStone->setBaseSprite();
		}
		//Draw and MoveAction
		int defaultX = arrPlayers[tempCurPlayerIdx]->getDefaultX();
		int defaultY = arrPlayers[tempCurPlayerIdx]->getDefaultY();
		int revisionValue = getMsPosRevision(5, i / 4);
		float rotValue = arrPlayers[tempCurPlayerIdx]->getRotationValue();
		auto tempTargetVec = Vec2(defaultX + revisionValue, defaultY);
		auto tempStandardValue = (myPlayerNum + 1) % 2;
		if (i % 2 == tempStandardValue)
		{
			tempTargetVec.setPoint(defaultX, defaultY + revisionValue);
		}
		tempSelStone->setRotation(rotValue);
		tempSelStone->actionMove(0.2f * (secretCnt + preDelayCnt), tempTargetVec, gameMetaData::msMovement::sharing);
		preDelayCnt++;
	}
}

void gameRoomObjLayer::setStartOrder()
{
	srand(time(nullptr));
	starterNum = (rand() % playerCnt) + 1;
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
		((npc*)arrPlayers[curPlayerNum])->setDiscardCnt(msNum, discardCnt);
	}
	this->schedule([=](float d) {
		this->unscheduleAllCallbacks();
		((npc*)arrPlayers[curPlayerNum])->npcProcess();
	}, 0.0f, 0, 1.0f, "npcPass");
}

//----checkEvent owned magicStone
void gameRoomObjLayer::checkOwnedMagic(EventCustom* checkOwnedMagicEvent)
{
	//std::cout << "check Event in" << std::endl;
	const int magicEnum = (int)(checkOwnedMagicEvent->getUserData());
	//std::cout << "checkOwnedMagic Event activate : " << magicEnum << std::endl;

	//----current player pointer
	auto curPlayer = arrPlayers[curPlayerNum];
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
		activateMagic(magicEnum);

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

void gameRoomObjLayer::activateMagic(const int magicEnum)
{
	auto currentPlayer = arrPlayers[curPlayerNum];
	int damage = 1;
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

		damage = inlineFunc::getRandomInt(1, 3);
	}
	case gameMetaData::msType::bangrang:
	{
		//----yongyong && bangrang damage action
		for (auto &i : arrPlayers)
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
		currentPlayer->actionGainLp(magicEnum);
		return;
	}
	case gameMetaData::msType::booung:
	{
		auto tempMs = pickAStone(gameMetaData::msStatus::secret);
		if (tempMs == nullptr)
		{
			//팝업창 예외처리
		}
		currentPlayer->pushbooung2List(tempMs);
		if (curPlayerNum == myPlayerNum)
			tempMs->actionRevealedSecret();
		return;
	}
	default:
		assert(magicEnum || " : 잘못된 magicEnum이 activateMagic에 입력됨");
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

void gameRoomObjLayer::reorderPlayerHand()
{
	auto curPlayer = arrPlayers[curPlayerNum];
	int playerMsListSize = curPlayer->getStoneListSize();
	for (int i = 0; i < playerMsListSize; i++)
	{
		auto tempMagicStone = curPlayer->getMagicStone(i);
		if ((isMyNumPlayer) && (curPlayerNum == myPlayerNum))
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
	for (const auto &playerPointer : arrPlayers)
	{
		if (playerPointer->getCurLP() == 0)
			return true;
	}

	//count curPlayer's rest hand
	if (arrPlayers[curPlayerNum]->getStoneListSize() == 0)
	{
		abrakaWHAT = true;
		return true;
	}

	return false;
}

void gameRoomObjLayer::calcScore()
{
	//roundfinisher
	buf4RoundEndPopUp.at(4) = curPlayerNum;

	//check lp / check booung / if curPlayer's lp is zero, no point
	if (arrPlayers[curPlayerNum]->getCurLP() > 0)
	{
		arrScore.at(arrPlayers[curPlayerNum]->getIndex()) += 2;
		buf4RoundEndPopUp.at(arrPlayers[curPlayerNum]->getIndex()) += 2;
	}

	if (abrakaWHAT)
	{
		arrScore.at(arrPlayers[curPlayerNum]->getIndex()) += 1;
		buf4RoundEndPopUp.at(arrPlayers[curPlayerNum]->getIndex()) += 1;
		abrakaWHAT = false;
		return;
	}

	for (const auto &elemPlayer : arrPlayers)
	{
		//check Lp
		if (elemPlayer->getCurLP() > 0)
		{
			//at least have 1 Lp, get 1 point
			arrScore.at(elemPlayer->getIndex()) += 1;
			buf4RoundEndPopUp.at(elemPlayer->getIndex()) += 1;

			//check booung
			arrScore.at(elemPlayer->getIndex()) += elemPlayer->getBooungListSize();
			buf4RoundEndPopUp.at(elemPlayer->getIndex()) += elemPlayer->getBooungListSize();
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
	auto curPlayer = arrPlayers[curPlayerNum];

	//magicStone의 보유개수가 5개가 아니고 사용되지않은 magicStone이 있다면 보충한다
	//Draw magicStone
	int curPlayerReqCnt = 5 - curPlayer->getStoneListSize();
	int drawCnt = 0;
	for (int i = 0; i < curPlayerReqCnt; i++)
	{
		auto tempSelStone = pickAStone(gameMetaData::msStatus::notUse);
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
	curPlayerNum = (curPlayerNum == playerCnt - 1) ? (0) : (curPlayerNum + 1);
	
	if ((!isMyNumPlayer) || (curPlayerNum != myPlayerNum))
	{
		if (arrPlayers[curPlayerNum]->isNPC())
		{
			((npc*)arrPlayers[curPlayerNum])->npcTurnOn();
			callNpcProcess();
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
	if ((stateEnum == gameMetaData::msStatus::notUse) && isAllUsed())
	{
		//checkArrStones();
		//std::cout << "All MagicStone is used" << std::endl;
		//뽑을 카드가 없을 경우 예외설정
		//1. 모두 사용됬을 경우
		//2. 초기화가 안되어서 분배에 실패했을 경우
		return nullptr;
	}

	magicStone *ptempMS;
	int rndIndex = 0;
	while (true)
	{
		rndIndex = inlineFunc::getRandomInt(stoneMinCnt, stoneMaxCnt - 1);
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
	int secretCnt = 0;
	int ownedCnt = 0;
	for (auto const &elemMagicStone : arrStones)
	{
		auto elemStatus = elemMagicStone->getStatus();
		if (elemStatus == gameMetaData::msStatus::notUse)
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
	for (auto const &i : arrPlayers)
	{
		//std::cout << "StoneList size : " << i->getStoneListSize() << std::endl;
		//std::cout << "BooungList size : " << i->getBooungListSize() << std::endl;
	}
}
