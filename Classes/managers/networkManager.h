#pragma once
#include <boost\asio.hpp>
#include <boost/thread.hpp>
#include "managers\singleton.h"
#include "managers\networkClass\protocol.h"
#include "managers\networkClass\stream.h"
#include "cocos2d.h"

class guestClient;
class hostServer;

namespace gameMetaData
{
	enum class gameMode;
}

class networkManager final : public singleton<networkManager>
{
public:
	void init();
	void start(gameMetaData::gameMode modeFlag);
	void close();
	bool checkTurnOn();

	void runClient(const std::string hostAddr);
	void clientConnectFail();
	void clientConnectSuccess();
	void runServer();

	std::string getMyAddr();

	void updatePlayerCnt(const int playerCnt);
	int getPlayerCnt();

	bool getClientConnectionState();

	//client request function
	void requestCurPlayerCnt();
	
private:

	void setMyServerAddr();

	gameMetaData::gameMode curGameMode;
	int curPlayerCnt;

	std::string serverAddr;

	boost::asio::io_service* clientIo;
	boost::thread* clientThread;
	guestClient* myClient;

	boost::asio::io_service* serverIo;
	boost::thread* serverThread;
	hostServer* server;

	//flags
	bool clientConnectionState;
};