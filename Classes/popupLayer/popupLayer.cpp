#include "popupLayer.h"
#include "gameObject\gameMetaData.h"
#include "managers\gameFlowManager.h"
#include <iostream>

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

	eventListener = cocos2d::EventListenerCustom::create("popupEndGame",
		CC_CALLBACK_1(popupLayer::setEndGame, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	eventListener = cocos2d::EventListenerCustom::create("popupEndRound",
		CC_CALLBACK_1(popupLayer::setEndRound, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	eventListener = cocos2d::EventListenerCustom::create("popupWarning",
		CC_CALLBACK_1(popupLayer::setWarning, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	playerSpr.resize(4);

	this->setVisible(false);

	/*
	auto tempOkSpr = cocos2d::Sprite::createWithSpriteFrameName("sprOk.png");
	auto tempCancelSpr = cocos2d::Sprite::createWithSpriteFrameName("sprCancel.png");
	tempCancelSpr->setColor(cocos2d::Color3B(150, 150, 150));
	btnOk = cocos2d::MenuItemImage::create();
	btnOk->initWithNormalSprite(tempOkSpr, tempCancelSpr, tempCancelSpr)
	*/
	this->setCascadeOpacityEnabled(true);

	return true;
}

void popupLayer::setBGSpr()
{
	auto BGSpr = cocos2d::Sprite::createWithSpriteFrameName("popupLayerBG.png");
	BGSpr->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS));
	this->addChild(BGSpr, gameMetaData::layerZOrder::backGroundZ);

	this->setVisible(true);
}

void popupLayer::setPopupBoard(enum class gameMetaData::popupBoardSize popupSizeEnum)
{
	gameMetaData::popupBoardSize key = popupSizeEnum;
	if (gameMetaData::mapPopupBoardSpr.find(key) != gameMetaData::mapPopupBoardSpr.end())
	{
		auto popupBoard = cocos2d::Sprite::createWithSpriteFrameName(gameMetaData::mapPopupBoardSpr.at(key));
		popupBoard->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS));
		this->addChild(popupBoard, gameMetaData::layerZOrder::objZ0);
	}
}

void popupLayer::setDisplayPlayer()
{
	for (int playerNum = 0; playerNum < 4; playerNum++)
	{
		auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName("sprPlayer.png");
		auto tempNumSpr = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempNumSpr->setTextureRect(gameFlowManager::getInstance()->getNumSprRect((playerNum + 1)));
		tempNumSpr->setPosition(cocos2d::Vec2(280.0f + 90.0f, STDAXIS - (63.0f * playerNum)));
		tempSpr->setPosition(cocos2d::Vec2(280.0f, STDAXIS - (65.0f * playerNum)));
		tempNumSpr->setScale(0.6f);
		tempSpr->setScale(0.6f);

		playerSpr.at(playerNum).first = tempSpr;
		playerSpr.at(playerNum).second = tempNumSpr;

		this->addChild(tempSpr, gameMetaData::layerZOrder::objZ1);
		this->addChild(tempNumSpr, gameMetaData::layerZOrder::objZ1);
	}
}

void popupLayer::initSprCntNum()
{
	sprCntNum = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
	sprCntNum->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(5));
	sprCntNum->setVisible(false);
	sprCntNum->setPosition(STDAXIS, STDAXIS);
}

cocos2d::Sequence * popupLayer::cntNumAction(int cntStartNum)
{
	cocos2d::Vector<cocos2d::FiniteTimeAction*> vecAction;

	//startNum ~ 1
	for (int i = cntStartNum; i > 0; i--)
	{
		auto tempFuncChangeCntNum = cocos2d::CallFunc::create([=]() {
			sprCntNum->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(i));
		});
		vecAction.pushBack(tempFuncChangeCntNum);

		auto tempDelay = cocos2d::DelayTime::create(1.0f);
		vecAction.pushBack(tempDelay);
	}
	//zero
	auto tempFuncChangeCntNum = cocos2d::CallFunc::create([=]() {
		sprCntNum->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(0));
	});
	vecAction.pushBack(tempFuncChangeCntNum);

	auto seq = cocos2d::Sequence::create(vecAction);

	return seq;
}

void popupLayer::setEndGame(cocos2d::EventCustom* checkOwnedMagicEvent)
{
	this->setOpacity(255);
	this->removeAllChildren();
	setBGSpr();
	setPopupBoard(gameMetaData::popupBoardSize::popup400500);

	//get Data
	std::array<int, 4>* arrEndScore;
	arrEndScore = (std::array<int,4>*)checkOwnedMagicEvent->getUserData();
	std::for_each(arrEndScore->begin(), arrEndScore->end(), [](int i) {std::cout << i << std::endl; });

	//display player
	setDisplayPlayer();

	this->setVisible(true);
}

void popupLayer::setEndRound(cocos2d::EventCustom* checkOwnedMagicEvent)
{
	this->setOpacity(255);
	this->removeAllChildren();
	setBGSpr();
	setPopupBoard(gameMetaData::popupBoardSize::popup400500);

	//get Data
	std::array<int, 5>* arrRoundScore;
	arrRoundScore = (std::array<int, 5>*)checkOwnedMagicEvent->getUserData();

	//display player
	setDisplayPlayer();

	//if player live score is zero, sprite color turn gray
	for (int playerNum = 0; playerNum < (int)arrRoundScore->size(); playerNum++)
	{
		if (arrRoundScore->at(playerNum) == 0)
		{
			playerSpr.at(playerNum).first->setColor(cocos2d::Color3B::GRAY);
			playerSpr.at(playerNum).second->setColor(cocos2d::Color3B::GRAY);
		}
	}

	//display markSprite
	auto sprMarkLive = cocos2d::Sprite::createWithSpriteFrameName("sprLive.png");
	sprMarkLive->setScale(0.3f);
	sprMarkLive->setPosition(cocos2d::Vec2(410.0f, STDAXIS + 50.0f));
	addChild(sprMarkLive, gameMetaData::layerZOrder::objZ1);
	auto sprMarkWin = cocos2d::Sprite::createWithSpriteFrameName("sprWin.png");
	sprMarkWin->setScale(0.3f);
	sprMarkWin->setPosition(cocos2d::Vec2(460.0f, STDAXIS + 50.0f));
	addChild(sprMarkWin, gameMetaData::layerZOrder::objZ1);
	auto sprMarkBooung = cocos2d::Sprite::createWithSpriteFrameName("sprBooung.png");
	sprMarkBooung->setScale(0.3f);
	sprMarkBooung->setPosition(cocos2d::Vec2(525.0f, STDAXIS + 50.0f));
	addChild(sprMarkBooung, gameMetaData::layerZOrder::objZ1);

	//display score inc/dec each live/victory/booung
	for (int playerNum = 0; playerNum < 4; playerNum++)
	{
		int liveScore = 0, winScore = 0, booungScore = 0;
		if (arrRoundScore->at(playerNum) > 0)
		{
			liveScore = 1;
			arrRoundScore->at(playerNum) -= liveScore;

			if (arrRoundScore->at(4) == playerNum)
			{
				winScore = 2;
				arrRoundScore->at(playerNum) -= winScore;
			}

			booungScore = arrRoundScore->at(playerNum);
		}

		auto tempSprLiveScore = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempSprLiveScore->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(liveScore));
		auto tempSprWinScore = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempSprWinScore->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(winScore));
		auto tempSprBooungScore = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempSprBooungScore->setTextureRect(gameFlowManager::getInstance()->getNumSprRect(booungScore));

		//yAxis = playerSprite, xAxis = sprMark
		tempSprLiveScore->setPosition(cocos2d::Vec2(410.0f, STDAXIS - (63.0f * playerNum)));
		tempSprWinScore->setPosition(cocos2d::Vec2(460.0f, STDAXIS - (63.0f * playerNum)));
		tempSprBooungScore->setPosition(cocos2d::Vec2(525.0f, STDAXIS - (63.0f * playerNum)));

		tempSprLiveScore->setScale(0.8f);
		tempSprWinScore->setScale(0.8f);
		tempSprBooungScore->setScale(0.8f);

		//if player live score is zero, sprite color turn gray
		if (liveScore == 0)
		{
			tempSprLiveScore->setColor(cocos2d::Color3B(150, 150, 150));
			tempSprWinScore->setColor(cocos2d::Color3B(150, 150, 150));
			tempSprBooungScore->setColor(cocos2d::Color3B(150, 150, 150));
		}

		this->addChild(tempSprLiveScore, gameMetaData::layerZOrder::objZ1);
		this->addChild(tempSprWinScore, gameMetaData::layerZOrder::objZ1);
		this->addChild(tempSprBooungScore, gameMetaData::layerZOrder::objZ1);
	}

	//close popup after 3sec
	initSprCntNum();
	sprCntNum->setPosition(cocos2d::Vec2(STDAXIS, 520.0f));
	sprCntNum->setVisible(true);
	this->addChild(sprCntNum, gameMetaData::layerZOrder::objZ1);
	
	auto seqChangeCntSpr = cntNumAction(5);
	sprCntNum->runAction(seqChangeCntSpr);
	
	this->scheduleOnce([=](float d) 
	{
		auto layerFadeOut = cocos2d::FadeOut::create(0.3f);
		auto layerHide = cocos2d::Hide::create();
		auto layerCloseSeq = cocos2d::Sequence::create(layerFadeOut, layerHide, NULL);
		this->runAction(layerFadeOut);

		cocos2d::EventCustom initRound("initRound");
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&initRound);
	}, 5.0f, "popupEndRoundClose");

	this->setVisible(true);
}

void popupLayer::setWarning(cocos2d::EventCustom* checkOwnedMagicEvent)
{
	this->removeAllChildren();
	setBGSpr();

	this->setVisible(true);
}
