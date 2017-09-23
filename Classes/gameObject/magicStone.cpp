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

ms_yongyong::~ms_yongyong()
{
}

ms_bangrang::~ms_bangrang()
{
}

ms_wind::~ms_wind()
{
}

ms_booung::~ms_booung()
{
}

ms_bunpok::~ms_bunpok()
{
}

ms_nungang::~ms_nungang()
{
}

ms_buljak::~ms_buljak()
{
}

ms_postion::~ms_postion()
{
}
