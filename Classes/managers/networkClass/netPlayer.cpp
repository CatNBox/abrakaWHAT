#include "netPlayer.h"

void netPlayer::init()
{
	playerID = -1;
	connectionState = gameMetaData::netPlayerState::disconnected;
	for (auto &i : handList)
	{
		i = gameMetaData::msType::pass;
	}
	gameRoomReady = false;
}

void netPlayer::setPlayerID(int id)
{
	playerID = id;
}

void netPlayer::setNpcState(gameMetaData::gameMode curGameMode)
{
	if (curGameMode == gameMetaData::gameMode::host)
	{
		connectionState = gameMetaData::netPlayerState::hostNPC;
	}
	else
	{
		connectionState = gameMetaData::netPlayerState::guestNPC;
	}
	gameRoomReady = true;
}

void netPlayer::setHandList(short hand[])
{
	memcpy_s(handList.data(), sizeof(short) * gameMetaData::defaultHandCnt, hand, sizeof(short) * gameMetaData::defaultHandCnt);
}

void netPlayer::connectSuccess()
{
	connectionState = gameMetaData::netPlayerState::connected;
}

void netPlayer::disconnect()
{
	connectionState = gameMetaData::netPlayerState::disconnected;
}

gameMetaData::netPlayerState netPlayer::getConnectionState()
{
	return connectionState;
}

void netPlayer::setGameRoomReady()
{
	gameRoomReady = true;
}

bool netPlayer::getGameRoomReadyState()
{
	return gameRoomReady;
}

void netPlayer::getHandList(short * outputList)
{
	if (outputList == nullptr)
	{
		//error handling
		return;
	}

	memcpy_s(outputList, sizeof(short) * gameMetaData::defaultHandCnt, handList.data(), sizeof(short) * gameMetaData::defaultHandCnt);
}
