#include "gameRoomUILayer.h"
#include "gameObject\gameMetaData.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"
#include <iostream>

using namespace cocos2d;

bool gameRoomUILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	settingEventListener();

	sprManager = new spriteManager;
	actManager = actionManager::getInstance();

	initUI();

	return true;
}

void gameRoomUILayer::settingEventListener()
{
	//set keyboardEvent
	keyListener = EventListenerKeyboard::create();
	if (keyListener != nullptr)
	{
		keyListener->onKeyPressed = CC_CALLBACK_2(gameRoomUILayer::onKeyPressed, this);
		keyListener->onKeyReleased = CC_CALLBACK_2(gameRoomUILayer::onKeyReleased, this);
		auto dispatcher = this->getEventDispatcher();
		dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	}
	else
	{
		log("### err ### gameRoomLayer::keyListener is nullptr ");
	}

	//event dispatch
	//enable passBtn, selectUI
	callBackListener = EventListenerCustom::create("choicerUIEnabled",
		[=](EventCustom* event) {
		magicChoicer->setEnabled(true);
		auto btnPass = (MenuItemImage*)magicChoicer->getChildByName("btnPass");
		btnPass->setEnabled(true);
		//other btns is diabled until last choice btn
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//disable passBtn, selectUI WHEN passing turn to next user
	callBackListener = EventListenerCustom::create("passTurn2NextUser",
		[=](EventCustom* event) {
		setEnabledInputUI(false, false);
		lastChooseMs = 0;
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//enable selectUI without passBtn WHEN turn back to me
	callBackListener = EventListenerCustom::create("myTurn",
		[=](EventCustom* event) {
		setEnabledInputUI(true, false);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//라운드가 종료되면 라운드 숫자 UP
	callBackListener = EventListenerCustom::create("roundUp",
		[=](EventCustom* event) {
		std::cout << "round : " << roundNum << std::endl;
		roundNum++;
		std::cout << "round : " << roundNum << std::endl;
		setRound();
		setEnabledInputUI(false, false);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

}

void gameRoomUILayer::setEnabledInputUI(const bool flag4KeyboardNselectUI, const bool flag4PassBtn)
{
	magicChoicer->setEnabled(flag4KeyboardNselectUI);
	for (auto i : arrBtnSelectStone)
	{
		i->setEnabled(flag4KeyboardNselectUI);
	}
	auto btnPass = (MenuItemImage*)magicChoicer->getChildByName("btnPass");
	btnPass->setEnabled(flag4PassBtn);

	keyListener->setEnabled(flag4KeyboardNselectUI);
}


void gameRoomUILayer::initUI()
{
	for (int i = 1; i < 9; i++)
	{
		auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(gameMetaData::arrMsSpriteName[i]);
		auto tempSprDisabled = cocos2d::Sprite::createWithSpriteFrameName(gameMetaData::arrMsSpriteName[i]);
		tempSprDisabled->setColor(cocos2d::Color3B(125, 125, 125));
		auto tempSprBackward = cocos2d::Sprite::createWithSpriteFrameName(gameMetaData::arrMsSpriteName[0]);
		auto tempItem = cocos2d::MenuItemImage::create(); 
		tempItem->initWithNormalSprite(
			tempSpr,
			tempSprBackward,
			tempSprDisabled,
			CC_CALLBACK_0(
				gameRoomUILayer::checkMagic,
				this,
				(const int)i
			)
		);
		arrBtnSelectStone.push_back(tempItem);
	}
	
	auto btnPass = MenuItemImage::create(
		"UISprite/ms_pass_abled_normal.png",
		"UISprite/ms_pass_abled_clicked.png",
		"UISprite/ms_pass_disabled.png",
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::pass
		)
	);

	magicChoicer = cocos2d::Menu::create();
	for (int i = 0; i < gameMetaData::variableMaxCnt::msTypeCnt - 1; i++)
	{
		magicChoicer->addChild(arrBtnSelectStone[i]);
	}

	btnPass->setEnabled(false);
	magicChoicer->addChild(btnPass, 0, "btnPass");
	magicChoicer->alignItemsHorizontallyWithPadding(10.0f);
	magicChoicer->setPosition(Vec2(384, 56));
	this->addChild(magicChoicer);

	auto seenCheckerBoard = Sprite::createWithSpriteFrameName("ms_seenBG.png");
	seenCheckerBoard->setPosition(Vec2(380, 420));
	seenCheckerBoard->setRotation(90.0f);

	this->addChild(seenCheckerBoard);

	roundSpr = Sprite::createWithSpriteFrameName("spr_number.png");
	roundSpr->setPosition(Vec2(50, 710));
	roundSpr->setScale(0.8f);
	setRound();

	this->addChild(roundSpr);

	auto scoreSpr = Sprite::createWithSpriteFrameName("sprScore.png");
	scoreSpr->setPosition(Vec2(660, 216));
	scoreSpr->setScale(0.5f);

	this->addChild(scoreSpr);
}

void gameRoomUILayer::setRound()
{
	//라운드 50 710
	roundSpr->setTextureRect(sprManager->getNumSprRect(roundNum));
}

bool gameRoomUILayer::checkRunningAction()
{
	int actionCnt = actManager->getRunningActionCnt();
	if (actionCnt != 0)
	{
		return true;
	}
	return false;
}

void gameRoomUILayer::checkMagic(const int magicStoneNumber)
{
	if (checkRunningAction())
		return;

	//처리중 버튼메뉴 비활성화 - 이벤트 람다함수를 통해 활성화됨
	magicChoicer->setEnabled(false);
	//this->setKeyboardEnabled(false); //변경할것

	lastChooseMs = magicStoneNumber;
	for (int i = 0; i < lastChooseMs-1; i++)
	{
		arrBtnSelectStone[i]->setEnabled(false);
	}

	EventCustom checkEvent("checkOwnedMagic");
	checkEvent.setUserData((void*)magicStoneNumber);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
}

void gameRoomUILayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (checkRunningAction() || !(magicChoicer->isEnabled()))
		return;

	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_1)
	{
		checkMagic(gameMetaData::msType::yongyong);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_2)
	{
		checkMagic(gameMetaData::msType::bangrang);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_3)
	{
		checkMagic(gameMetaData::msType::wind);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_4)
	{
		checkMagic(gameMetaData::msType::booung);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_5)
	{
		checkMagic(gameMetaData::msType::bunpok);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_6)
	{
		checkMagic(gameMetaData::msType::nungang);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_7)
	{
		checkMagic(gameMetaData::msType::buljak);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_8)
	{
		checkMagic(gameMetaData::msType::potion);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_F5)
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("initRound");
	}
}

void gameRoomUILayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
}
