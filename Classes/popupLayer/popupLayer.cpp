#include "popupLayer.h"
#include "mainMenuScene\mainMenuScene.h"
#include "managers\spriteManager.h"
#include "gameObject\gameMetaData.h"
#include "managers/networkManager.h"

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
			
	this->setCascadeOpacityEnabled(true);

	sprManager = new spriteManager;

	return true;
}

void popupLayer::callbackPopupClose()
{
	auto layerFadeOut = cocos2d::FadeOut::create(0.3f);
	auto layerHide = cocos2d::Hide::create();
	auto layerCloseSeq = cocos2d::Sequence::create(layerFadeOut, layerHide, NULL);
	this->runAction(layerFadeOut);
}

void popupLayer::callbackGameExit()
{
	callbackPopupClose();

	//if gameMode::host/guest, must call networkManager->close()
	networkManager::getInstance()->close();

	this->scheduleOnce([=](float d) 
	{
		auto mainMenuScene = cocos2d::TransitionFade::create(1.0f, mainMenuScene::createScene());
		cocos2d::Director::getInstance()->replaceScene(mainMenuScene);
	}, 0.5f, "gameExitFromPopup");
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
		auto tempSprRect = sprManager->getNumSprRect(playerNum + 1);
		tempNumSpr->setTextureRect(tempSprRect);
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

void popupLayer::initSprCntdwnNum()
{
	sprCntdwnNum = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
	auto tempSprRect = sprManager->getNumSprRect(5);
	sprCntdwnNum->setTextureRect(tempSprRect);
	sprCntdwnNum->setVisible(false);
	sprCntdwnNum->setPosition(STDAXIS, STDAXIS);
}

cocos2d::Sequence * popupLayer::cntNumAction(int cntStartNum)
{
	cocos2d::Vector<cocos2d::FiniteTimeAction*> vecAction;

	//startNum ~ 1
	for (int i = cntStartNum; i > 0; i--)
	{
		auto tempFuncChangeCntNum = cocos2d::CallFunc::create([=]() {
			sprCntdwnNum->setTextureRect(sprManager->getNumSprRect(i));
		});
		vecAction.pushBack(tempFuncChangeCntNum);

		auto tempDelay = cocos2d::DelayTime::create(1.0f);
		vecAction.pushBack(tempDelay);
	}
	//zero
	auto tempFuncChangeCntNum = cocos2d::CallFunc::create([=]() {
		sprCntdwnNum->setTextureRect(sprManager->getNumSprRect(0));
	});
	vecAction.pushBack(tempFuncChangeCntNum);

	auto seq = cocos2d::Sequence::create(vecAction);

	return seq;
}

void popupLayer::setEndGame(cocos2d::EventCustom* endGameEvent)
{
	this->setOpacity(255);
	this->removeAllChildren();
	setBGSpr();
	setPopupBoard(gameMetaData::popupBoardSize::popup400500);

	//get Data
	std::array<int, 4>* arrEndScore;
	arrEndScore = (std::array<int,4>*)endGameEvent->getUserData();

	//display player
	setDisplayPlayer();

	//display score
	for (int playerNum = 0; playerNum < arrEndScore->size(); playerNum++)
	{
		int tempScore = arrEndScore->at(playerNum);
		if (tempScore > 9)
		{
			auto temp10Spr = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
			temp10Spr->setTextureRect(sprManager->getNumSprRect(1));
			temp10Spr->setPosition(cocos2d::Vec2(420.0f, STDAXIS - (63.0f*playerNum)));
			temp10Spr->setScale(0.6f);
			this->addChild(temp10Spr, gameMetaData::layerZOrder::objZ1);
		}
		auto tempScoreSpr = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempScoreSpr->setTextureRect(sprManager->getNumSprRect(tempScore%10));
		tempScoreSpr->setPosition(cocos2d::Vec2(450.0f, STDAXIS - (63.0f*playerNum)));
		tempScoreSpr->setScale(0.6f);
		this->addChild(tempScoreSpr, gameMetaData::layerZOrder::objZ1);
	}

	//setBtn	
	auto tempOkSprNormal = cocos2d::Sprite::createWithSpriteFrameName("sprOk.png");
	auto tempOkSprPress = cocos2d::Sprite::createWithSpriteFrame(tempOkSprNormal->getSpriteFrame());
	auto tempOkSprDisable = cocos2d::Sprite::createWithSpriteFrame(tempOkSprNormal->getSpriteFrame());
	tempOkSprPress->setColor(cocos2d::Color3B::GRAY);
	tempOkSprDisable->setColor(cocos2d::Color3B(50, 50, 50));
	btnOk = cocos2d::MenuItemImage::create();
	btnOk->initWithNormalSprite(tempOkSprNormal, tempOkSprPress, tempOkSprDisable, CC_CALLBACK_0(popupLayer::callbackGameExit, this));
	btnMenu = cocos2d::Menu::create();
	btnMenu->addChild(btnOk);
	btnMenu->setPosition(cocos2d::Vec2(STDAXIS, 520.0f));
	this->addChild(btnMenu, gameMetaData::layerZOrder::objZ1);

	this->setVisible(true);
}

void popupLayer::setEndRound(cocos2d::EventCustom* endRoundEvent)
{
	this->setOpacity(255);
	this->removeAllChildren();
	setBGSpr();
	setPopupBoard(gameMetaData::popupBoardSize::popup400500);

	//get Data
	std::array<int, 5>* arrRoundScore;
	arrRoundScore = (std::array<int, 5>*)endRoundEvent->getUserData();
	
	//display player
	setDisplayPlayer();

	//if player live score is zero, sprite color turn gray
	for (int playerNum = 0; playerNum < playerCnt; playerNum++)
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
	for (int playerNum = 0; playerNum < playerCnt; playerNum++)
	{
		int liveScore = 0, winScore = 0, booungScore = 0;
		if (arrRoundScore->at(playerNum) > 0)
		{
			liveScore = 1;
			arrRoundScore->at(playerNum) -= liveScore;

			if (arrRoundScore->at(roundWinnerIdx) == playerNum)
			{
				winScore = 2;
				arrRoundScore->at(playerNum) -= winScore;
			}

			booungScore = arrRoundScore->at(playerNum);
		}

		auto tempSprLiveScore = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempSprLiveScore->setTextureRect(sprManager->getNumSprRect(liveScore));
		auto tempSprWinScore = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempSprWinScore->setTextureRect(sprManager->getNumSprRect(winScore));
		auto tempSprBooungScore = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		tempSprBooungScore->setTextureRect(sprManager->getNumSprRect(booungScore));

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
			tempSprLiveScore->setColor(cocos2d::Color3B::GRAY);
			tempSprWinScore->setColor(cocos2d::Color3B::GRAY);
			tempSprBooungScore->setColor(cocos2d::Color3B::GRAY);
		}

		this->addChild(tempSprLiveScore, gameMetaData::layerZOrder::objZ1);
		this->addChild(tempSprWinScore, gameMetaData::layerZOrder::objZ1);
		this->addChild(tempSprBooungScore, gameMetaData::layerZOrder::objZ1);
	}

	//close popup after 3sec
	initSprCntdwnNum();
	sprCntdwnNum->setPosition(cocos2d::Vec2(STDAXIS, 520.0f));
	sprCntdwnNum->setVisible(true);
	this->addChild(sprCntdwnNum, gameMetaData::layerZOrder::objZ1);
	
	auto seqChangeCntSpr = cntNumAction(5);
	sprCntdwnNum->runAction(seqChangeCntSpr);
	
	this->scheduleOnce([=](float d) 
	{
		callbackPopupClose();

		cocos2d::EventCustom initRound("initRound");
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&initRound);
	}, 5.0f, "popupEndRoundClose");

	this->setVisible(true);
}

void popupLayer::setWarning(cocos2d::EventCustom* warningEvent)
{
	this->setOpacity(255);
	this->removeAllChildren();
	setBGSpr();
	setPopupBoard(gameMetaData::popupBoardSize::popup400245);

	//get data
	gameMetaData::warningCode warningFlag = *(gameMetaData::warningCode*)warningEvent->getUserData();

	//setTextLabel
	setWarningLabel(warningFlag);

	//setBtn	
	setWarningBtn(warningFlag);

	this->setVisible(true);
}

void popupLayer::setWarningLabel(gameMetaData::warningCode warningCodeEnum)
{
	if (warningCodeEnum == gameMetaData::warningCode::exitBtnWarning)
	{
		auto tempLabel = cocos2d::Label::createWithTTF("Are you sure exiting current game?", "fonts/Xiomara-Script.ttf", 26.0f,cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
		//tempLabel->setBMFontSize(14.0f);
		tempLabel->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS + 50.0f));
		this->addChild(tempLabel, gameMetaData::layerZOrder::objZ1);
	}
	else if (warningCodeEnum == gameMetaData::warningCode::wrongIpFormat)
	{
		auto tempLabel1 = cocos2d::Label::createWithTTF("Not match IP Format. plz check Text.", "fonts/Marker Felt.ttf", 26.0f, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
		auto tempLabel2 = cocos2d::Label::createWithTTF("Right format e.g : 012.012.255.255", "fonts/Marker Felt.ttf", 26.0f, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
		//tempLabel->setBMFontSize(14.0f);
		tempLabel1->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS + 65.0f));
		tempLabel2->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS + 30.0f));
		this->addChild(tempLabel1, gameMetaData::layerZOrder::objZ1);
		this->addChild(tempLabel2, gameMetaData::layerZOrder::objZ1);
	}
}

void popupLayer::setWarningBtn(gameMetaData::warningCode warningCodeEnum)
{
	if (warningCodeEnum == gameMetaData::warningCode::exitBtnWarning)
	{
		auto tempOkSprNormal = cocos2d::Sprite::createWithSpriteFrameName("sprOk.png");
		auto tempOkSprPress = cocos2d::Sprite::createWithSpriteFrame(tempOkSprNormal->getSpriteFrame());
		auto tempOkSprDisable = cocos2d::Sprite::createWithSpriteFrame(tempOkSprNormal->getSpriteFrame());
		tempOkSprPress->setColor(cocos2d::Color3B::GRAY);
		tempOkSprDisable->setColor(cocos2d::Color3B(50, 50, 50));
		btnOk = cocos2d::MenuItemImage::create();
		btnOk->initWithNormalSprite(tempOkSprNormal, tempOkSprPress, tempOkSprDisable, CC_CALLBACK_0(popupLayer::callbackGameExit, this));
		btnOk->setScale(0.6f);

		auto tempCancelSprNormal = cocos2d::Sprite::createWithSpriteFrameName("sprCancel.png");
		auto tempCancelSprPress = cocos2d::Sprite::createWithSpriteFrame(tempCancelSprNormal->getSpriteFrame());
		auto tempCancelSprDisable = cocos2d::Sprite::createWithSpriteFrame(tempCancelSprNormal->getSpriteFrame());
		tempCancelSprPress->setColor(cocos2d::Color3B::GRAY);
		tempCancelSprDisable->setColor(cocos2d::Color3B(50, 50, 50));
		btnCancle = cocos2d::MenuItemImage::create();
		btnCancle->initWithNormalSprite(tempCancelSprNormal, tempCancelSprPress, tempCancelSprDisable, CC_CALLBACK_0(popupLayer::callbackPopupClose, this));
		btnCancle->setScale(0.6f);

		btnMenu = cocos2d::Menu::create(btnOk, btnCancle, NULL);
		btnMenu->alignItemsHorizontallyWithPadding(3.0f);
		btnMenu->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS - 30.0f));
		this->addChild(btnMenu, gameMetaData::layerZOrder::objZ1);
	}
	else if (warningCodeEnum == gameMetaData::warningCode::wrongIpFormat)
	{
		auto tempOkSprNormal = cocos2d::Sprite::createWithSpriteFrameName("sprOk.png");
		auto tempOkSprPress = cocos2d::Sprite::createWithSpriteFrame(tempOkSprNormal->getSpriteFrame());
		auto tempOkSprDisable = cocos2d::Sprite::createWithSpriteFrame(tempOkSprNormal->getSpriteFrame());
		tempOkSprPress->setColor(cocos2d::Color3B::GRAY);
		tempOkSprDisable->setColor(cocos2d::Color3B(50, 50, 50));
		btnOk = cocos2d::MenuItemImage::create();
		btnOk->initWithNormalSprite(tempOkSprNormal, tempOkSprPress, tempOkSprDisable, CC_CALLBACK_0(popupLayer::callbackPopupClose, this));
		btnOk->setScale(0.6f);

		btnMenu = cocos2d::Menu::create(btnOk, NULL);
		btnMenu->alignItemsHorizontallyWithPadding(3.0f);
		btnMenu->setPosition(cocos2d::Vec2(STDAXIS, STDAXIS - 40.0f));
		this->addChild(btnMenu, gameMetaData::layerZOrder::objZ1);
	}
}
