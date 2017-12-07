#include "magicStone.h"
#include "SimpleAudioEngine.h"

/*----------------------------------------


magicStone


-----------------------------------------*/
int magicStone::getMagic()
{
	return this->magic;
}

int magicStone::getStatus()
{
	return this->status;
}

void magicStone::setStatus(const gameMetaData::msStatus newStatus)
{
	status = newStatus;
}

void magicStone::initObjData()
{
	status = gameMetaData::msStatus::notUse;
	curX = stdAxis;
	curY = stdAxis;
	tX = stdAxis;
	tY = stdAxis;
	actionActiveChecker = false;
	this->stopAllActions();
	this->setOpacity(255);
	this->setScale(1);
	this->setVisible(false);
	this->setRotation(0);
	this->setLocalZOrder(gameMetaData::layerZOrder::objZ2);
	this->setPosition(stdAxis, stdAxis);
}

bool magicStone::initMsSprite()
{
	return this->initWithSpriteFrameName(gameMetaData::arrMsSpriteName[magic]);
}

bool magicStone::setBaseSprite()
{
	return this->initWithSpriteFrameName(gameMetaData::arrMsSpriteName[gameMetaData::msType::base]);
}

bool magicStone::isActionRunning()
{
	return actionActiveChecker;
}

void magicStone::toggleLockAction()
{
	actionActiveChecker = !actionActiveChecker;
}

void magicStone::actionMove(const float priorDelay, const cocos2d::Vec2 targetPos)
{
	//if (isActionRunning())
	//	return;

	this->stopAllActions();
	toggleLockAction();	//Lock runAction
	auto preDelay = cocos2d::DelayTime::create(priorDelay);
	auto showThis = cocos2d::Show::create();
	auto waitDelay = cocos2d::DelayTime::create(0.5f);
	auto moving = cocos2d::MoveTo::create(0.8f, targetPos);
	auto callTogLock = cocos2d::CallFunc::create(CC_CALLBACK_0(magicStone::toggleLockAction, this));
	auto callPlaySFX = cocos2d::CallFunc::create([=]() 
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("soundResource/msMoving00.wav",false,0.3f,0.0f,0.7f); 
	});
	cocos2d::Vector<cocos2d::FiniteTimeAction*> actionList;
	actionList.pushBack(preDelay);
	actionList.pushBack(showThis);
	if(priorDelay != 0.1f)
		actionList.pushBack(callPlaySFX);
	actionList.pushBack(moving);
	actionList.pushBack(callTogLock);
	auto seq = cocos2d::Sequence::create(actionList);

	this->runAction(seq);
}

void magicStone::actionActivated()
{
	//if (isActionRunning())
	//	return;

	this->stopAllActions();
	toggleLockAction();	//Lock runAction
	initMsSprite();
	this->setStatus(gameMetaData::msStatus::discard);
	auto moving = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(stdAxis, stdAxis));
	auto scaling = cocos2d::ScaleTo::create(1.0f, 5.0f);
	auto fadeOut = cocos2d::FadeOut::create(1.0f);
	auto spawning = cocos2d::Spawn::create(scaling, fadeOut, NULL);
	auto callTogLock = cocos2d::CallFunc::create(CC_CALLBACK_0(magicStone::toggleLockAction, this));
	auto seq = cocos2d::Sequence::create(moving, spawning, callTogLock, NULL);

	this->runAction(seq);
}

void magicStone::actionRevealedSecret()
{
	//----like flip magicStone
	auto zOrderUp = cocos2d::CallFunc::create([=]() {this->setLocalZOrder(gameMetaData::layerZOrder::objZ2); });
	auto shrinking = cocos2d::ScaleTo::create(0.3f, 0.01f, 1.5f);
	auto changeTexture = cocos2d::CallFunc::create([=]() {this->initMsSprite(); });
	auto growing = cocos2d::ScaleTo::create(0.3f, secretScale, secretScale);
	auto zOrderDown = cocos2d::CallFunc::create([=]() {this->setLocalZOrder(gameMetaData::layerZOrder::objZ0); });
	auto seq = cocos2d::Sequence::create(zOrderUp, shrinking, changeTexture, growing, zOrderDown, NULL);
	this->runAction(seq);
}

magicStone::~magicStone()
{
}

/*----------------------------------------


msYongyong


-----------------------------------------*/
msYongyong::msYongyong()
{
	cocos2d::log("@@@ create YONGYONG CLASS @@@");
	initMagic();
}

msYongyong::msYongyong(const msYongyong& clon)
{
	cocos2d::log("@@@ cloning YONGYONG CLASS @@@");
	initMagic();
}

magicStone * msYongyong::clone()
{
	return  new msYongyong(*this);
}

msYongyong::~msYongyong()
{
}

void msYongyong::initMagic()
{
	magic = gameMetaData::msType::yongyong;
}

/*----------------------------------------


msBangrang


-----------------------------------------*/
msBangrang::msBangrang()
{
	cocos2d::log("@@@ create BANGRANG CLASS @@@");
	initMagic();
}

msBangrang::msBangrang(const msBangrang & clon)
{
	cocos2d::log("@@@ cloning BANGRANG CLASS @@@");
	initMagic();
}

magicStone * msBangrang::clone()
{
	return  new msBangrang(*this);
}

void msBangrang::initMagic()
{
	magic = gameMetaData::msType::bangrang;
}

msBangrang::~msBangrang()
{
}

/*----------------------------------------


msWind


-----------------------------------------*/
msWind::msWind()
{
	cocos2d::log("@@@ create WIND CLASS @@@");
	initMagic();
}

msWind::msWind(const msWind & clon)
{
	cocos2d::log("@@@ cloning WIND CLASS @@@");
	initMagic();
}

magicStone * msWind::clone()
{
	return  new msWind(*this);
}

void msWind::initMagic()
{
	magic = gameMetaData::msType::wind;
}

msWind::~msWind()
{
}

/*----------------------------------------


msBooung


-----------------------------------------*/
msBooung::msBooung()
{
	cocos2d::log("@@@ create BOOUNG CLASS @@@");
	initMagic();
}

msBooung::msBooung(const msBooung & clon)
{
	cocos2d::log("@@@ cloning BOOUNG CLASS @@@");
	initMagic();
}

magicStone * msBooung::clone()
{
	return  new msBooung(*this);
}

void msBooung::initMagic()
{
	magic = gameMetaData::msType::booung;
}

msBooung::~msBooung()
{
}

/*----------------------------------------


msBunpok


-----------------------------------------*/
msBunpok::msBunpok()
{
	cocos2d::log("@@@ create BUNPOK CLASS @@@");
	initMagic();
}

msBunpok::msBunpok(const msBunpok & clon)
{
	cocos2d::log("@@@ cloning BUNPOK CLASS @@@");
	initMagic();
}

msBunpok::~msBunpok()
{
}

magicStone * msBunpok::clone()
{
	return  new msBunpok(*this);
}

void msBunpok::initMagic()
{
	magic = gameMetaData::msType::bunpok;
}

/*----------------------------------------


msNungang


-----------------------------------------*/
msNungang::msNungang()
{
	cocos2d::log("@@@ create NUNGANG CLASS @@@");
	initMagic();
}

msNungang::msNungang(const msNungang & clon)
{
	cocos2d::log("@@@ cloning NUNGANG CLASS @@@");
	initMagic();
}

msNungang::~msNungang()
{
}

magicStone * msNungang::clone()
{
	return  new msNungang(*this);
}

void msNungang::initMagic()
{
	magic = gameMetaData::msType::nungang;
}

/*----------------------------------------


msBuljak


-----------------------------------------*/
msBuljak::msBuljak()
{
	cocos2d::log("@@@ create BULJAK CLASS @@@");
	initMagic();
}

msBuljak::msBuljak(const msBuljak & clon)
{
	cocos2d::log("@@@ cloning BULJAK CLASS @@@");
	initMagic();
}

msBuljak::~msBuljak()
{
}

magicStone * msBuljak::clone()
{
	return  new msBuljak(*this);
}

void msBuljak::initMagic()
{
	magic = gameMetaData::msType::buljak;
}

/*----------------------------------------


msPotion


-----------------------------------------*/
msPotion::msPotion()
{
	cocos2d::log("@@@ create potion CLASS @@@");
	initMagic();
}

msPotion::msPotion(const msPotion & clon)
{
	cocos2d::log("@@@ cloning potion CLASS @@@");
	initMagic();
}

msPotion::~msPotion()
{
}

magicStone * msPotion::clone()
{
	return  new msPotion(*this);
}

void msPotion::initMagic()
{
	magic = gameMetaData::msType::potion;
}
