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

protected:
	int magic = gameMetaData::magicStones::base;
	int state = gameMetaData::stoneState::notUse;
};

class ms_yongyong : public magicStone
{
public:
	ms_yongyong()
	{
		cocos2d::log("@@@ create YONGYONG CLASS @@@");
		magic = gameMetaData::magicStones::yongyong;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_yongyong();
};
class ms_bangrang : public magicStone
{
public:
	ms_bangrang()
	{
		cocos2d::log("@@@ create BANGRANG CLASS @@@");
		magic = gameMetaData::magicStones::bangrang;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_bangrang();
};
class ms_wind : public magicStone
{
public:
	ms_wind()
	{
		cocos2d::log("@@@ create WIND CLASS @@@");
		magic = gameMetaData::magicStones::wind;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_wind();
};
class ms_booung : public magicStone
{
public:
	ms_booung()
	{
		cocos2d::log("@@@ create BOOUNG CLASS @@@");
		magic = gameMetaData::magicStones::booung;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_booung();
};
class ms_bunpok : public magicStone
{
public:
	ms_bunpok()
	{
		cocos2d::log("@@@ create BUNPOK CLASS @@@");
		magic = gameMetaData::magicStones::bunpok;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_bunpok();
};
class ms_nungang : public magicStone
{
public:
	ms_nungang()
	{
		cocos2d::log("@@@ create NUNGANG CLASS @@@");
		magic = gameMetaData::magicStones::nungang;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_nungang();
};
class ms_buljak : public magicStone
{
public:
	ms_buljak()
	{
		cocos2d::log("@@@ create BULJAK CLASS @@@");
		magic = gameMetaData::magicStones::buljak;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_buljak();
};
class ms_postion : public magicStone
{
public:
	ms_postion()
	{
		cocos2d::log("@@@ create POSTION CLASS @@@");
		magic = gameMetaData::magicStones::postion;
		state = gameMetaData::stoneState::notUse;
	};
	~ms_postion();
};