#include "magicStone.h"

void magicStone::init()
{
	state = gameMetaData::stoneState::notUse;
}

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

magicStone::~magicStone()
{
}

msYongyong::~msYongyong()
{
}

msBangrang::~msBangrang()
{
}

msWind::~msWind()
{
}

msBooung::~msBooung()
{
}

msBunpok::~msBunpok()
{
}

msNungang::~msNungang()
{
}

msBuljak::~msBuljak()
{
}

msPostion::~msPostion()
{
}
