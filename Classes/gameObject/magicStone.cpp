#include "magicStone.h"

using namespace gameMetaData;

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

void magicStone::setState(const gameMetaData::msStatus newState)
{
	state = newState;
}

void magicStone::init()
{
	visible(false);
	state = gameMetaData::msStatus::notUse;
	curX = stdAxis;
	curY = stdAxis;
	tX = stdAxis;
	tY = stdAxis;
}

void magicStone::initTexture(const gameMetaData::msType msTypeEnum)
{
	auto temp = arrMsSprite[msTypeEnum];
	if (state == msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

magicStone::~magicStone()
{
}

void magicStone::visible(const bool textureVisible)
{
	msTexture->setVisible(textureVisible);
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

void msYongyong::initTexture()
{
	auto temp = arrMsSprite[msType::yongyong];
	if (state == msStatus::mine) 
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

msYongyong::~msYongyong()
{
}

void msYongyong::init()
{
	magic = gameMetaData::msType::yongyong;
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

void msBangrang::initTexture()
{
	std::string temp = arrMsSprite[msType::bangrang];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msBangrang::init()
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

void msWind::initTexture()
{
	std::string temp = arrMsSprite[msType::wind];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msWind::init()
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

void msBooung::initTexture()
{
	std::string temp = arrMsSprite[msType::booung];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msBooung::init()
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

void msBunpok::initTexture()
{
	std::string temp = arrMsSprite[msType::bunpok];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msBunpok::init()
{
	magic = gameMetaData::msType::bunpok;
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

void msNungang::initTexture()
{
	std::string temp = arrMsSprite[msType::nungang];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msNungang::init()
{
	magic = gameMetaData::msType::nungang;
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

void msBuljak::initTexture()
{
	std::string temp = arrMsSprite[msType::buljak];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msBuljak::init()
{
	magic = gameMetaData::msType::buljak;
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

void msPostion::initTexture()
{
	std::string temp = arrMsSprite[msType::postion];
	if (state == gameMetaData::msStatus::mine)
	{
		temp = arrMsSprite[msType::base];
	}

	auto tempSpr = cocos2d::Sprite::createWithSpriteFrameName(temp);
	msTexture->setTexture(tempSpr->getTexture());
}

void msPostion::init()
{
	magic = gameMetaData::msType::postion;
}
