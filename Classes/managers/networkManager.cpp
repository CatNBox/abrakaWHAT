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
	for (int i = 0; i < playerList.size(); i++)
	{
		playerList[i].init();
	}
	/*
	for (auto &i : playerList)
	{
		i.init();
	}
	*/
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

void networkManager::getInitRoundData(short outSecretDeck[], short outPlayer1Hand[], short outPlayer2Hand[], short outPlayer3Hand[], short outPlayer4Hand[])
{
	//call from gameRoomObjLayer::update()
	memcpy_s(outSecretDeck, gameMetaData::defaultSecretCnt * sizeof(short), this->secretDeckList.data(), gameMetaData::defaultSecretCnt * sizeof(short));

	playerList[0].getHandList(outPlayer1Hand);
	playerList[1].getHandList(outPlayer2Hand);
	playerList[2].getHandList(outPlayer3Hand);
	playerList[3].getHandList(outPlayer4Hand);

	curProgressStage = gameMetaData::gameProgressStage::roundSetReady;
}

void networkManager::getPickedMagicNetData(short *outMagicEnum, int * outPlayerIdx, int * outDamageValue)
{
	if ((outMagicEnum == nullptr) || (outPlayerIdx == nullptr) || (outDamageValue == nullptr))
	{
		//output variable null handling
		return;
	}

	*outMagicEnum = bufPickedMagicType;
	*outPlayerIdx = bufCurTurnPlayerIdx;
	*outDamageValue = bufDamageValue;

	curProgressStage = gameMetaData::gameProgressStage::takeMagicResult;
}

void networkManager::getRefillNetData(short outRefillHand[], short * outDrawCnt, int * outCurPlayerIdx)
{
	if ((outRefillHand == nullptr) || (outDrawCnt == nullptr) || (outCurPlayerIdx == nullptr))
	{
		//output variable null handling
		return;
	}

	memcpy_s(outRefillHand, sizeof(short)*netProtocol::maxPlayerHandCnt, bufRefillHand, sizeof(short)*netProtocol::maxPlayerHandCnt);
	*outDrawCnt = bufRefillSize;
	*outCurPlayerIdx = bufCurTurnPlayerIdx;
}

void networkManager::requestSettingNPC(int id)
{
	//send req_npc to all players
	netProtocol::PKT_REQ_NPC sendPkt;
	sendPkt.init();
	sendPkt.npcSessionID = id;

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::requestSettingOrder(int playerTurnOrder[])
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

void networkManager::requestCheckOwnedMagic(short pickedMagicType, int curTurnPlayerIdx, int damageValue)
{
	netProtocol::PKT_REQ_CHECKMAGIC sendPkt;
	sendPkt.init();
	sendPkt.pickedMagicType = pickedMagicType;
	sendPkt.curTurnPlayerIdx = curTurnPlayerIdx;
	sendPkt.damageValue = damageValue;

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::requestRefillHand(short bufRefillHand[], short bufSize, int curPlayerIdx)
{
	//send req_refill to server for echo all players
	netProtocol::PKT_REQ_REFILL sendPkt;
	sendPkt.init();
	memcpy_s(sendPkt.refillHand, sizeof(short)*bufSize, bufRefillHand, sizeof(short)*bufSize);
	sendPkt.refillSize = bufSize;
	sendPkt.curTurnPlayerIdx = curPlayerIdx;

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::requestHostSetRound(short secretDeck[], 
	short player1hand[], short player2hand[], short player3hand[], short player4hand[])
{
	netProtocol::PKT_HOST_SETROUND sendPkt;
	sendPkt.init();

	memcpy_s(sendPkt.secretMsList, sizeof(short)*netProtocol::maxSecretCnt, secretDeck, sizeof(short)*netProtocol::maxSecretCnt);
	memcpy_s(sendPkt.player1MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, player1hand, sizeof(short)*netProtocol::maxPlayerHandCnt);
	memcpy_s(sendPkt.player2MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, player2hand, sizeof(short)*netProtocol::maxPlayerHandCnt);
	memcpy_s(sendPkt.player3MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, player3hand, sizeof(short)*netProtocol::maxPlayerHandCnt);
	memcpy_s(sendPkt.player4MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, player4hand, sizeof(short)*netProtocol::maxPlayerHandCnt);

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
}

void networkManager::setMyClientID(int id)
{
	this->myClientId = id;
}

void networkManager::setCurPlayersLoginState(bool loginStateList[])
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

void networkManager::setTurnOrder(int turnOrderList[])
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

void networkManager::setRoundByHostData(short secretDeck[], 
	short player1Hand[], short player2Hand[], short player3Hand[], short player4Hand[])
{
	memcpy_s(secretDeckList.data(), sizeof(short)*netProtocol::maxSecretCnt, secretDeck, sizeof(short)*netProtocol::maxSecretCnt);

	playerList[0].setHandList(player1Hand);
	playerList[1].setHandList(player2Hand);
	playerList[2].setHandList(player3Hand);
	playerList[3].setHandList(player4Hand);

	curProgressStage = gameMetaData::gameProgressStage::roundNetDataReady;
}

void networkManager::setPickedMagicData(const short pickedMagic, const int curPlayerIdx, const int damageValue)
{
	//picked magic data on current turn
	bufPickedMagicType = pickedMagic;
	bufCurTurnPlayerIdx = curPlayerIdx;
	bufDamageValue = damageValue;

	curProgressStage = gameMetaData::gameProgressStage::requestCheckOwnedMagic;
}

void networkManager::setRefillNetData(short refillList[], const short refillSize, const short curPlayerIdx)
{
	memcpy_s(bufRefillHand, sizeof(short)*netProtocol::maxPlayerHandCnt, refillList, sizeof(short)*netProtocol::maxPlayerHandCnt);
	bufRefillSize = refillSize;
	bufCurTurnPlayerIdx = curPlayerIdx;

	curProgressStage = gameMetaData::gameProgressStage::requestRefillHand;
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
