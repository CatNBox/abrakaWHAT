#pragma once
#include "gameObject/gameMetaData.h"
#include <array>

class netPlayer
{
public:
	void init();
	void setPlayerID(int id);
	void setNpcState(gameMetaData::gameMode curGameMode);
	void setHandList(short hand[]);
	void connectSuccess();
	void disconnect();
	gameMetaData::netPlayerState getConnectionState();
	void setGameRoomReady();
	bool getGameRoomReadyState();
	void getHandList(short *outputList);

private:
	int playerID;
	gameMetaData::netPlayerState connectionState;
	std::array<short, gameMetaData::defaultHandCnt> handList;
	bool gameRoomReady;
};