#include "mainMenuScene\mainMenuObjLayer.h"
#include "gameObject\gameMetaData.h"
#include "gameRoomScene\gameRoomScene.h"


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
	auto btnURL = MenuItemImage::create(
		"UISprite/btnGetRealNormal.png",
		"UISprite/btnGetRealPress.png",
		"UISprite/btnGetRealPress.png",
		CC_CALLBACK_0(mainMenuObjLayer::openBuyURL, this)
	);

	int mainMenuX = 384;
	int mainMenuY = 300;

	//buildMode set in gameMetaData.h
#if (CURRENT_BUILD_MODE == SINGLE_MODE_BUILD)
	auto mainMenu = Menu::create(btnSingle, btnExit,  nullptr);
	mainMenu->alignItemsVerticallyWithPadding(4.0f);
	mainMenu->setPosition(Vec2(mainMenuX, mainMenuY));
#elif (CURRENT_BUILD_MODE == NETWORK_MODE_BUILD)
	auto mainMenu = Menu::create(btnHost, btnJoin, btnExit, nullptr);
	mainMenu->alignItemsVerticallyWithPadding(2.0f);
	mainMenuY = 280;
	mainMenu->setPosition(Vec2(mainMenuX, mainMenuY));
#elif (CURRENT_BUILD_MODE == RELEASE_MODE_BUILD)
	auto mainMenu = Menu::create(btnHost, btnJoin, btnExit, btnSingle, nullptr);
	mainMenu->alignItemsVerticallyWithPadding(2.0f);
	mainMenuY = 190;
	mainMenu->setPosition(Vec2(mainMenuX, mainMenuY));
	btnSingle->setPosition(Vec2(0, 450));
#endif

	mainMenu->addChild(btnURL);
	btnURL->setScale(0.7f);
	btnURL->setPosition(Vec2(620 - mainMenuX, 80 - mainMenuY));

	this->addChild(mainMenu);

	return true;
}

void mainMenuObjLayer::exitBtnCallback()
{
	Director::getInstance()->end();
}

void mainMenuObjLayer::hostGameBtnCallback()
{
}

void mainMenuObjLayer::joinGameBtnCallback()
{
}

void mainMenuObjLayer::singleGameBtnCallback()
{
	auto singleModeScene = TransitionSlideInT::create(0.6f, gameRoomScene::createScene());
	Director::getInstance()->replaceScene(singleModeScene);
}

void mainMenuObjLayer::openBuyURL()
{
	cocos2d::Application::getInstance()->openURL("http://www.koreaboardgames.com/boardgame/game_view.php?prd_idx=15937");
	cocos2d::Application::getInstance()->openURL("http://www.divedice.com/site/game/gds_detail01.php?mode=view&prd_idx=15937");
}
