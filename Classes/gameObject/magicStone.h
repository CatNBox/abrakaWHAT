#pragma once
#include "cocos2d.h"
#include "gameMetaData.h"

/*----------------------------------------


magicStone


-----------------------------------------*/
class magicStone : public cocos2d::Sprite
{
public:
	int getMagic();
	int getStatus();
	void setStatus(const gameMetaData::msStatus newStatus);
	void initObjData();
	bool initMsTexture();
	bool setBaseSprite();

	virtual ~magicStone();
	virtual magicStone* clone() = 0; //for prototype pattern

protected:
	int magic = gameMetaData::msType::base;
	int status = gameMetaData::msStatus::notUse;
	int actionState;

	float curX = stdAxis;
	float curY = stdAxis;

	float tX = stdAxis;
	float tY = stdAxis;
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
	void initMagic();
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
	void initMagic() ;
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
	void initMagic() ;
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
	void initMagic() ;
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
	void initMagic() ;
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
	void initMagic() ;
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
	void initMagic() ;
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
	void initMagic() ;
};