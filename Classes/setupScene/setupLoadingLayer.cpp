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

	int bgIdx = gameFlowManager::getInstance().getRandomInt(0, 3);
	auto sprite = Sprite::create(gameMetaData::arrBGSprite.at(bgIdx));

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	this->preloadSprites();
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
	gameFlowManager::getInstance().changeScene2MainMenu();
}

void setupLodingLayer::preloadSprites()
{
	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("magicStones.plist", "magicStones.png");
	spriteCache->addSpriteFramesWithFile("seenChecker.plist", "seenChecker.png");

	auto tempSpr = Sprite::create("UISprite/spr_number.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "spr_number.png");

	tempSpr->initWithFile("lpToken/lpToken00.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken00.png");
	tempSpr->initWithFile("lpToken/lpToken01.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken01.png");
	tempSpr->initWithFile("lpToken/lpToken02.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken02.png");
	tempSpr->initWithFile("lpToken/lpToken03.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken03.png");
	tempSpr->initWithFile("lpToken/lpToken04.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken04.png");
	tempSpr->initWithFile("lpToken/lpToken05.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken05.png");

	tempSpr->initWithFile("msEffectSpr/yongyongRedScreen.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "yongyongRedScreen.png");

	tempSpr->initWithFile("UISprite/sprScore.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprScore.png");
	tempSpr->initWithFile("UISprite/sprPlayer.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprPlayer.png");

	tempSpr->initWithFile("popupLayerSpr/popupLayerBG.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popupLayerBG.png");
	tempSpr->initWithFile("popupLayerSpr/popup400500.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popup400500.png");
	tempSpr->initWithFile("popupLayerSpr/popup400245.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popup400245.png");
	tempSpr->initWithFile("UISprite/uiPopMessage.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "uiPopMessage.png");

	tempSpr->initWithFile("UISprite/sprCancel.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprCancel.png");
	tempSpr->initWithFile("UISprite/sprOk.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprOk.png");
	tempSpr->initWithFile("UISprite/sprLive.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprLive.png");
	tempSpr->initWithFile("UISprite/sprWin.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprWin.png");
	tempSpr->initWithFile("UISprite/sprBooung.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprBooung.png");
}
