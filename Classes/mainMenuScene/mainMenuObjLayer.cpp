#include "mainMenuScene\mainMenuObjLayer.h"
#include "managers\gameFlowManager.h"

using namespace cocos2d;

bool mainMenuObjLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	auto btnHost = MenuItemImage::create(
		"UISprite/btnHostNormal.png"
		, "UISprite/btnHostPress.png"
		, "UISprite/btnHostPress.png"
		, CC_CALLBACK_0(mainMenuObjLayer::hostGameBtnCallback,this)
	);
	auto btnJoin = MenuItemImage::create(
		"UISprite/btnJoinNormal.png"
		, "UISprite/btnJoinPress.png"
		, "UISprite/btnJoinPress.png"
		, CC_CALLBACK_0(mainMenuObjLayer::joinGameBtnCallback, this)
	);
	auto btnExit = MenuItemImage::create(
		"UISprite/btnExitNormal.png"
		, "UISprite/btnExitPress.png"
		, "UISprite/btnExitPress.png"
		, CC_CALLBACK_0(mainMenuObjLayer::exitBtnCallback, this)
	);
	auto btnSingle = MenuItemImage::create(
		"UISprite/btnSingleNormal.png"
		, "UISprite/btnSinglePress.png"
		, "UISprite/btnSinglePress.png"
		, CC_CALLBACK_0(mainMenuObjLayer::singleGameBtnCallback, this)
	);

#if (CURRENT_BUILD_MODE == SINGLE_MODE_BUILD)
	auto mainMenu = Menu::create(btnSingle, btnExit,  nullptr);
	mainMenu->alignItemsVerticallyWithPadding(4.0f);
	mainMenu->setPosition(Vec2(384, 300));
#elif (CURRENT_BUILD_MODE == NETWORK_MODE_BUILD)
	auto mainMenu = Menu::create(btnHost, btnJoin, btnExit, nullptr);
	mainMenu->alignItemsVerticallyWithPadding(2.0f);
	mainMenu->setPosition(Vec2(384, 280));
#elif (CURRENT_BUILD_MODE == RELEASE_MODE_BUILD)
	auto mainMenu = Menu::create(btnHost, btnJoin, btnExit, btnSingle, nullptr);
	mainMenu->alignItemsVerticallyWithPadding(2.0f);
	mainMenu->setPosition(Vec2(384, 190));
	btnSingle->setPosition(Vec2(0, 450));
#endif

	this->addChild(mainMenu);

	return true;
}

void mainMenuObjLayer::exitBtnCallback()
{
	gameFlowManager::getInstance()->endGame();
}

void mainMenuObjLayer::hostGameBtnCallback()
{
}

void mainMenuObjLayer::joinGameBtnCallback()
{
}

void mainMenuObjLayer::singleGameBtnCallback()
{
	gameFlowManager::getInstance()->changeScene2SingleMode();
}
