#include "magicStone.h"

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
	this->setVisible(false);
	this->setRotation(0);
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

void magicStone::actionMove(const float delay, const cocos2d::Vec2 targetPos)
{
	if (isActionRunning())
		return;

	toggleLockAction();	//Lock runAction
	auto preDelay = cocos2d::DelayTime::create(delay);
	auto showThis = cocos2d::Show::create();
	auto waitDelay = cocos2d::DelayTime::create(0.5f);
	auto moving = cocos2d::MoveTo::create(0.8f, targetPos);
	auto callTogLock = cocos2d::CallFunc::create(CC_CALLBACK_0(magicStone::toggleLockAction, this));
	auto seq = cocos2d::Sequence::create(preDelay, showThis, waitDelay, moving, callTogLock, NULL);

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


msPostion


-----------------------------------------*/
msPostion::msPostion()
{
	cocos2d::log("@@@ create POSTION CLASS @@@");
	initMagic();
}

msPostion::msPostion(const msPostion & clon)
{
	cocos2d::log("@@@ cloning POSTION CLASS @@@");
	initMagic();
}

msPostion::~msPostion()
{
}

magicStone * msPostion::clone()
{
	return  new msPostion(*this);
}

void msPostion::initMagic()
{
	magic = gameMetaData::msType::postion;
}
