#include "waitingRoomJoinLayer.h"
#include "managers\spriteManager.h"
#include "managers\actionManager.h"
#include "managers/networkManager.h"

using namespace cocos2d;

bool waitingRoomJoinLayer::init(gameMetaData::gameMode modeFlag)
{
	if (!Layer::init())
	{
		return false;
	}
	curGameMode = modeFlag;
	ipAddrSprFormat = "000.000.000.000";
	ipAddrNetFormat = "0.0.0.0";
	
	sprManager = new spriteManager;
	actManager = actionManager::getInstance();
	netManager = networkManager::getInstance();

	initTimeout(gameMetaData::defaultTimeoutCnt);
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

void waitingRoomJoinLayer::update(float dTime)
{
	//check timeOut default 10.0f
	this->_timeout -= dTime;
	if (this->_timeout <= 0)
	{
		this->unscheduleUpdate();
	}

	//check flags in networkManager
	if (netManager->getMyClientConnectionState() == gameMetaData::netPlayerState::connected)
	{
		callbackJoinSuccess();
	}
}

void waitingRoomJoinLayer::initUI()
{
	btnMenu = Menu::create();
	btnMenu->setAnchorPoint(Vec2::ZERO);
	btnMenu->setPosition(Vec2::ZERO);

	this->addChild(btnMenu);

	// init ipNum to 000.000.000.000
	int addWeightValue = 0;
	for (int i = 0; i < 12; i++)
	{
		if (i % 3 == 0)
		{
			addWeightValue += 10;
		}
		//sprite IPAddr
		ipAddrSpr[i] = cocos2d::Sprite::createWithSpriteFrameName("spr_number.png");
		ipAddrSpr[i]->setTextureRect(sprManager->getNumSprRect(0));
		ipAddrSpr[i]->setAnchorPoint(Vec2::ZERO);
		ipAddrSpr[i]->setPosition(Vec2(54 + 50*i + addWeightValue, 580));
		this->addChild(ipAddrSpr[i], gameMetaData::layerZOrder::objZ1);

		//btn UpArrow
		auto tempNormalUASpr = Sprite::createWithSpriteFrameName("sprUpArrow.png");
		auto tempClickedUASpr = Sprite::createWithSpriteFrameName("sprUpArrow.png");
		tempClickedUASpr->setColor(Color3B(125, 125, 125));
		auto tempDisabledUASpr = Sprite::createWithSpriteFrameName("sprUpArrow.png");
		tempDisabledUASpr->setColor(Color3B(95, 95, 95));
		auto tempUA = MenuItemImage::create();
		tempUA->initWithNormalSprite(
			tempNormalUASpr,
			tempClickedUASpr,
			tempDisabledUASpr,
			CC_CALLBACK_0(
				waitingRoomJoinLayer::ipAddrDigitUp,
				this,
				i
			)
		);
		tempUA->setScale(0.5f);
		tempUA->setAnchorPoint(Vec2::ZERO);
		tempUA->setPosition(Vec2(40 + 50 * i + addWeightValue, 635));
		btnMenu->addChild(tempUA);

		//btn DownArrow
		auto tempNormalDASpr = Sprite::createWithSpriteFrameName("sprUpArrow.png");
		auto tempClickedDASpr = Sprite::createWithSpriteFrameName("sprUpArrow.png");
		tempClickedDASpr->setColor(Color3B(125, 125, 125));
		auto tempDisabledDASpr = Sprite::createWithSpriteFrameName("sprUpArrow.png");
		tempDisabledDASpr->setColor(Color3B(95, 95, 95));
		auto tempDA = MenuItemImage::create();
		tempDA->initWithNormalSprite(
			tempNormalDASpr,
			tempClickedDASpr,
			tempDisabledDASpr,
			CC_CALLBACK_0(
				waitingRoomJoinLayer::ipAddrDigitDown,
				this,
				i
			)
		);
		tempDA->setRotationX(180.0f);
		tempDA->setScale(0.5f);
		tempDA->setAnchorPoint(Vec2::ZERO);
		tempDA->setPosition(Vec2(40 + 50 * i + addWeightValue, 600));
		btnMenu->addChild(tempDA);
	}
	for (int i = 0; i < 3; i++)
	{
		//dot spr
		auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName("sprDot.png");
		tempSpr->setScale(0.8f);
		tempSpr->setAnchorPoint(Vec2::ZERO);
		tempSpr->setPosition(Vec2(199 + 160 * i, 572));
		this->addChild(tempSpr, gameMetaData::layerZOrder::objZ0);
	}

	//join btn
	auto tempNormalJoinSpr = Sprite::createWithSpriteFrameName("btnJoinNormal.png");
	auto tempClickedJoinSpr = Sprite::createWithSpriteFrameName("btnJoinNormal.png");
	tempClickedJoinSpr->setColor(Color3B(125, 125, 125));
	auto tempDisabledJoinSpr = Sprite::createWithSpriteFrameName("btnJoinNormal.png");
	tempDisabledJoinSpr->setColor(Color3B(95, 95, 95));
	auto btnJoin = MenuItemImage::create();
	btnJoin->initWithNormalSprite(
		tempNormalJoinSpr,
		tempClickedJoinSpr,
		tempDisabledJoinSpr,
		CC_CALLBACK_0(
			waitingRoomJoinLayer::join2IPAddr,
			this
		)
	);
	btnJoin->setScale(0.8f);
	btnJoin->setPosition(Vec2(384, 150));
	btnMenu->addChild(btnJoin);

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
	btnCopyFromClipboard->setScale(0.7f);
	btnCopyFromClipboard->setAnchorPoint(Vec2::ZERO);
	btnCopyFromClipboard->setPosition(Vec2(100, 450));
	
	btnMenu->addChild(btnCopyFromClipboard);
}

void waitingRoomJoinLayer::ipAddrDigitUp(const int sprIndex)
{
	//ipAddrSprFormat[index] value + 1
	//if curValue is 9, change value to 0
	char tempBuf[2] = { '\0','\0' };

	//not match ipAddrSprFormat idx and ipAddrSpr idx
	int strIdx = sprIndex + (sprIndex / 3);
	tempBuf[0] = ipAddrSprFormat.at(strIdx);
	int tempVal = atoi(tempBuf);

	//012 3 456 7 8910 11 121314
	//limit 000.000.000.000 255.255.255.255
	if (sprIndex == 0 || sprIndex == 3 || sprIndex == 6 || sprIndex == 9)
	{
		tempVal = (tempVal + 1) > 2 ? 0 : tempVal + 1;
		if (tempVal == 2)
		{
			if (ipAddrSprFormat.at(strIdx + 1) > '5')
			{
				ipAddrSprFormat.at(strIdx + 1) = '5';
				ipAddrSpr[sprIndex + 1]->setTextureRect(sprManager->getNumSprRect(5));
			}

			if ((ipAddrSprFormat.at(strIdx + 1) == '5') && (ipAddrSprFormat.at(strIdx + 2) > '5'))
			{
				ipAddrSprFormat.at(strIdx + 2) = '5';
				ipAddrSpr[sprIndex + 2]->setTextureRect(sprManager->getNumSprRect(5));
			}
		}
	}
	else
	{
		int diff = strIdx % 4;
		int tempHeader = strIdx - diff;
		if (diff == 1)
		{
			if (ipAddrSprFormat.at(tempHeader) == '2')
			{
				tempVal = (tempVal + 1) > 5 ? 0 : tempVal + 1;
				if (tempVal == 5)
				{
					if (ipAddrSprFormat.at(tempHeader + 2) > '5')
					{
						ipAddrSprFormat.at(tempHeader + 2) = '5';
						ipAddrSpr[sprIndex + 1]->setTextureRect(sprManager->getNumSprRect(5));
					}
				}
			}
			else
			{
				tempVal = (tempVal + 1) > 9 ? 0 : tempVal + 1;
			}
		}
		else if (diff == 2)
		{
			if ((ipAddrSprFormat.at(tempHeader) == '2') && (ipAddrSprFormat.at(tempHeader+1) == '5'))
			{
				tempVal = (tempVal + 1) > 5 ? 0 : tempVal + 1;
			}
			else
			{
				tempVal = (tempVal + 1) > 9 ? 0 : tempVal + 1;
			}
		}
	}

	_itoa_s(tempVal, tempBuf, 10);
	ipAddrSprFormat.at(strIdx) = tempBuf[0];

	//call getNumRect(ipAddrSprFormat[index])
	ipAddrSpr[sprIndex]->setTextureRect(sprManager->getNumSprRect(tempVal));
}

void waitingRoomJoinLayer::ipAddrDigitDown(const int sprIndex)
{
	//ipAddrSprFormat[index] value - 1
	//if curValue is 0, change value to 9
	char tempBuf[2] = { '\0','\0' };

	//not match ipAddrSprFormat idx and ipAddrSpr idx
	int strIdx = sprIndex + (sprIndex / 3);
	tempBuf[0] = ipAddrSprFormat.at(strIdx);
	int tempVal = atoi(tempBuf);

	//limit 000.000.000.000 255.255.255.255
	if (sprIndex == 0 || sprIndex == 3 || sprIndex == 6 || sprIndex == 9)
	{
		tempVal = (tempVal - 1) < 0 ? 2 : tempVal - 1;
		tempVal = tempVal > 2 ? 2 : tempVal;
		if (tempVal == 2)
		{
			if (ipAddrSprFormat.at(strIdx + 1) > '5')
			{
				ipAddrSprFormat.at(strIdx + 1) = '5';
				ipAddrSpr[sprIndex + 1]->setTextureRect(sprManager->getNumSprRect(5));
			}

			if ((ipAddrSprFormat.at(strIdx + 1) == '5') && (ipAddrSprFormat.at(strIdx + 2) > '5'))
			{
				ipAddrSprFormat.at(strIdx + 2) = '5';
				ipAddrSpr[sprIndex + 2]->setTextureRect(sprManager->getNumSprRect(5));
			}
		}
	}
	else
	{
		int diff = strIdx % 4;
		int tempHeader = strIdx - diff;
		if (diff == 1)
		{
			if (ipAddrSprFormat.at(tempHeader) == '2')
			{
				tempVal = (tempVal - 1) < 0 ? 5 : tempVal - 1;
				if (tempVal == 5)
				{
					if (ipAddrSprFormat.at(tempHeader + 2) > '5')
					{
						ipAddrSprFormat.at(tempHeader + 2) = '5';
						ipAddrSpr[sprIndex + 1]->setTextureRect(sprManager->getNumSprRect(5));
					}
				}
			}
			else
			{
				tempVal = (tempVal - 1) < 0 ? 9 : tempVal - 1;
			}
		}
		else if (diff == 2)
		{
			if ((ipAddrSprFormat.at(tempHeader) == '2') && (ipAddrSprFormat.at(tempHeader + 1) == '5'))
			{
				tempVal = (tempVal - 1) < 0 ? 5 : tempVal - 1;
			}
			else
			{
				tempVal = (tempVal - 1) < 0 ? 9 : tempVal - 1;
			}
		}
	}

	_itoa_s(tempVal, tempBuf, 10);
	ipAddrSprFormat.at(strIdx) = tempBuf[0];

	//call getNumRect(ipAddrSprFormat[index])
	ipAddrSpr[sprIndex]->setTextureRect(sprManager->getNumSprRect(tempVal));
}

void waitingRoomJoinLayer::displayIpAddrFromClip()
{
	//copy from clipboard
	//don't forget close after open
	if (::OpenClipboard(NULL))
	{
		unsigned int priorityList = CF_TEXT;
		//check exist string in clipboard
		if (::GetPriorityClipboardFormat(&priorityList, 1) == CF_TEXT)
		{
			//get handle of memory recorded string from clipboard
			HANDLE hClipboardData = ::GetClipboardData(CF_TEXT);

			//get memory address. use GlobalLock
			char *pClipboardData = (char*)::GlobalLock(hClipboardData);
			std::string strClipData = pClipboardData;

			//match string ip format
			bool chkFormat = true;
			std::string strFormat = "000.000.000.000";
			for (int i = strClipData.size() - 1, bi = strFormat.size() - 1, tempCnt = 0;
				i >= 0;
				i--, bi--)
			{
				char bufData = strClipData.at(i);
				if(bufData != '.')
				{
					if (bufData >= '0' && bufData <= '9')
					{
						chkFormat = true;
						strFormat.at(bi) = bufData;
					}
					else
					{
						chkFormat = false;
						break;
					}
				}
				else
				{
					tempCnt++;
					bi = strFormat.size() - (tempCnt * 4);
					if (bi < 0)
					{
						chkFormat = false;
						break;
					}
				}
			}

			if (chkFormat)
			{
				ipAddrSprFormat = strFormat;
				//update ipAddrSprFormat display
				updateIpAddrDisplay();
			}
			else
			{
				//popup
				//announce required format - 012.255.234.198
				callWrongIpPopup();
			}
			//unlock memory
			::GlobalUnlock(hClipboardData);
		}
		::CloseClipboard();
	}
}

void waitingRoomJoinLayer::updateIpAddrDisplay()
{
	for (int i = 0; i < ipAddrSprFormat.size(); i++)
	{
		if (i % 4 == 3)
		{
			continue;
		}
		char tempBuf[2] = { '\0','\0' };
		tempBuf[0] = ipAddrSprFormat[i];
		int tempNum = atoi(tempBuf);
		int sprIndex = i - (i / 4);
		ipAddrSpr[sprIndex]->setTextureRect(sprManager->getNumSprRect(tempNum));
	}
}

void waitingRoomJoinLayer::convertIpAddrFormat()
{
	int readCharCnt = 0;
	int readPartCnt = 1;
	std::string tempBuffer = "";

	ipAddrNetFormat = "";

	for (auto i : ipAddrSprFormat)
	{
		readCharCnt++;

		if (i == '.')
		{
			readCharCnt = 0;
			tempBuffer += i;
			ipAddrNetFormat += tempBuffer;
			tempBuffer = "";
			continue;
		}

		if (i >= '0' && i <= '9')
		{
			if (tempBuffer.size() == 1 && tempBuffer.at(0) == '0')
			{
				tempBuffer = i;
			}
			else
			{
				tempBuffer += i;
			}
		}
	}
	ipAddrNetFormat += tempBuffer;

}

bool waitingRoomJoinLayer::checkIpAddrSuitability()
{
	for (int i = 0; i < 14; i += 4)
	{
		if (ipAddrSprFormat.at(i) > '2')
		{
			return false;
		}
		else if (ipAddrSprFormat.at(i) == '2')
		{
			if (ipAddrSprFormat.at(i + 1) > '5')
			{
				return false;
			}
			else if (ipAddrSprFormat.at(i+1) == '5')
			{
				if (ipAddrSprFormat.at(i + 2) > '5')
				{
					return false;
				}
			}
		}
	}

	std::string tempBuffer = "";
	tempBuffer.assign(ipAddrNetFormat, 0, 3);

	if (tempBuffer.at(0) == '0')
	{
		return false;
	}
	else if (tempBuffer == "127")
	{
		return false;
	}
	else if (tempBuffer >= "255")
	{
		return false;
	}

	return true;
}

void waitingRoomJoinLayer::join2IPAddr()
{  
	//connect host
	convertIpAddrFormat();
	if (checkIpAddrSuitability() == false)
	{
		//add error popup
		return;
	}

	networkManager::getInstance()->runClient(ipAddrNetFormat);

	//check connection state until timeout
	this->updateNetworkInfo();
}

void waitingRoomJoinLayer::updateNetworkInfo()
{
	//call update scheduling
	this->scheduleUpdate();
}

void waitingRoomJoinLayer::initTimeout(float time)
{
	this->_timeout = time;
}

void waitingRoomJoinLayer::callWrongIpPopup()
{
	EventCustom popupEvent("popupWarning");
	auto warningFlag = gameMetaData::warningCode::wrongIpFormat;
	popupEvent.setUserData(&warningFlag);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&popupEvent);
}

void waitingRoomJoinLayer::callbackJoinSuccess()
{
	this->unscheduleUpdate();

	this->setVisible(false);

	//call waitingRoomUiLayer wakeup
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("guestModeUILayerWakeup");
}
