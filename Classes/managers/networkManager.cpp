#include "managers\networkManager.h"
#include "gameObject\gameMetaData.h"
#include "managers\networkClass\protocol.h"
#include <boost\system\error_code.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "managers/networkClass/guestClient.h"
#include "managers/networkClass/hostServer.h"

void networkManager::init(gameMetaData::gameMode modeFlag)
{
	curGameMode = modeFlag;
	curProgressStage = gameMetaData::gameProgressStage::waiting;
	curPlayerCnt = 0;
	curNPCCnt = 0;
	turnOrderSettingState = false;
	this->clientIo = nullptr;
	this->clientThread = nullptr;
	this->myClient = nullptr;
	this->serverIo = nullptr;
	this->serverThread = nullptr;
	this->server = nullptr;
	this->myClientId = -1;
	for (auto i : playerList)
	{
		i.init();
	}
}

void networkManager::start()
{

	if (curGameMode == gameMetaData::gameMode::single)
	{
		//nothing
		return;
	}

	if (curGameMode == gameMetaData::gameMode::host)
	{
		runServer();
		setMyServerAddr();
		runClient(serverAddr);
	}
	else if (curGameMode == gameMetaData::gameMode::guest)
	{
	}
}

void networkManager::close()
{
	if (curGameMode == gameMetaData::gameMode::single)
	{
		return;
	}
	else if (checkTurnOn() == false)
	{
		//no work, when not alloc network class pointers
		return;
	}

	std::cout << "$$ work close()" << std::endl;

	//io_service stop and thread join
	if (curGameMode == gameMetaData::gameMode::host)
	{
		clientIo->stop();
		myClient->close();
		clientThread->join();
		serverIo->stop();
		serverThread->join();

		delete clientThread;
		delete myClient;
		delete clientIo;
		delete serverThread;
		delete server;
		delete serverIo;
	}
	else if (curGameMode == gameMetaData::gameMode::guest)
	{
		clientIo->stop();
		myClient->close();
		clientThread->join();

		delete clientThread;
		delete myClient;
		delete clientIo;
	}
}

bool networkManager::checkTurnOn()
{
	if (clientIo == nullptr) {
		std::cout << "clientIo null" << std::endl; return false;
	}
	if (clientThread == nullptr) {
		std::cout << "clientThread null" << std::endl; return false;
	}
	if (myClient == nullptr) {
		std::cout << "myClient null" << std::endl; return false;
	}

	if (curGameMode == gameMetaData::gameMode::host)
	{
		if (serverIo == nullptr) {
			std::cout << "serverIo null" << std::endl; return false;
		}
		if (serverThread == nullptr) {
			std::cout << "serverThread null" << std::endl; return false;
		}
		if (server == nullptr) {
			std::cout << "server null" << std::endl; return false;
		}
	}

	return true;
}

void networkManager::runClient(const std::string hostAddr)
{
	auto endPoint = boost::asio::ip::tcp::endpoint(
		boost::asio::ip::address::from_string(hostAddr),
		netProtocol::port
	);

	serverAddr = hostAddr;
	
	auto tempIo = new boost::asio::io_service;
	clientIo = tempIo;
	myClient = new guestClient(*clientIo);
	myClient->connect(endPoint);

	clientThread = new boost::thread(boost::bind(&boost::asio::io_service::run, clientIo));
}

void networkManager::clientConnectFail()
{
	clientIo->stop();
	myClient->close();
	clientThread->join();

	delete clientThread;
	delete myClient;
	delete clientIo;

	//popup connect fail
}

void networkManager::clientConnectSuccess()
{
	playerList[myClientId].connectSuccess();
}

void networkManager::runServer()
{
	serverIo = new boost::asio::io_service;
	server = new hostServer(*serverIo);
	server->start();
	
	//server On
	serverThread = new boost::thread(boost::bind(&boost::asio::io_service::run, serverIo));
}

std::string networkManager::getServerAddr()
{
	return serverAddr;
}

void networkManager::updatePlayerCnt(const int playerCnt)
{
	curPlayerCnt = playerCnt;
}

int networkManager::getPlayerCnt()
{
	return curPlayerCnt;
}

int networkManager::getNPCCnt()
{
	return curNPCCnt;
}

gameMetaData::netPlayerState networkManager::getMyClientConnectionState()
{
	if (myClientId >= 0 && myClientId < playerList.size())
	{
		return playerList[myClientId].getConnectionState();
	}

	return gameMetaData::netPlayerState::unknown;
}

gameMetaData::netPlayerState networkManager::getPlayerConnectionState(const int id)
{
	return playerList[id].getConnectionState();
}

bool networkManager::getTurnOrderSettingState()
{
	return turnOrderSettingState;
}

int networkManager::getTurnOrder(const int id)
{
	return playerTurnOrder[id];
}

gameMetaData::gameProgressStage networkManager::getNetworkProgressStage()
{
	return curProgressStage;
}

bool networkManager::getPlayerGameRoomReadyState(const int playerId)
{
	return playerList[playerId].getGameRoomReadyState();
}

int networkManager::getMyClientId()
{
	return myClientId;
}

bool networkManager::isAllPlayerReady()
{
	bool allReadyState = true;
	for (auto i : playerList)
	{
		if (i.getGameRoomReadyState() == false)
		{
			allReadyState = false;
			break;
		}
	}

	return allReadyState;
}

void networkManager::requestSettingNPC(int id)
{
	//send req_npc to all players
	netProtocol::PKT_REQ_NPC sendPkt;
	sendPkt.init();
	sendPkt.npcSessionID = id;

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::requestSettingOrder(int * playerTurnOrder)
{
	//send req_order to server for echo all players
	netProtocol::PKT_REQ_ORDER sendPkt;
	sendPkt.init();
	memcpy(sendPkt.turnOrderList, playerTurnOrder, sizeof(int)*netProtocol::maxSessionCnt);

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::requestGameRoomStart()
{
	netProtocol::PKT_REQ_START sendPkt;
	sendPkt.init();

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::requestGameRoomSceneReady()
{
	netProtocol::PKT_REQ_READY sendPkt;
	sendPkt.init();

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::setMyClientID(int id)
{
	this->myClientId = id;
}

void networkManager::setCurPlayersLoginState(bool* loginStateList)
{
	for (int i = 0; i < netProtocol::maxSessionCnt; i++)
	{
		if (loginStateList[i])
		{
			playerList[i].connectSuccess();
		}
		else
		{
			playerList[i].disconnect();
		}
	}
	updateCurPlayerCnt();
}

void networkManager::setJoinedNewPlayer(int joinedPlayerId)
{
	playerList[joinedPlayerId].connectSuccess();
	updateCurPlayerCnt();
}

void networkManager::setDisconnectPlayer(int disconnectPlayerId)
{
	playerList[disconnectPlayerId].disconnect();
	updateCurPlayerCnt();
}

void networkManager::setNpc(int NpcId)
{
	playerList[NpcId].setNpcState(this->curGameMode);
	updateCurPlayerCnt();
}

void networkManager::setTurnOrder(int * turnOrderList)
{
	for (int i = 0; i < netProtocol::maxSessionCnt; i++)
	{
		playerTurnOrder[i] = turnOrderList[i];
	}

	turnOrderSettingState = true;
}

void networkManager::setFlagReadyLoadingGameRoom()
{
	curProgressStage = gameMetaData::gameProgressStage::readyLoadingGameRoomScene;
}

void networkManager::setFlagLoadingGameRooom()
{
	curProgressStage = gameMetaData::gameProgressStage::loadingGameRoomScene;
}

void networkManager::setGameRoomSceneReady(int readyId)
{
	playerList[readyId].setGameRoomReady();
}

void networkManager::setMyServerAddr()
{
	boost::asio::ip::tcp::resolver resovler(*serverIo);
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
	boost::asio::ip::tcp::resolver::iterator it = resovler.resolve(query);

	while (it != boost::asio::ip::tcp::resolver::iterator())
	{
		boost::asio::ip::address addr = (it++)->endpoint().address();
		if (addr.is_v4())
		{
			std::cout << addr.to_string() << std::endl;
			serverAddr = addr.to_string();
			break;
		}
	}
}

void networkManager::updateCurPlayerCnt()
{
	curPlayerCnt = 0;
	curNPCCnt = 0;
	for (auto i : playerList)
	{
		auto tempState = i.getConnectionState();
		if (tempState == gameMetaData::netPlayerState::connected)
		{
			curPlayerCnt++;
		}
		else if (tempState == gameMetaData::netPlayerState::guestNPC
			|| tempState == gameMetaData::netPlayerState::hostNPC)
		{
			curNPCCnt++;
		}
	}
}
