#include "setupScene\setupLoadingLayer.h"
#include "mainMenuScene\mainMenuScene.h"
#include "gameObject\gameMetaData.h"
#include "managers\spriteManager.h"

// on "init" you need to initialize your instance
bool setupLodingLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto label = cocos2d::Label::createWithTTF("AbrakaWHAT", "fonts/Marker Felt.ttf", 36);

	// position the label on the center of the screen
	label->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	int bgIdx = inlineFunc::getRandomInt(0, 3);
	auto sprite = cocos2d::Sprite::create(gameMetaData::arrBGSprite.at(bgIdx));

	// position the sprite on the center of the screen
	sprite->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	//preload
	sprManager = new spriteManager;
	sprManager->preLoadingSprites();
	delete sprManager;

	//check and set INI file
	if (!checkExistINI())
	{
		setINI();
	}

	//change scene
	this->scheduleOnce(cocos2d::SEL_SCHEDULE(&setupLodingLayer::changeScene2MainMenu), 1.5f);

	return true;
}

void setupLodingLayer::changeScene2MainMenu(float d)
{
	auto mainMenuScene = cocos2d::TransitionFade::create(1.0f, mainMenuScene::createScene());
	cocos2d::Director::getInstance()->replaceScene(mainMenuScene);
}

bool setupLodingLayer::checkExistINI()
{
	if (std::FILE *file = std::fopen("option.ini", "r"))
	{
		return true;
	}
	return false;
}

void setupLodingLayer::setINI()
{
	//if not exist ini file make one by default value
	wchar_t buf2ini[32];

	_itow_s(gameMetaData::defaultPlayerCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"playerCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultSecretCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"secretCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultYongCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"yongCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultBangrangCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"bangrangCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultWindCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"windCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultBooungCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"booungCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultBunpokCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"bunpokCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultNungangCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"nungangCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultBuljakCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"buljakCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultPotionCnt, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"potionCnt", buf2ini, L".\\option.ini");
	_itow_s(gameMetaData::defaultMaxLifePoint, buf2ini, 10);
	WritePrivateProfileString(L"RoundOption", L"maxLifePoint", buf2ini, L".\\option.ini");
}
