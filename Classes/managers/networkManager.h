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
	void getInitRoundData(
		short outSecretDeck[],
		short outPlayer1Hand[],
		short outPlayer2Hand[],
		short outPlayer3Hand[],
		short outPlayer4Hand[]
	);
	void getPickedMagicNetData(short *outMagicEnum, int *outPlayerIdx, int *outDamageValue);
	void getRefillNetData(short outRefillHand[], short *outDrawCnt, int *outCurPlayerIdx);

	//client request function
	void requestSettingNPC(int id);
	void requestSettingOrder(int playerTurnOrder[]);
	void requestGameRoomStart();
	void requestGameRoomSceneReady();
	void requestCheckOwnedMagic(short pickedMagicType, int curTurnPlayerIdx, int damageValue = 1);
	void requestRefillHand(short bufRefillHand[], short bufSize, int curPlayerIdx);

	//host request function
	void requestHostSetRound(
		short secretDeck[],
		short player1hand[],
		short player2hand[],
		short player3hand[],
		short player4hand[]
	);

	//set network data
	void setMyClientID(int id);
	void setCurPlayersLoginState(bool loginStateList[]);
	void setJoinedNewPlayer(int joinedPlayerId);
	void setDisconnectPlayer(int disconnectPlayerId);
	void setNpc(int NpcId);
	void setTurnOrder(int turnOrderList[]);
	void setFlagReadyLoadingGameRoom();
	void setFlagLoadingGameRooom();
	void setGameRoomSceneReady(int readyId);
	void setRoundByHostData(
		short secretDeck[],
		short player1Hand[],
		short player2Hand[],
		short player3Hand[],
		short player4Hand[]
	);
	void setPickedMagicData(const short pickedMagic, const int curPlayerIdx, const int damageValue);
	void setRefillNetData(short refillList[], const short refillSize, const short curPlayerIdx);
	
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
	std::array<netPlayer, netProtocol::maxSecretCnt> playerList;
	std::array<int, netProtocol::maxSecretCnt> playerTurnOrder;
	std::array<short, netProtocol::maxSecretCnt> secretDeckList;

	//netData buffer
	short bufPickedMagicType;
	int bufCurTurnPlayerIdx;
	int bufDamageValue;
	short bufRefillHand[netProtocol::maxPlayerHandCnt];
	short bufRefillSize;

	//flags
	gameMetaData::gameMode curGameMode;
	gameMetaData::gameProgressStage curProgressStage;
	bool turnOrderSettingState;
};