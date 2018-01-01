#include "magicStone.h"
#include "managers\gameFlowManager.h"
//#include <iostream>

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
	curX = STDAXIS;
	curY = STDAXIS;
	tX = STDAXIS;
	tY = STDAXIS;
	this->initMsSprite();
	this->stopAllActions();
	this->setOpacity(255);
	this->setScale(1);
	this->setVisible(false);
	this->setRotation(0);
	this->setLocalZOrder(gameMetaData::layerZOrder::objZ2);
	this->setPosition(STDAXIS, STDAXIS);
}

bool magicStone::initMsSprite()
{
	return this->initWithSpriteFrameName(gameMetaData::arrMsSpriteName[magic]);
}

bool magicStone::setBaseSprite()
{
	return this->initWithSpriteFrameName(gameMetaData::arrMsSpriteName[gameMetaData::msType::base]);
}

void magicStone::notifyActionStart()
{
	gameFlowManager::getInstance()->incRunningActionCnt();
}

void magicStone::notifyActionEnd()
{
	gameFlowManager::getInstance()->decRunningActionCnt();
}

void magicStone::actionMove(const float priorDelay, const cocos2d::Vec2 targetPos, const int movementEnum)
{
	this->stopAllActions();
	auto preDelay = cocos2d::DelayTime::create(priorDelay);
	auto showThis = cocos2d::Show::create();
	auto waitDelay = cocos2d::DelayTime::create(0.5f);
	auto moving = cocos2d::MoveTo::create(0.8f, targetPos);
	auto callPlaySFX = cocos2d::CallFunc::create([=]() 
	{
		gameFlowManager::getInstance()->getSoundManager()->playSfx(gameMetaData::sfxName::msMoving00);
	});

	cocos2d::Sequence* seq;
	if(movementEnum != gameMetaData::msMovement::reordering)
		seq = gameFlowManager::getInstance()->wrapActions(preDelay, showThis, callPlaySFX, moving, NULL);
	else
		seq = gameFlowManager::getInstance()->wrapActions(preDelay, showThis, moving, NULL);

	this->runAction(seq);
}

void magicStone::actionActivated()
{
	this->stopAllActions();
	this->initMsSprite();
	this->setStatus(gameMetaData::msStatus::discard);
	auto moving = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(STDAXIS, STDAXIS));
	auto scaling = cocos2d::ScaleTo::create(1.0f, 5.0f);
	auto fadeOut = cocos2d::FadeOut::create(1.0f);
	auto spawning = cocos2d::Spawn::create(scaling, fadeOut, NULL);
	auto callPlaySFX = cocos2d::CallFunc::create([=]()
	{
		gameFlowManager::getInstance()->getSoundManager()->playSfx(gameMetaData::sfxName::activateMagic00);
	});

	auto seq = gameFlowManager::getInstance()->wrapActions(moving, callPlaySFX, spawning, NULL);

	this->runAction(seq);
}

void magicStone::actionRevealedSecret()
{
	//----like flip magicStone
	auto zOrderUp = cocos2d::CallFunc::create([=]() {this->setLocalZOrder(gameMetaData::layerZOrder::objZ2); });
	auto shrinking = cocos2d::ScaleTo::create(0.3f, 0.01f, 1.5f);
	auto changeTexture = cocos2d::CallFunc::create([=]() {this->initMsSprite(); });
	auto growing = cocos2d::ScaleTo::create(0.3f, SECRETSTONE_BASESCALE, SECRETSTONE_BASESCALE);
	auto zOrderDown = cocos2d::CallFunc::create([=]() {this->setLocalZOrder(gameMetaData::layerZOrder::objZ0); });
	
	auto seq = gameFlowManager::getInstance()->wrapActions(zOrderUp, shrinking, changeTexture, growing, zOrderDown, NULL);

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
