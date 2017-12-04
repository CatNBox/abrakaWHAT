#include "gameRoomObjLayer.h"
#include "managers\gameFlowManager.h"
#include <iostream>

using namespace cocos2d;

bool gameRoomObjLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//eventListener setting
	settingEventListener();

	//countOption value setting
	settingCntValues();
	
	//create players
	createPlayers();

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
	playerCnt = gameFlowManager::getInstance()->getPlayerCount();
	secretCnt = gameFlowManager::getInstance()->getSecretCount();

	arrMsCnt[gameMetaData::msType::yongyong] = gameFlowManager::getInstance()->getYongyongCount();
	arrMsCnt[gameMetaData::msType::bangrang] = gameFlowManager::getInstance()->getBangrangCount();
	arrMsCnt[gameMetaData::msType::wind] = gameFlowManager::getInstance()->getWindCount();
	arrMsCnt[gameMetaData::msType::booung] = gameFlowManager::getInstance()->getBooungCount();
	arrMsCnt[gameMetaData::msType::bunpok] = gameFlowManager::getInstance()->getBunpokCount();
	arrMsCnt[gameMetaData::msType::nungang] = gameFlowManager::getInstance()->getNungangCount();
	arrMsCnt[gameMetaData::msType::buljak] = gameFlowManager::getInstance()->getBuljakCount();
	arrMsCnt[gameMetaData::msType::potion] = gameFlowManager::getInstance()->getPotionCount();

	stoneMaxCnt = 
		arrMsCnt[gameMetaData::msType::yongyong]		//yongyong 
		+ arrMsCnt[gameMetaData::msType::bangrang]	//bangrang
		+ arrMsCnt[gameMetaData::msType::wind]	//wind
		+ arrMsCnt[gameMetaData::msType::booung]	//booung
		+ arrMsCnt[gameMetaData::msType::bunpok]	//bunpok
		+ arrMsCnt[gameMetaData::msType::nungang]	//nungang
		+ arrMsCnt[gameMetaData::msType::buljak]	//buljak
		+ arrMsCnt[gameMetaData::msType::potion];	//potion
}

void gameRoomObjLayer::createPlayers()
{
	//���� �� �÷��̾� ��ȣ�� �������� �� �� - ���� ��Ƽ��� ����
	bool bPlayer = true;
	arrPlayers.resize(playerCnt);
	int idx = 0;
	for (auto &i : arrPlayers)
	{
		player* tempPlayer;
		if (bPlayer == true)
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
		if (i == myPlayerNum)	//��Ƽ��� �� �� �÷��̾� ��ȣ�� �ǵ���
		{
			defaultY = 160;
		}
		else if (i == (myPlayerNum + 1) % playerCnt)
		{
			defaultX = 700;
			defaultY = 434;
			rotValue = 90.0f;
		}
		else if (i == (myPlayerNum + 2) % playerCnt)
		{
			defaultY = 700;
		}
		else if (i == (myPlayerNum + 3) % playerCnt)
		{
			defaultX = 68;
			defaultY = 434;
			rotValue = -90.0f;
		}
		else
		{
			assert(true && !"createPlayer()���� �߸��� PlayerCnt���� ����");
		}
		arrPlayers[i]->setDefaultX(defaultX);
		arrPlayers[i]->setDefaultY(defaultY);
		arrPlayers[i]->setRotationValue(rotValue);
	}
}

void gameRoomObjLayer::createMagicStones()
{
	arrStones.resize(stoneMaxCnt);

	arrStones[0] = new msYongyong;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong]] 
		= new msBangrang;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]] 
		= new msWind;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang] 
		+ arrMsCnt[gameMetaData::msType::wind]] 
		= new msBooung;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang] 
		+ arrMsCnt[gameMetaData::msType::wind] 
		+ arrMsCnt[gameMetaData::msType::booung]] 
		= new msBunpok;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]
		+ arrMsCnt[gameMetaData::msType::wind]
		+ arrMsCnt[gameMetaData::msType::booung]
		+ arrMsCnt[gameMetaData::msType::bunpok]] 
		= new msNungang;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]
		+ arrMsCnt[gameMetaData::msType::wind]
		+ arrMsCnt[gameMetaData::msType::booung]
		+ arrMsCnt[gameMetaData::msType::bunpok]
		+ arrMsCnt[gameMetaData::msType::nungang]] 
		= new msBuljak;

	arrStones[arrMsCnt[gameMetaData::msType::yongyong] 
		+ arrMsCnt[gameMetaData::msType::bangrang]
		+ arrMsCnt[gameMetaData::msType::wind]
		+ arrMsCnt[gameMetaData::msType::booung]
		+ arrMsCnt[gameMetaData::msType::bunpok]
		+ arrMsCnt[gameMetaData::msType::nungang]
		+ arrMsCnt[gameMetaData::msType::buljak]] 
		= new msPotion;

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
		this->addChild(arrStones[i], gameMetaData::layerZOrder::objZ2);
	}
}

void gameRoomObjLayer::createSeenChecker()
{
	//205,400 / 16*32 / y+40 x+23 / +35

	Sprite* seenMs;
	for (int msNum = 0; msNum < 8; msNum++)
	{
		//seenChecker ��������Ʈ���� ��ǥ����
		int defaultX = 205, defaultY = 400, recalibX = 23, recalibY = 40;
		if (msNum > 3)
			defaultX = 205 + recalibX * 3 + 35;

		std::vector<std::pair<Sprite*,bool>> tempVector;

		//seenCnt�� ����ŭ seen��������Ʈ �׸�, �� �ܿ��� notSeen��������Ʈ
		for (int seenObjNum = 0; seenObjNum < arrMsCnt[msNum + 1]; seenObjNum++)
		{
			seenMs = Sprite::createWithSpriteFrameName(gameMetaData::arrNotSeenSpriteName[msNum]);

			seenMs->setPosition(Vec2(defaultX + seenObjNum * recalibX, defaultY + (msNum % 4) * recalibY));
			this->addChild(seenMs,gameMetaData::layerZOrder::objZ0);
			std::pair<Sprite*, bool> tempPair(seenMs,false);
			tempVector.push_back(tempPair);
		}
		seenChecker.push_back(tempVector);
	}
}

void gameRoomObjLayer::createPlayerLpObj()
{
	//��ġ�� �÷��̾� ��ġ���� ����+-�� �������� ��������̵���
	//objLayer�� LP sprite�� ������
	int idx = 0;
	for (auto &i : arrPlayers)
	{
		for (int j = 0; j < gameFlowManager::getInstance()->getMaxLifePoint(); j++)
		{
			auto temp = i->createLpObj(idx + 1);
			this->addChild(temp, gameMetaData::layerZOrder::objZ1);
		}
		idx++;
	}
}

void gameRoomObjLayer::initRound()
{
	//�ӽ��ڵ�
	isChanged = true;

	//0.init each Object
	//init magicStones
	for (auto &i : arrStones)
	{
		i->initObjData();
	}
	//init Players
	for (auto &i : arrPlayers)
	{
		i->init();
	}
	//init seenChecker
	for (int msNum = 0; msNum < 8; msNum++)
	{
		for (auto &j : seenChecker[msNum])
		{
			j.first->initWithSpriteFrameName(gameMetaData::arrNotSeenSpriteName[msNum]);
			j.second = false;
		}
	}
	//init player's LP

	//display RoundCnt
	EventCustom passTurnEvent("roundUp");
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&passTurnEvent);

	//1. select Secret Stone
	selSecretStone();

	//2. share magicStones to each player
	shareStone2Player();

	//3. setting firstTurn Player
	starterNum = 0;
	curPlayerNum = 0;
	setStartOrder(); //starterNum setting

					 //checkArrStones();
					 //checkArrPlayers();
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
			std::cout << "initRound()::secretTemp���� ���� ��� ���Ǿ �й� �Ұ���" << std::endl;
			break;
		}

		//arrSecret.push_back(secretTemp);	//I think it is not necessary

		secretTemp->setStatus(gameMetaData::msStatus::secret);
		secretTemp->setBaseSprite();
		//Draw and MoveAction
		auto targetPos = Vec2(230 + i * 100, 600);
		secretTemp->actionMove(0.2f * preDelayCnt, targetPos);
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
			std::cout << "initRound()::tempSelStone���� ���� ��� ���Ǿ �й� �Ұ���" << std::endl;
			break;
		}

		//Sharing by player order
		int tempCurPlayerIdx = i%playerCnt;
		arrPlayers[tempCurPlayerIdx]->pushStone2List(tempSelStone);
		tempSelStone->setStatus(gameMetaData::msStatus::owned);
		//if (tempCurPlayerIdx == myPlayerNum)
		//{
		//	tempSelStone->setBaseSprite();
		//}
		//else
		//{
			tempSelStone->initMsSprite();
		//}
		//Draw and MoveAction
		int defaultX = arrPlayers[tempCurPlayerIdx]->getDefaultX();
		int defaultY = arrPlayers[tempCurPlayerIdx]->getDefaultY();
		int revisionValue = getMsPosRevision(5, i / 4);
		float rotValue = arrPlayers[tempCurPlayerIdx]->getRotationValue();
		auto tempVec = Vec2(defaultX + revisionValue, defaultY);
		if (tempCurPlayerIdx % 2 == 1)
		{
			tempVec.setPoint(defaultX, defaultY + revisionValue);
		}
		tempSelStone->setRotation(rotValue);
		tempSelStone->actionMove(0.2f * (secretCnt + preDelayCnt), tempVec);
		preDelayCnt++;
	}
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
		//seenCheckUpdate();
		//stoneObjUpdate();
		//curLPUpdate();

		isChanged = false;
	}
}

void gameRoomObjLayer::dataUpdate()
{
	//discard�� ���ڸ�ŭ �ش� ms�� seenCnt����
	for (int i = 0; i < 8; i++)
	{
		//seenCnt[i] = 0;
	}
	for (auto &i : arrStones)
	{
		int magicEnum = i->getMagic() - 1;
		//if (i->getStatus() == gameMetaData::msStatus::discard)
			//seenCnt[magicEnum]++;
	}

	if (arrPlayers[curPlayerNum]->isNPC())
	{
		npc* tempNpc = (npc*)arrPlayers[curPlayerNum];
		tempNpc->npcProcess();
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

//������ magicStone üũ�̺�Ʈ
void gameRoomObjLayer::checkOwnedMagic(EventCustom* checkOwnedMagicEvent)
{
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
		//seenChecker update
		for (auto &i : seenChecker[magicEnum - 1])
		{
			if (i.second == false)
			{
				auto seenCheckEffect = cocos2d::ParticleSystemQuad::create("particle4SeenCheck.plist");
				seenCheckEffect->setPosition(i.first->getPosition());
				this->addChild(seenCheckEffect,gameMetaData::layerZOrder::effectZ);
				i.first->initWithSpriteFrameName(gameMetaData::arrSeenSpriteName[magicEnum - 1]);
				i.second = true;
				break;
			}
		}
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
	//�ӽ��ڵ�
	isChanged = true;

	//current Player
	auto curPlayer = arrPlayers[curPlayerNum];

	//magicStone�� ���������� 5���� �ƴϰ� ���������� magicStone�� �ִٸ� �����Ѵ�
	//��� ���Ǿ��ٸ� ���带 �����Ѵ�
	if (isAllUsed())
	{
		//��������
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


	//���� �ѱ��
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
		//���� ī�尡 ���� ��� ���ܼ���
		//1. ��� ������ ���
		//2. �ʱ�ȭ�� �ȵǾ �й迡 �������� ���
		return nullptr;
	}

	magicStone *ptempMS;
	int rndIndex = 0;
	while (true)
	{
		rndIndex = gameFlowManager::getInstance()->getRandomInt(stoneMinCnt, stoneMaxCnt - 1);
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

int gameRoomObjLayer::getMsPosRevision(int msListSize, int msOrder)
{
	return 38 * (msListSize - 1 - (msOrder * 2));
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
