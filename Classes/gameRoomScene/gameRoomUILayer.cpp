#include "gameRoomUILayer.h"
#include "managers\gameFlowManager.h"
#include "gameObject\gameMetaData.h"

using namespace cocos2d;

bool gameRoomUILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

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
			(const int)gameMetaData::magicStones::yongyong
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
			(const int)gameMetaData::magicStones::bangrang
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
			(const int)gameMetaData::magicStones::wind
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
			(const int)gameMetaData::magicStones::booung
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
			(const int)gameMetaData::magicStones::bunpok
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
			(const int)gameMetaData::magicStones::nungang
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
			(const int)gameMetaData::magicStones::buljak
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
			(const int)gameMetaData::magicStones::postion
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
			(const int)gameMetaData::magicStones::pass
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
}

void gameRoomUILayer::checkMagic(const int magicStoneNumber)
{
	EventCustom checkEvent("checkOwnedMagic");
	checkEvent.setUserData((void*)magicStoneNumber);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&checkEvent);
}

void gameRoomUILayer::returnMainMenu()
{
	//ÆË¾÷Ã¢ ±¸Çö
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

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_2)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_3)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_4)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_5)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_6)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_7)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_8)
	{

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_F5)
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("initRound");
	}
}

void gameRoomUILayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
}
