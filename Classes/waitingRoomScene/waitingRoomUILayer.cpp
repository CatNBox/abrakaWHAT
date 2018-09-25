#include "waitingRoomScene\waitingRoomUILayer.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"
#include "gameRoomScene\gameRoomScene.h"

using namespace cocos2d;

bool waitingRoomUILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	settingEventListener();
	initUI();

	sprManager = new spriteManager;
	actManager = actionManager::getInstance();
	
	return true;
}

void waitingRoomUILayer::setGameMode(gameMetaData::gameMode modeFlag)
{
	curMode = modeFlag;
}

void waitingRoomUILayer::settingEventListener()
{
	//eventDispatch
	//exitBtn
	//gameStartBtn
	//settingPlayerOrder orderingBtn
}

void waitingRoomUILayer::initUI()
{
	//setting menu and menuItem
	//exitBtn, orderingBtn, startBtn
	auto btnExit = MenuItemImage::create(
		"UISprite/btnExitNormal.png",
		"UISprite/btnExitPress.png",
		"UISprite/btnExitPress.png",
		CC_CALLBACK_0(
			waitingRoomUILayer::returnMainmenuCallback,
			this
		)
	);
	btnExit->setScale(0.5f);
	btnExit->setPosition(Vec2(330, 670+56-384));

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

	/*
	auto btnOrdering = MenuItemImage::create(
		"UISprite/btnOrderingNormal.png",
		"UISprite/btnOrderingPress.png",
		"UISprite/btnOrderingPress.png",
		CC_CALLBACK_0(
			waitingRoomUILayer::orderingCallback,
			this
		)
	);
	*/

	btnMenu = Menu::create();

	btnMenu->addChild(btnExit);
	btnMenu->addChild(btnStart);
	//btnMenu->addChild(btnOrdering);

	this->addChild(btnMenu);

	initPlayerLabel();
	//addPlayerLabel();
}

void waitingRoomUILayer::initPlayerLabel()
{
	playerCnt = gameMetaData::defaultPlayerCnt;
	playerList.resize(playerCnt);

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
		i->setPosition(Vec2(180, labelY -= 70));

		this->addChild(i, gameMetaData::layerZOrder::objZ0);
	}
}

void waitingRoomUILayer::addPlayerLabel()
{
	//add other player on multiplayMode
	auto playerSpr02 = Sprite::createWithSpriteFrameName("sprPlayer.png");

	playerSpr02->setScale(0.7f);
	playerSpr02->setAnchorPoint(Vec2::ZERO);
	playerSpr02->setPosition(Vec2(180, 500));

	this->addChild(playerSpr02, gameMetaData::layerZOrder::objZ0);
}

void waitingRoomUILayer::returnMainmenuCallback()
{
	EventCustom popupEvent("popupWarning");
	auto warningFlag = gameMetaData::warningCode::exitBtnWarning;
	popupEvent.setUserData(&warningFlag);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&popupEvent);
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

	/*
	on multiMode, popup ask adding CPU when not enough player 
	*/
	setBtnDisabled();
	orderingCallback();

	
	this->schedule([=](float d) {
		this->runGameScene();
	}, 0.0f, 0, 2.0f, "startGameScene");
	
}

void waitingRoomUILayer::orderingCallback()
{
	auto tempCheckSet = std::vector<bool>(playerList.size()); //init by false
	for (auto i = 0; i < playerList.size(); i++)
	{
		int pickNum = 0;
		int breakInf = 0;
		for(;;)
		{
			auto tempCursor = inlineFunc::getRandomInt(0, tempCheckSet.size() - 1);
			if (!tempCheckSet.at(tempCursor))
			{
				tempCheckSet.at(tempCursor) = true;
				pickNum = tempCursor + 1;
				break;
			}

			if (breakInf > 1000) break; //오류추가
			else breakInf++;
		}
		playerOrder[i] = pickNum;

		auto orderNum = Sprite::createWithSpriteFrameName("spr_number.png");
		orderNum->setTextureRect(sprManager->getNumSprRect(pickNum));
		orderNum->setScale(0.6f);
		orderNum->setAnchorPoint(Vec2::ZERO);
		auto posX = playerList.at(i)->getPositionX();
		auto posY = playerList.at(i)->getPositionY();
		orderNum->setPosition(Vec2(posX + 200, posY + 10));
		//orderNum->setVisible(false);
		this->addChild(orderNum);

		//액션추가
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
	auto startGameScene = TransitionCrossFade::create(0.3f, gameRoomScene::createScene(curMode, playerOrder));
	Director::getInstance()->replaceScene(startGameScene);
}

