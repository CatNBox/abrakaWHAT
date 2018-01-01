#include "popupLayer.h"
#include "gameObject\gameMetaData.h"
#include "managers\gameFlowManager.h"

popupLayer::popupLayer()
{
}

popupLayer::~popupLayer()
{
}

bool popupLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	eventListener = cocos2d::EventListenerCustom::create("endGame",
		CC_CALLBACK_1(popupLayer::setEndGame, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	eventListener = cocos2d::EventListenerCustom::create("endRound",
		CC_CALLBACK_1(popupLayer::setEndRound, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	eventListener = cocos2d::EventListenerCustom::create("endWarning",
		CC_CALLBACK_1(popupLayer::setWarning, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	this->setVisible(false);

	return true;
}

void popupLayer::setBGSpr()
{
	auto BGSpr = cocos2d::Sprite::createWithSpriteFrameName("popupLayerBG.png");
	BGSpr->setPosition(cocos2d::Vec2(392, 392));
	this->addChild(BGSpr);
}

void popupLayer::setEndGame(cocos2d::EventCustom* checkOwnedMagicEvent)
{
	this->removeAllChildren();
	setBGSpr();
}

void popupLayer::setEndRound(cocos2d::EventCustom* checkOwnedMagicEvent)
{
	this->removeAllChildren();
	setBGSpr();
}

void popupLayer::setWarning(cocos2d::EventCustom* checkOwnedMagicEvent)
{
	this->removeAllChildren();
	setBGSpr();
}
