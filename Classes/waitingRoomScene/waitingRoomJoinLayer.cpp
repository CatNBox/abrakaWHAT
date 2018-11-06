#include "waitingRoomJoinLayer.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"

using namespace cocos2d;

bool waitingRoomJoinLayer::init(gameMetaData::gameMode modeFlag)
{
	if (!Layer::init())
	{
		return false;
	}
	curMode = modeFlag;
	ipAddr = "000.000.000.000";

	sprManager = new spriteManager;
	actManager = actionManager::getInstance();

	initUI();

	return true;
}

waitingRoomJoinLayer * waitingRoomJoinLayer::createWithParam(gameMetaData::gameMode modeFlag)
{
	waitingRoomJoinLayer *pRet = new(std::nothrow) waitingRoomJoinLayer();
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

void waitingRoomJoinLayer::initUI()
{
	// init ipNum to 000.000.000.000
	int addWeightValue = 0;
	for (int i = 0; i < 12; i++)
	{
		if (i % 3 == 0)
		{
			addWeightValue += 10;
		}
		ipAddrSpr[i] = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		ipAddrSpr[i]->setTextureRect(sprManager->getNumSprRect(0));
		ipAddrSpr[i]->setAnchorPoint(Vec2::ZERO);
		ipAddrSpr[i]->setPosition(Vec2(54 + 50*i + addWeightValue, 580));
		this->addChild(ipAddrSpr[i], gameMetaData::layerZOrder::objZ1);
	}
	for (int i = 0; i < 3; i++)
	{
		//dot spr
		auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName("sprDot.png");
		tempSpr->setScale(0.8f);
		tempSpr->setAnchorPoint(Vec2::ZERO);
		tempSpr->setPosition(Vec2(196 + 160 * i, 572));
		this->addChild(tempSpr, gameMetaData::layerZOrder::objZ0);
	}

	// clipboard copy btn
	auto tempNormalCFCSpr = Sprite::createWithSpriteFrameName("btnCopyFromClip.png");
	auto tempClickedCFCSpr = Sprite::createWithSpriteFrameName("btnCopyFromClip.png");
	tempClickedCFCSpr->setColor(Color3B(125, 125, 125));
	auto tempDisabledCFCSpr = Sprite::createWithSpriteFrameName("btnCopyFromClip.png");
	tempDisabledCFCSpr->setColor(Color3B(95, 95, 95));
	auto btnCopyFromClipboard = MenuItemImage::create();
	btnCopyFromClipboard->initWithNormalSprite(
		tempNormalCFCSpr,
		tempClickedCFCSpr,
		tempDisabledCFCSpr,
		CC_CALLBACK_0(
			waitingRoomJoinLayer::displayIpAddrFromClip,
			this
		)
	);

	btnCopyFromClipboard->setAnchorPoint(Vec2::ZERO);
	btnCopyFromClipboard->setScale(0.7f);
	btnCopyFromClipboard->setPosition(Vec2(100, 500));

	auto tempMenu = Menu::createWithItem(btnCopyFromClipboard);
	tempMenu->setAnchorPoint(Vec2::ZERO);
	tempMenu->setPosition(Vec2::ZERO);

	this->addChild(tempMenu);

	// try join btn

}

void waitingRoomJoinLayer::ipAddrDigitUp(const int sprIndex)
{
	//ipAddr[index] value + 1
	//if curValue is 9, change value to 0

	//call getNumRect(ipAddr[index])
}

void waitingRoomJoinLayer::ipAddrDigitDown(const int sprIndex)
{
	//ipAddr[index] value - 1
	//if curValue is 0, change value to 9

	//call getNumRect(ipAddr[index])
}

void waitingRoomJoinLayer::displayIpAddrFromClip()
{
}
