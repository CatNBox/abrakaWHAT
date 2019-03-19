#pragma once
#include "gameObject/gameMetaData.h"
#include <array>

class netPlayer
{
public:
	void init();
	void setPlayerID(int id);
	void setNpcState(gameMetaData::gameMode curGameMode);
	void connectSuccess();
	void disconnect();
	gameMetaData::netPlayerState getConnectionState();
	void setGameRoomReady();
	bool getGameRoomReadyState();

private:
	int playerID;
	gameMetaData::netPlayerState connectionState;
	std::array<gameMetaData::msType, 5> handList;
	bool gameRoomReady;
};