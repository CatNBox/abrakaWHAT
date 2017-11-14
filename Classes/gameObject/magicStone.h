#pragma once
#include "cocos2d.h"
#include "gameMetaData.h"

class magicStone : public cocos2d::Object
{
public:
	void init();
	int getMagic();
	int getState();
	void setState(gameMetaData::stoneState newState);
	virtual ~magicStone();

	virtual magicStone* clone() = 0; //for prototype pattern

protected:
	int magic = gameMetaData::magicStones::base;
	int state = gameMetaData::stoneState::notUse;
};

class msYongyong : public magicStone
{
public:
	msYongyong()
	{
		cocos2d::log("@@@ create YONGYONG CLASS @@@");
		magic = gameMetaData::magicStones::yongyong;
		state = gameMetaData::stoneState::notUse;
	};
	msYongyong(const msYongyong& clon)
	{
		cocos2d::log("@@@ cloning YONGYONG CLASS @@@");
		magic = gameMetaData::magicStones::yongyong;
		state = gameMetaData::stoneState::notUse;
	};
	~msYongyong();
};
class msBangrang : public magicStone
{
public:
	msBangrang()
	{
		cocos2d::log("@@@ create BANGRANG CLASS @@@");
		magic = gameMetaData::magicStones::bangrang;
		state = gameMetaData::stoneState::notUse;
	};
	~msBangrang();
};
class msWind : public magicStone
{
public:
	msWind()
	{
		cocos2d::log("@@@ create WIND CLASS @@@");
		magic = gameMetaData::magicStones::wind;
		state = gameMetaData::stoneState::notUse;
	};
	~msWind();
};
class msBooung : public magicStone
{
public:
	msBooung()
	{
		cocos2d::log("@@@ create BOOUNG CLASS @@@");
		magic = gameMetaData::magicStones::booung;
		state = gameMetaData::stoneState::notUse;
	};
	~msBooung();
};
class msBunpok : public magicStone
{
public:
	msBunpok()
	{
		cocos2d::log("@@@ create BUNPOK CLASS @@@");
		magic = gameMetaData::magicStones::bunpok;
		state = gameMetaData::stoneState::notUse;
	};
	~msBunpok();
};
class msNungang : public magicStone
{
public:
	msNungang()
	{
		cocos2d::log("@@@ create NUNGANG CLASS @@@");
		magic = gameMetaData::magicStones::nungang;
		state = gameMetaData::stoneState::notUse;
	};
	~msNungang();
};
class msBuljak : public magicStone
{
public:
	msBuljak()
	{
		cocos2d::log("@@@ create BULJAK CLASS @@@");
		magic = gameMetaData::magicStones::buljak;
		state = gameMetaData::stoneState::notUse;
	};
	~msBuljak();
};
class msPostion : public magicStone
{
public:
	msPostion()
	{
		cocos2d::log("@@@ create POSTION CLASS @@@");
		magic = gameMetaData::magicStones::postion;
		state = gameMetaData::stoneState::notUse;
	};
	~msPostion();
};