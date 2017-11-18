#pragma once
#include "cocos2d.h"
#include "gameMetaData.h"

#define stdAxis 384.0f

/*----------------------------------------


magicStone


-----------------------------------------*/
class magicStone : public cocos2d::Object
{
public:
	int getMagic();
	int getState();
	void setState(gameMetaData::stoneState newState);
	void init();

	magicStone() {};

	virtual ~magicStone();
	virtual magicStone* clone() = 0; //for prototype pattern

protected:
	int magic = gameMetaData::magicStones::base;
	int state = gameMetaData::stoneState::notUse;

	float curX = stdAxis;
	float curY = stdAxis;

	float tX = stdAxis;
	float tY = stdAxis;

	cocos2d::Sprite* msTexture;
};

/*----------------------------------------


msYongyong


-----------------------------------------*/
class msYongyong : public magicStone
{
public:
	msYongyong();
	msYongyong(const msYongyong& clon);
	~msYongyong();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msBangrang


-----------------------------------------*/
class msBangrang : public magicStone
{
public:
	msBangrang();
	msBangrang(const msBangrang& clon);
	~msBangrang();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msWind


-----------------------------------------*/
class msWind : public magicStone
{
public:
	msWind();
	msWind(const msWind& clon);
	~msWind();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msBooung


-----------------------------------------*/
class msBooung : public magicStone
{
public:
	msBooung();
	msBooung(const msBooung& clon);
	~msBooung();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msBunpok


-----------------------------------------*/
class msBunpok : public magicStone
{
public:
	msBunpok();
	msBunpok(const msBunpok& clon);
	~msBunpok();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msNungang


-----------------------------------------*/
class msNungang : public magicStone
{
public:
	msNungang();
	msNungang(const msNungang& clon);
	~msNungang();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msBuljak


-----------------------------------------*/
class msBuljak : public magicStone
{
public:
	msBuljak();
	msBuljak(const msBuljak& clon);
	~msBuljak();

	magicStone* clone() override;

private:
	void init() ;
};

/*----------------------------------------


msPostion


-----------------------------------------*/
class msPostion : public magicStone
{
public:
	msPostion();
	msPostion(const msPostion& clon);
	~msPostion();

	magicStone* clone() override;

private:
	void init() ;
};