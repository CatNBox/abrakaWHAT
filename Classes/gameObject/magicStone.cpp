#include "magicStone.h"
#include "managers\actionManager.h"
#include "managers\soundManager.h"
#include "gameMetaData.h"

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

void magicStone::setState(const gameMetaData::msState newStatus)
{
	status = newStatus;
}

void magicStone::initObjData()
{
	status = gameMetaData::msState::notUse;
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

void magicStone::actionMove(const float priorDelay, const cocos2d::Vec2 targetPos, const gameMetaData::msMovement movementEnum)
{
	this->stopAllActions();
	auto preDelay = cocos2d::DelayTime::create(priorDelay);
	auto showThis = cocos2d::Show::create();
	auto waitDelay = cocos2d::DelayTime::create(0.5f);
	auto moving = cocos2d::MoveTo::create(0.8f, targetPos);
	auto callPlaySFX = cocos2d::CallFunc::create([=]() 
	{
		sndManager->playSfx(gameMetaData::sfxName::msMoving00);
	});

	cocos2d::Sequence* seq;
	if(movementEnum != gameMetaData::msMovement::reordering)
		seq = actManager->wrapActions4Cnt(preDelay, showThis, callPlaySFX, moving, NULL);
	else
		seq = actManager->wrapActions4Cnt(preDelay, showThis, moving, NULL);

	this->runAction(seq);
}

void magicStone::actionActivated()
{
	this->stopAllActions();
	this->initMsSprite();
	this->setState(gameMetaData::msState::discard);
	auto moving = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(STDAXIS, STDAXIS));
	auto scaling = cocos2d::ScaleTo::create(1.0f, 5.0f);
	auto fadeOut = cocos2d::FadeOut::create(1.0f);
	auto spawning = cocos2d::Spawn::create(scaling, fadeOut, NULL);
	auto callPlaySFX = cocos2d::CallFunc::create([=]()
	{
		sndManager->playSfx(gameMetaData::sfxName::activateMagic00);
	});

	auto seq = actManager->wrapActions4Cnt(moving, callPlaySFX, spawning, NULL);

	this->runAction(seq);
}

void magicStone::actionRevealedSecret()
{
	//----looks like flip magicStone
	auto zOrderUp = cocos2d::CallFunc::create([=]() {this->setLocalZOrder(gameMetaData::layerZOrder::objZ2); });
	auto shrinking = cocos2d::ScaleTo::create(0.3f, 0.01f, 1.5f);
	auto changeTexture = cocos2d::CallFunc::create([=]() {this->initMsSprite(); });
	auto growing = cocos2d::ScaleTo::create(0.3f, SECRETSTONE_BASESCALE, SECRETSTONE_BASESCALE);
	auto zOrderDown = cocos2d::CallFunc::create([=]() {this->setLocalZOrder(gameMetaData::layerZOrder::objZ0); });
	
	auto seq = actManager->wrapActions4Cnt(zOrderUp, shrinking, changeTexture, growing, zOrderDown, NULL);

	this->runAction(seq);
}

magicStone::magicStone()
{
	actManager = actionManager::getInstance();
	sndManager = new soundManager;

	magic = gameMetaData::msType::base;
	initObjData();
}

magicStone::magicStone(const gameMetaData::msType msType)
{
	actManager = actionManager::getInstance();
	sndManager = new soundManager;

	magic = msType;
	initObjData();
}

magicStone::~magicStone()
{
	delete sndManager;
}

magicStone * magicStone::clone()
{
	return new magicStone(magic);
}
