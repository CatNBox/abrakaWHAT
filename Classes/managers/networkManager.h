#pragma once
#include <boost\asio.hpp>
#include <boost/thread.hpp>
#include "managers\singleton.h"
#include "managers\networkClass\protocol.h"
#include "managers\networkClass\stream.h"
#include "managers/networkClass/netPlayer.h"
#include <array>
#include "cocos2d.h"

class guestClient;
class hostServer;

class networkManager final : public singleton<networkManager>
{
public:
	void init(gameMetaData::gameMode modeFlag);
	void start();
	void close();
	bool checkTurnOn();

	void runClient(const std::string hostAddr);
	void clientConnectFail();
	void clientConnectSuccess();
	void runServer();

	std::string getServerAddr();

	void updatePlayerCnt(const int playerCnt);

	//cocos2d node request function
	int getPlayerCnt();
	int getNPCCnt();
	gameMetaData::netPlayerState getMyClientConnectionState();
	gameMetaData::netPlayerState getPlayerConnectionState(const int id);
	bool getTurnOrderSettingState();
	int getTurnOrder(const int id);
	gameMetaData::gameProgressStage getNetworkProgressStage();
	bool getPlayerGameRoomReadyState(const int playerId);
	int getMyClientId();
	bool isAllPlayerReady();

	//client request function
	void requestSettingNPC(int id);
	void requestSettingOrder(int* playerTurnOrder);
	void requestGameRoomStart();
	void requestGameRoomSceneReady();

	//set network data
	void setMyClientID(int id);
	void setCurPlayersLoginState(bool* loginStateList);
	void setJoinedNewPlayer(int joinedPlayerId);
	void setDisconnectPlayer(int disconnectPlayerId);
	void setNpc(int NpcId);
	void setTurnOrder(int* turnOrderList);
	void setFlagReadyLoadingGameRoom();
	void setFlagLoadingGameRooom();
	void setGameRoomSceneReady(int readyId);
	
private:

	void setMyServerAddr();
	void updateCurPlayerCnt();

	int myClientId;
	int curPlayerCnt;
	int curNPCCnt;

	std::string serverAddr;

	boost::asio::io_service* clientIo;
	boost::thread* clientThread;
	guestClient* myClient;

	boost::asio::io_service* serverIo;
	boost::thread* serverThread;
	hostServer* server;

	//playerList
	std::array<netPlayer, 4> playerList;
	std::array<int, 4> playerTurnOrder;

	//flags
	gameMetaData::gameMode curGameMode;
	gameMetaData::gameProgressStage curProgressStage;
	bool turnOrderSettingState;
};