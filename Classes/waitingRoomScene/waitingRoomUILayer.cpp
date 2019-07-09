#include "waitingRoomScene\waitingRoomUILayer.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"
#include "managers\networkManager.h"
#include "gameRoomScene\gameRoomScene.h"

using namespace cocos2d;

bool waitingRoomUILayer::init(gameMetaData::gameMode modeFlag)
{
	if (!Layer::init())
	{
		return false;
	}
	curGameMode = modeFlag;
	hostAddr = "000.000.000.000";
	playerTurnOrder[0] = 0;
	playerCnt = 0;
	NPCCnt = 0;
	sprManager = new spriteManager;
	actManager = actionManager::getInstance();
	netManager = networkManager::getInstance();

	if (curGameMode == gameMetaData::gameMode::single)
	{
		curProgressStage = gameMetaData::gameProgressStage::waiting;
	}
	else
	{
		curProgressStage = netManager->getNetworkProgressStage();
	}
		
	settingEventListener();
	initUI();

	this->scheduleUpdate();

	return true;
}

waitingRoomUILayer * waitingRoomUILayer::createWithParam(gameMetaData::gameMode modeFlag)
{
	waitingRoomUILayer *pRet = new(std::nothrow) waitingRoomUILayer();
	if (pRet && pRet->init(modeFlag))
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

void waitingRoomUILayer::update(float dTime)
{
	if (curGameMode == gameMetaData::gameMode::single)
	{

	}
	else
	{
		//check server state
		//get connected playerCnt
		auto netPlayerCnt = netManager->getPlayerCnt();
		auto netNPCCnt = netManager->getNPCCnt();
		if (playerCnt != netPlayerCnt
			|| NPCCnt != netNPCCnt)
		{
			playerCnt = netPlayerCnt;
			NPCCnt = netNPCCnt;
			updatePlayerLabel();
		}

		//setting turnOrder
		if (playerTurnOrder[0] == 0)
		{
			if (netManager->getTurnOrderSettingState())
			{
				setPlayerTurnOrder();
				showSpriteTurnOrder();

				if (curGameMode == gameMetaData::gameMode::host)
				{
					netManager->requestGameRoomStart();
				}
			}
		}

		curProgressStage = netManager->getNetworkProgressStage();
	}

	//gameRoomScene loading if ready for loading gameRoom
	if (curProgressStage == gameMetaData::gameProgressStage::readyLoadingGameRoomScene)
	{
		if (curGameMode == gameMetaData::gameMode::single)
		{
			curProgressStage = gameMetaData::gameProgressStage::loadingGameRoomScene;
			showSpriteTurnOrder();
		}
		else
		{
			netManager->setFlagLoadingGameRooom();
		}

		auto sprCntdwnNum = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		auto tempSprRect = sprManager->getNumSprRect(4);
		sprCntdwnNum->setTextureRect(tempSprRect);
		sprCntdwnNum->setPosition(STDAXIS, 75.0f);
		this->addChild(sprCntdwnNum, gameMetaData::layerZOrder::objZ1);

		cocos2d::Vector<cocos2d::FiniteTimeAction*> vecAction;

		//startNum ~ 1
		for (int i = 4; i > 0; i--)
		{
			auto tempFuncChangeCntNum = cocos2d::CallFunc::create([=]() {
				sprCntdwnNum->setTextureRect(sprManager->getNumSprRect(i));
			});
			vecAction.pushBack(tempFuncChangeCntNum);

			auto tempDelay = cocos2d::DelayTime::create(1.0f);
			vecAction.pushBack(tempDelay);
		}
		//zero
		auto tempFuncChangeCntNum = cocos2d::CallFunc::create([=]() {
			sprCntdwnNum->setTextureRect(sprManager->getNumSprRect(0));
		});
		vecAction.pushBack(tempFuncChangeCntNum);

		auto seq = cocos2d::Sequence::create(vecAction);
		sprCntdwnNum->runAction(seq);

		this->schedule([=](float d) {
			this->unscheduleUpdate();
			this->runGameScene();
		}, 0.0f, 0, 4.0f, "startGameScene");
	}
}

void waitingRoomUILayer::settingEventListener()
{
	//eventDispatch
	//gameStartBtn
	//settingPlayerOrder orderingBtn

	//initIpAddr
	eventListener = EventListenerCustom::create("UILayerInitIpAddr",
		CC_CALLBACK_0(waitingRoomUILayer::updateIpAddr, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	//guestMode visible true
	eventListener = EventListenerCustom::create("guestModeUILayerWakeup",
		[=](EventCustom* event) {
		this->setVisible(true);
		this->updateIpAddr();
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void waitingRoomUILayer::initUI()
{
	//menu create
	btnMenu = Menu::create();
	this->addChild(btnMenu);

	// gameStart btn
	if (curGameMode != gameMetaData::gameMode::guest)
	{
		auto btnStart = MenuItemImage::create(
			"UISprite/btnStartNormal.png",
			"UISprite/btnStartPress.png",
			"UISprite/btnStartPress.png",
			CC_CALLBACK_0(
				waitingRoomUILayer::startGameCallback,
				this
			)
		);
		btnStart->setScale(0.8f);
		btnStart->setPosition(Vec2(0, -230));

		btnMenu->addChild(btnStart);
	}

	//init playerLabel visible false
	playerList.resize(gameMetaData::defaultPlayerCnt);

	auto tempPlayerLabel = Sprite::createWithSpriteFrameName("sprPlayer.png");
	playerList.at(0) = tempPlayerLabel;

	int labelY = 650;
	for (auto &i : playerList)
	{
		if (i == nullptr)
		{
			auto tempCpuLabel = Sprite::createWithSpriteFrameName("sprPlayer.png");
			i = tempCpuLabel;
		}

		i->setScale(0.7f);
		i->setAnchorPoint(Vec2::ZERO);
		i->setPosition(Vec2(100, labelY -= 70));
		i->setVisible(false);

		this->addChild(i, gameMetaData::layerZOrder::objZ0);
	}

	if (curGameMode != gameMetaData::gameMode::single)
	{
		// copy to clipboard btn
		auto tempNormalC2CSpr = Sprite::createWithSpriteFrameName("btnCopyToClip.png");
		tempNormalC2CSpr->setColor(Color3B(225, 225, 225));
		auto tempClickedC2CSpr = Sprite::createWithSpriteFrameName("btnCopyToClip.png");
		tempClickedC2CSpr->setColor(Color3B(125, 125, 125));
		auto tempDisabledC2CSpr = Sprite::createWithSpriteFrameName("btnCopyToClip.png");
		tempDisabledC2CSpr->setColor(Color3B(95, 95, 95));
		auto btnCopyToClipboard = MenuItemImage::create();
		btnCopyToClipboard->initWithNormalSprite(
			tempNormalC2CSpr,
			tempClickedC2CSpr,
			tempDisabledC2CSpr,
			CC_CALLBACK_0(
				waitingRoomUILayer::copy2Clipboard,
				this
			)
		);
		btnCopyToClipboard->setScale(0.4f);
		btnCopyToClipboard->setAnchorPoint(Vec2::ZERO);
		btnCopyToClipboard->setPosition(Vec2(80, 200));

		btnMenu->addChild(btnCopyToClipboard);

		initIpAddrSpr();
		updateIpAddr();
	}

	updatePlayerLabel();
}

void waitingRoomUILayer::updateIpAddr()
{
	hostAddr = netManager->getServerAddr();
	setIpAddrSpr();
}

void waitingRoomUILayer::updatePlayerLabel()
{
	for (int i = 0; i < gameMetaData::defaultPlayerCnt; i++)
	{
		if (curGameMode == gameMetaData::gameMode::single)
		{
			if (i == 0)
			{
				playerList[i]->initWithSpriteFrameName("sprPlayer.png");
				playerList[i]->setScale(0.7f);
				playerList[i]->setAnchorPoint(Vec2::ZERO);
				playerList[i]->setVisible(true);
			}
			else
			{
				playerList[i]->initWithSpriteFrameName("sprNPC.png");
				playerList[i]->setScale(0.7f);
				playerList[i]->setAnchorPoint(Vec2::ZERO);
				playerList[i]->setVisible(true);
			}
		}
		else
		{
			auto tempPlayersState = netManager->getPlayerConnectionState(i);
			if (tempPlayersState == gameMetaData::netPlayerState::disconnected)
			{
				playerList[i]->setVisible(false);
			}
			else if (tempPlayersState == gameMetaData::netPlayerState::connected)
			{
				/*
					add code setting player texture
				*/
				playerList[i]->initWithSpriteFrameName("sprPlayer.png");
				playerList[i]->setScale(0.7f);
				playerList[i]->setAnchorPoint(Vec2::ZERO);
				playerList[i]->setVisible(true);
			}
			else
			{
				/*
					add code setting NPC texture
				*/
				playerList[i]->initWithSpriteFrameName("sprNPC.png");
				playerList[i]->setScale(0.7f);
				playerList[i]->setAnchorPoint(Vec2::ZERO);
				playerList[i]->setVisible(true);
			}
		}
	}
}

void waitingRoomUILayer::addPlayerLabel()
{
	//add other player on multiplayMode
	auto playerSpr02 = Sprite::createWithSpriteFrameName("sprPlayer.png");

	playerSpr02->setScale(0.7f);
	playerSpr02->setAnchorPoint(Vec2::ZERO);
	playerSpr02->setPosition(Vec2(140, 500));

	this->addChild(playerSpr02, gameMetaData::layerZOrder::objZ0);
}

void waitingRoomUILayer::startGameCallback()
{
	/*
	start 시퀀스
	1. 버튼클릭
	2. 입력불가로 만든 후 연출
	3. 숫자가 날아와서 플레이어 옆에 붙음
	4. waitingRoomScene 천천히 페이드아웃 후 gameRoomScene으로 변경
	*/

	if (curGameMode == gameMetaData::gameMode::single)
	{

	}
	else
	{
		/*
		on multiMode, popup ask adding CPU when not enough player
		*/
		//not joined player slot check
		for (int i = 0; i < gameMetaData::defaultPlayerCnt; i++)
		{
			auto curPlayerState = netManager->getPlayerConnectionState(i);
			if (curPlayerState == gameMetaData::netPlayerState::disconnected)
			{
				netManager->requestSettingNPC(i);
			}
		}
	}

	setBtnDisabled();
	if (setBufTurnOrder())
	{
		if (curGameMode == gameMetaData::gameMode::single)
		{
			curProgressStage = gameMetaData::gameProgressStage::readyLoadingGameRoomScene;
		}
		else
		{
			netManager->requestSettingOrder(bufTurnOrder);
		}
	}
	else
	{
		//0~3의 랜덤값 중 한번도 선택되지않은 숫자가 있는 오류
		assert(true && !"startGameCallback()에서 turnOrder가 제대로 설정되지않음");
	}
}

bool waitingRoomUILayer::setBufTurnOrder()
{
	auto pickedNumCheckList = std::vector<bool>(playerList.size()); //init by false
	for (auto i = 0; i < playerList.size(); i++)
	{
		int pickOrderNum = 0;
		int breakInf = 0;
		for(;;)
		{
			auto tempCursor = inlineFunc::getRandomInt(0, pickedNumCheckList.size() - 1);
			if (pickedNumCheckList.at(tempCursor) == false)
			{
				pickedNumCheckList.at(tempCursor) = true;
				pickOrderNum = tempCursor + 1;
				break;
			}

			if (breakInf > 1000) break; //오류추가
			else breakInf++;
		}
		if (pickOrderNum == 0)
		{
			return false;
		}
		bufTurnOrder[i] = pickOrderNum;
		if (curGameMode == gameMetaData::gameMode::single)
		{
			playerTurnOrder[i] = pickOrderNum;
		}
	}

	return true;
}

void waitingRoomUILayer::setPlayerTurnOrder()
{
	for (int i = 0; i < gameMetaData::defaultPlayerCnt; i++)
	{
		playerTurnOrder[i] = netManager->getTurnOrder(i);
	}
}

void waitingRoomUILayer::showSpriteTurnOrder()
{
	for (auto i = 0; i < playerList.size(); i++)
	{
		//setting order number
		auto orderNum = Sprite::createWithSpriteFrameName("spr_number.png");
		orderNum->setTextureRect(sprManager->getNumSprRect(playerTurnOrder[i]));
		orderNum->setScale(0.6f);
		orderNum->setAnchorPoint(Vec2::ZERO);
		auto posX = playerList.at(i)->getPositionX();
		auto posY = playerList.at(i)->getPositionY();
		orderNum->setPosition(Vec2(posX + 220, posY + 10));
		//orderNum->setVisible(false);
		this->addChild(orderNum);

		//액션추가
	}
}

void waitingRoomUILayer::initIpAddrSpr()
{
	int addWeightValue = 0;
	for (int i = 0; i < 12; i++)
	{
		if (i % 3 == 0)
		{
			addWeightValue += 12;
		}
		//sprite IPAddr
		ipAddrSpr[i] = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		ipAddrSpr[i]->setTextureRect(sprManager->getNumSprRect(0));
		ipAddrSpr[i]->setScale(0.7f);
		ipAddrSpr[i]->setAnchorPoint(Vec2::ZERO);
		ipAddrSpr[i]->setPosition(Vec2(400 + 25 * i + addWeightValue, 630));
		this->addChild(ipAddrSpr[i], gameMetaData::layerZOrder::objZ1);
	}
	for (int i = 0; i < 3; i++)
	{
		//dot spr
		auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName("sprDot.png");
		tempSpr->setScale(0.3f);
		tempSpr->setAnchorPoint(Vec2::ZERO);
		tempSpr->setPosition(Vec2(485 + 90 * i, 622));
		this->addChild(tempSpr, gameMetaData::layerZOrder::objZ0);
	}
}

void waitingRoomUILayer::errorWrongIpFormat()
{
	for (auto &ipSpr : ipAddrSpr)
	{
		ipSpr->setVisible(false);
	}
	btnMenu->setVisible(false);

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName("sprError.png");
	tempSpr->setScale(0.7f);
	tempSpr->setAnchorPoint(Vec2::ZERO);
	tempSpr->setPosition(Vec2(400, 600));
	this->addChild(tempSpr, gameMetaData::layerZOrder::objZ0);
}

void waitingRoomUILayer::setIpAddrSpr()
{
	//0.0.0.0 = 000.000.000.000
	bool wrongFormat = false;
	std::string bufStr = "000.000.000.000";
	for (int i = hostAddr.size() - 1, bi = bufStr.size()-1, tempCnt = 0; 
		i >= 0; 
		i--, bi--)
	{
		char bufData = hostAddr.at(i);
		if (bufData != '.')
		{
			if (bufData >= '0' && bufData <= '9')
			{
				bufStr.at(bi) = bufData;
			}
			else
			{
				wrongFormat = true;
				break;
			}

		}
		else
		{
			//3711
			tempCnt++;
			bi = bufStr.size() - (tempCnt*4);
			if (bi < 0)
			{
				wrongFormat = true;
				break;
			}
		}
	}

	if (wrongFormat)
	{
		errorWrongIpFormat();
		return;
	}

	hostAddr = bufStr;

	for (int i = 0; i < hostAddr.size(); i++)
	{
		if (i % 4 == 3)
		{
			continue;
		}
		char buf4atoi[2] = { '\0','\0' };
		buf4atoi[0] = hostAddr[i];
		int tempNum = atoi(buf4atoi);
		int sprIndex = i - (i / 4);
		ipAddrSpr[sprIndex]->setTextureRect(sprManager->getNumSprRect(tempNum));
	}
}

void waitingRoomUILayer::copy2Clipboard()
{
	int targetStrLen = hostAddr.size() + 1; //include '\0'
	const char* bufStr = hostAddr.c_str();

	//alloc memory
	HANDLE hData = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, targetStrLen);

	char* pData = (char*)::GlobalLock(hData);
	if (pData != nullptr)
	{
		memcpy(pData, bufStr, targetStrLen);

		//unlock to cpy to clipboard
		::GlobalUnlock(hData);

		//open
		if (::OpenClipboard(NULL))
		{
			//empties clipboard
			::EmptyClipboard();
			//cpy
			::SetClipboardData(CF_TEXT, hData);
			//close
			::CloseClipboard();
		}
	}
}

void waitingRoomUILayer::setBtnDisabled()
{
	btnMenu->setEnabled(false);
}

void waitingRoomUILayer::setCpuSpr()
{
}

void waitingRoomUILayer::runGameScene()
{
	auto startGameScene = TransitionCrossFade::create(0.3f, gameRoomScene::createScene(curGameMode, playerTurnOrder));
	Director::getInstance()->replaceScene(startGameScene);
}

