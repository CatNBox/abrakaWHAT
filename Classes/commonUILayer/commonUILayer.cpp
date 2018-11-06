#include "commonUILayer.h"
#include "managers\spriteManager.h"
#include "gameObject\gameMetaData.h"

using namespace cocos2d;

bool commonUILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	sprManager = new spriteManager;
	commonUIMenu = Menu::create();
	commonUIMenu->setAnchorPoint(Vec2::ZERO);
	commonUIMenu->setPosition(Vec2::ZERO);
	this->addChild(commonUIMenu);

	setExitBtn2MainMenu();

	return true;
}

void commonUILayer::setExitBtn2MainMenu()
{
	auto btnExit = MenuItemImage::create(
		"UISprite/btnExitNormal.png",
		"UISprite/btnExitPress.png",
		"UISprite/btnExitPress.png",
		CC_CALLBACK_0(
			commonUILayer::returnMainmenuCallback,
			this
		)
	);
	btnExit->setScale(0.5f);
	btnExit->setPosition(Vec2(700, 730));

	commonUIMenu->addChild(btnExit);
}

void commonUILayer::returnMainmenuCallback()
{
	EventCustom popupEvent("popupWarning");
	auto warningFlag = gameMetaData::warningCode::exitBtnWarning;
	popupEvent.setUserData(&warningFlag);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&popupEvent);
}
