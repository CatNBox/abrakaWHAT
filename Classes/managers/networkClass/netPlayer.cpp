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
