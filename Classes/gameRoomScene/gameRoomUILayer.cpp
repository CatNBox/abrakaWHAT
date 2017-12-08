#include "gameRoomUILayer.h"
#include "managers\gameFlowManager.h"
#include "gameObject\gameMetaData.h"
#include <iostream>

using namespace cocos2d;

bool gameRoomUILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	//arrBtnSelectStone.resize(8);
	settingEventListener();

	this->initUI();

	return true;
}

void gameRoomUILayer::settingEventListener()
{

	//Ű���� �κ�Ʈ ���
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
		std::abort();
	}

	//���� �̺�Ʈ ����ġ
	//�н���ư �� ����ui Ȱ��ȭ
	callBackListener = EventListenerCustom::create("choicerUIEnabled",
		[=](EventCustom* event) {
		auto btnPass = (MenuItemImage*)magicChoicer->getChildByName("btnPass");
		btnPass->setEnabled(true);
		magicChoicer->setEnabled(true);
		//this->setKeyboardEnabled(true);//Ű���� �̺�Ʈ ��Ȱ��ȭ�� �ٲܰ�
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//���� �Ѿ�� �н���ư �� ����ui ��Ȱ��ȭ
	callBackListener = EventListenerCustom::create("passTurn2NextUser",
		[=](EventCustom* event) {
		std::cout << " �н� �� " << std::endl;
		auto btnPass = (MenuItemImage*)magicChoicer->getChildByName("btnPass");
		btnPass->setEnabled(false);
		magicChoicer->setEnabled(false);
		lastChooseMs = 0;
		//this->setKeyboardEnabled(false);//Ű���� �̺�Ʈ ��Ȱ��ȭ�� �ٲܰ�
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//������ ���ƿ��� magicStone����ui Ȱ��ȭ
	callBackListener = EventListenerCustom::create("myTurn",
		[=](EventCustom* event) {
		std::cout << "my Turn back" << std::endl;
		magicChoicer->setEnabled(true);
		for (int i = 0; i < 8; i++)
		{
			arrBtnSelectStone[i]->setEnabled(true);
		}
		//this->setKeyboardEnabled(true);//Ű���� �̺�Ʈ ��Ȱ��ȭ�� �ٲܰ�
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//���尡 ����Ǹ� ���� ���� UP
	callBackListener = EventListenerCustom::create("roundUp",
		[=](EventCustom* event) {
		roundNum++;
		std::cout << " ���� �� : " << roundNum << std::endl;
		setRound();
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);
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

	auto btnExit = MenuItemImage::create(
		"UISprite/btnExitNormal.png",
		"UISprite/btnExitPress.png",
		"UISprite/btnExitPress.png",
		CC_CALLBACK_0(
			gameRoomUILayer::returnMainMenu,
			this
		)
	);
	btnExit->setScale(0.5f);
	btnExit->setPosition(Vec2(330, 670));

	magicChoicer = cocos2d::Menu::create();
	for (int i = 0; i < 8; i++)
	{
		magicChoicer->addChild(arrBtnSelectStone[i]);
	}

	btnPass->setEnabled(false);
	magicChoicer->addChild(btnPass, 0, "btnPass");
	magicChoicer->alignItemsHorizontallyWithPadding(10.0f);
	magicChoicer->setPosition(Vec2(384, 56));
	magicChoicer->addChild(btnExit);
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
}

void gameRoomUILayer::setRound()
{
	std::cout << "���� ��" << std::endl;

	//���� 50 710
	roundSpr->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(roundNum));
}
void gameRoomUILayer::checkMagic(const int magicStoneNumber)
{
	//ó���� ��ư�޴� ��Ȱ��ȭ - �̺�Ʈ �����Լ��� ���� Ȱ��ȭ��
	magicChoicer->setEnabled(false);
	//this->setKeyboardEnabled(false); //�����Ұ�

	lastChooseMs = magicStoneNumber;
	for (int i = 0; i < lastChooseMs-1; i++)
	{
		arrBtnSelectStone[i]->setEnabled(false);
	}

	EventCustom checkEvent("checkOwnedMagic");
	checkEvent.setUserData((void*)magicStoneNumber);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
}

void gameRoomUILayer::returnMainMenu()
{
	//�˾�â ����
	log("### gameRoomUILayer::returnMainMenu in");
	gameFlowManager::getInstance()->changeScene2MainMenu();
}

void gameRoomUILayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		gameFlowManager::getInstance()->endGame();
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
