#include "setupScene\setupLoadingLayer.h"
#include "managers\gameFlowManager.h"
#include "gameObject\gameMetaData.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool setupLodingLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("AbrakaWHAT", "fonts/Marker Felt.ttf", 36);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	int bgIdx = gameFlowManager::getInstance()->getRandomInt(0, 3);
	auto sprite = Sprite::create(gameMetaData::arrBGSprite.at(bgIdx));

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	log("### preload sprite / call in setupLoadingLayer");
	gameFlowManager::getInstance()->preloadSprites();

	this->scheduleOnce(SEL_SCHEDULE(&setupLodingLayer::callChangeScene2MainMenu), 1.5f);


	return true;
}

void setupLodingLayer::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

}

void setupLodingLayer::callChangeScene2MainMenu(float d)
{
	log("### setupLoadingLayer::callChangeScene2MainMenu in");
	gameFlowManager::getInstance()->changeScene2MainMenu();
}
