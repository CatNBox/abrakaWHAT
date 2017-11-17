#pragma once
#include "cocos2d.h"
#include "gameMetaData.h"

class magicStone : public cocos2d::Object
{
public:
	int getMagic();
	int getState();
	void setState(gameMetaData::stoneState newState);
	virtual void init()
	{
		state = gameMetaData::stoneState::notUse;
	};
	magicStone()
	{
		init();
	};

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
	};
	msYongyong(const msYongyong& clon)
	{
		cocos2d::log("@@@ cloning YONGYONG CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msYongyong(*this);
	};
	~msYongyong();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::yongyong;
	}
};
class msBangrang : public magicStone
{
public:
	msBangrang()
	{
		cocos2d::log("@@@ create BANGRANG CLASS @@@");
	};
	msBangrang(const msBangrang& clon)
	{
		cocos2d::log("@@@ cloning BANGRANG CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msBangrang(*this);
	};
	~msBangrang();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::bangrang;
	}
};
class msWind : public magicStone
{
public:
	msWind()
	{
		cocos2d::log("@@@ create WIND CLASS @@@");
	};
	msWind(const msWind& clon)
	{
		cocos2d::log("@@@ cloning WIND CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msWind(*this);
	};
	~msWind();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::wind;
	}
};
class msBooung : public magicStone
{
public:
	msBooung()
	{
		cocos2d::log("@@@ create BOOUNG CLASS @@@");
	};
	msBooung(const msBooung& clon)
	{
		cocos2d::log("@@@ cloning BOOUNG CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msBooung(*this);
	};
	~msBooung();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::booung;
	}
};
class msBunpok : public magicStone
{
public:
	msBunpok()
	{
		cocos2d::log("@@@ create BUNPOK CLASS @@@");
	};
	msBunpok(const msBunpok& clon)
	{
		cocos2d::log("@@@ cloning BUNPOK CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msBunpok(*this);
	};
	~msBunpok();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::bunpok;
	}
};
class msNungang : public magicStone
{
public:
	msNungang()
	{
		cocos2d::log("@@@ create NUNGANG CLASS @@@");
	};
	msNungang(const msNungang& clon)
	{
		cocos2d::log("@@@ cloning NUNGANG CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msNungang(*this);
	};
	~msNungang();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::nungang;
	}
};
class msBuljak : public magicStone
{
public:
	msBuljak()
	{
		cocos2d::log("@@@ create BULJAK CLASS @@@");
	};
	msBuljak(const msBuljak& clon)
	{
		cocos2d::log("@@@ cloning BULJAK CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msBuljak(*this);
	};
	~msBuljak();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::buljak;
	}
};
class msPostion : public magicStone
{
public:
	msPostion()
	{
		cocos2d::log("@@@ create POSTION CLASS @@@");
	};
	msPostion(const msPostion& clon)
	{
		cocos2d::log("@@@ cloning POSTION CLASS @@@");
	};
	magicStone* clone() override
	{
		return  new msPostion(*this);
	};
	~msPostion();
private:
	void init() override
	{
		magic = gameMetaData::magicStones::postion;
	}
};