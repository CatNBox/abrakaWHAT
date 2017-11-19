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

	//키보드 인벤트 등록
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

	//각종 이벤트 디스패치
	//패스버튼 및 선택ui 활성화
	callBackListener = EventListenerCustom::create("choicerUIEnabled",
		[=](EventCustom* event) {
		auto btnPass = (MenuItemImage*)magicChoicer->getChildByName("btnPass");
		btnPass->setEnabled(true);
		magicChoicer->setEnabled(true);
		//this->setKeyboardEnabled(true);//키보드 이벤트 비활성화로 바꿀것
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//턴이 넘어갈땐 패스버튼 및 선택ui 비활성화
	callBackListener = EventListenerCustom::create("passTurn2NextUser",
		[=](EventCustom* event) {
		std::cout << " 패스 턴 " << std::endl;
		auto btnPass = (MenuItemImage*)magicChoicer->getChildByName("btnPass");
		btnPass->setEnabled(false);
		magicChoicer->setEnabled(false);
		//this->setKeyboardEnabled(false);//키보드 이벤트 비활성화로 바꿀것
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//내턴이 돌아오면 magicStone선택ui 활성화
	callBackListener = EventListenerCustom::create("myTurn",
		[=](EventCustom* event) {
		magicChoicer->setEnabled(true);
		//this->setKeyboardEnabled(true);//키보드 이벤트 비활성화로 바꿀것
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	//라운드가 종료되면 라운드 숫자 UP
	callBackListener = EventListenerCustom::create("roundUp",
		[=](EventCustom* event) {
		roundNum++;
		std::cout << " 라운드 업 : " << roundNum << std::endl;
		setRound();
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(callBackListener, this);

	this->initUI();

	return true;
}

void gameRoomUILayer::initUI()
{
	auto sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprYongyong = Sprite::createWithSpriteFrameName("ms1_yongyong.png");
	if (sprYongyong == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprYongyong is nullptr ");
		std::abort();
	}
	auto btnYongyong = MenuItemImage::create();

	btnYongyong->initWithNormalSprite(
		sprYongyong,
		sprStoneBackward,
		sprYongyong,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::yongyong
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBangrang = Sprite::createWithSpriteFrameName("ms2_bangrang.png");
	if (sprBangrang == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBangrang is nullptr ");
		std::abort();
	}
	auto btnBangrang = MenuItemImage::create();
	btnBangrang->initWithNormalSprite(
		sprBangrang,
		sprStoneBackward,
		sprBangrang,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::bangrang
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprWind = Sprite::createWithSpriteFrameName("ms3_wind.png");
	if (sprWind == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprWind is nullptr ");
		std::abort();
	}
	auto btnWind = MenuItemImage::create();
	btnWind->initWithNormalSprite(
		sprWind,
		sprStoneBackward,
		sprWind,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::wind
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBooung = Sprite::createWithSpriteFrameName("ms4_booung.png");
	if (sprBooung == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBooung is nullptr ");
		std::abort();
	}
	auto btnBooung = MenuItemImage::create();
	btnBooung->initWithNormalSprite(
		sprBooung,
		sprStoneBackward,
		sprBooung,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::booung
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBunpok = Sprite::createWithSpriteFrameName("ms5_bunpok.png");
	if (sprBunpok == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBunpok is nullptr ");
		std::abort();
	}
	auto btnBunpok = MenuItemImage::create();
	btnBunpok->initWithNormalSprite(
		sprBunpok,
		sprStoneBackward,
		sprBunpok,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::bunpok
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprNungang = Sprite::createWithSpriteFrameName("ms6_nungang.png");
	if (sprNungang == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprNungang is nullptr ");
		std::abort();
	}
	auto btnNungang = MenuItemImage::create();
	btnNungang->initWithNormalSprite(
		sprNungang,
		sprStoneBackward,
		sprNungang,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::nungang
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBuljak = Sprite::createWithSpriteFrameName("ms7_buljack.png");
	if (sprBuljak == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBuljak is nullptr ");
		std::abort();
	}
	auto btnBuljak = MenuItemImage::create();
	btnBuljak->initWithNormalSprite(
		sprBuljak,
		sprStoneBackward,
		sprBuljak,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::buljak
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprPostion = Sprite::createWithSpriteFrameName("ms8_postion.png");
	if (sprPostion == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprPostion is nullptr ");
		std::abort();
	}
	auto btnPostion = MenuItemImage::create();
	btnPostion->initWithNormalSprite(
		sprPostion,
		sprStoneBackward,
		sprPostion,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::postion
		)
	);
	
	auto btnPass = MenuItemImage::create();
	btnPass->initWithNormalImage(
		"UISprite/ms_pass_abled_normal.png",
		"UISprite/ms_pass_abled_clicked.png",
		"UISprite/ms_pass_disabled.png",
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::pass
		)
	);

	auto btnExit = MenuItemImage::create();
	btnExit->initWithNormalImage(
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

	magicChoicer = Menu::create(
		btnYongyong,
		btnBangrang,
		btnWind,
		btnBooung,
		btnBunpok,
		btnNungang,
		btnBuljak,
		btnPostion,
		nullptr
	);
	btnPass->setEnabled(false);
	magicChoicer->addChild(btnPass, 0, "btnPass");
	magicChoicer->alignItemsHorizontallyWithPadding(10.0f);
	magicChoicer->setPosition(Vec2(384, 56));
	magicChoicer->addChild(btnExit);
	this->addChild(magicChoicer);

	auto seenCheckerBoard = Sprite::createWithSpriteFrameName("ms_seenBG.png");
	seenCheckerBoard->setPosition(Vec2(340, 460));
	seenCheckerBoard->setRotation(90.0f);

	this->addChild(seenCheckerBoard);

	roundSpr = gameFlowManager::getInstance()->getNumSprite(roundNum);
	setRound();

	this->addChild(roundSpr);
}

void gameRoomUILayer::setRound()
{
	std::cout << "라운드 업" << std::endl;
	//라운드 50 710
	auto temp = gameFlowManager::getInstance()->getNumSprite(roundNum);
	
	roundSpr->setTextureRect(temp->getTextureRect());
	roundSpr->setPosition(Vec2(50, 710));
	roundSpr->setScale(0.8f);
}

void gameRoomUILayer::checkMagic(const int magicStoneNumber)
{
	//처리중 버튼메뉴 비활성화 - 이벤트 람다함수를 통해 활성화됨
	magicChoicer->setEnabled(false);
	//this->setKeyboardEnabled(false); //변경할것

	EventCustom checkEvent("checkOwnedMagic");
	checkEvent.setUserData((void*)magicStoneNumber);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
}

void gameRoomUILayer::returnMainMenu()
{
	//팝업창 구현
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
		checkMagic(gameMetaData::msType::postion);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_F5)
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("initRound");
	}
}

void gameRoomUILayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
}
