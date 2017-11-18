#include "magicStone.h"

/*----------------------------------------


magicStone


-----------------------------------------*/
int magicStone::getMagic()
{
	return this->magic;
}

int magicStone::getState()
{
	return this->state;
}

void magicStone::setState(gameMetaData::stoneState newState)
{
	state = newState;
}

void magicStone::init()
{
	state = gameMetaData::stoneState::notUse;
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
	init();
}

msYongyong::msYongyong(const msYongyong& clon)
{
	cocos2d::log("@@@ cloning YONGYONG CLASS @@@");
	init();
}

magicStone * msYongyong::clone()
{
	return  new msYongyong(*this);
}

msYongyong::~msYongyong()
{
}

void msYongyong::init()
{
	magic = gameMetaData::magicStones::yongyong;
	//msTexture = cocos2d::Sprite::createWithSpriteFrameName("ms1_yongyong.png");
	cocos2d::log("calling msYongyonh init()");
}

/*----------------------------------------


msBangrang


-----------------------------------------*/
msBangrang::msBangrang()
{
	cocos2d::log("@@@ create BANGRANG CLASS @@@");
	init();
}

msBangrang::msBangrang(const msBangrang & clon)
{
	cocos2d::log("@@@ cloning BANGRANG CLASS @@@");
	init();
}

magicStone * msBangrang::clone()
{
	return  new msBangrang(*this);
}

void msBangrang::init()
{
	magic = gameMetaData::magicStones::bangrang;
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
	init();
}

msWind::msWind(const msWind & clon)
{
	cocos2d::log("@@@ cloning WIND CLASS @@@");
	init();
}

magicStone * msWind::clone()
{
	return  new msWind(*this);
}

void msWind::init()
{
	magic = gameMetaData::magicStones::wind;
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
	init();
}

msBooung::msBooung(const msBooung & clon)
{
	cocos2d::log("@@@ cloning BOOUNG CLASS @@@");
	init();
}

magicStone * msBooung::clone()
{
	return  new msBooung(*this);
}

void msBooung::init()
{
	magic = gameMetaData::magicStones::booung;
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
	init();
}

msBunpok::msBunpok(const msBunpok & clon)
{
	cocos2d::log("@@@ cloning BUNPOK CLASS @@@");
	init();
}

msBunpok::~msBunpok()
{
}

magicStone * msBunpok::clone()
{
	return  new msBunpok(*this);
}

void msBunpok::init()
{
	magic = gameMetaData::magicStones::bunpok;
}

/*----------------------------------------


msNungang


-----------------------------------------*/
msNungang::msNungang()
{
	cocos2d::log("@@@ create NUNGANG CLASS @@@");
	init();
}

msNungang::msNungang(const msNungang & clon)
{
	cocos2d::log("@@@ cloning NUNGANG CLASS @@@");
	init();
}

msNungang::~msNungang()
{
}

magicStone * msNungang::clone()
{
	return  new msNungang(*this);
}

void msNungang::init()
{
	magic = gameMetaData::magicStones::nungang;
}

/*----------------------------------------


msBuljak


-----------------------------------------*/
msBuljak::msBuljak()
{
	cocos2d::log("@@@ create BULJAK CLASS @@@");
	init();
}

msBuljak::msBuljak(const msBuljak & clon)
{
	cocos2d::log("@@@ cloning BULJAK CLASS @@@");
	init();
}

msBuljak::~msBuljak()
{
}

magicStone * msBuljak::clone()
{
	return  new msBuljak(*this);
}

void msBuljak::init()
{
	magic = gameMetaData::magicStones::buljak;
}

/*----------------------------------------


msPostion


-----------------------------------------*/
msPostion::msPostion()
{
	cocos2d::log("@@@ create POSTION CLASS @@@");
	init();
}

msPostion::msPostion(const msPostion & clon)
{
	cocos2d::log("@@@ cloning POSTION CLASS @@@");
	init();
}

msPostion::~msPostion()
{
}

magicStone * msPostion::clone()
{
	return  new msPostion(*this);
}

void msPostion::init()
{
	magic = gameMetaData::magicStones::postion;
}
