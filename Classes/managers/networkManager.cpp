#include "managers\networkManager.h"
#include "gameObject\gameMetaData.h"
#include "managers\networkClass\protocol.h"
#include <boost\system\error_code.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "managers/networkClass/guestClient.h"
#include "managers/networkClass/hostServer.h"

void networkManager::init()
{
	this->clientIo = nullptr;
	this->clientThread = nullptr;
	this->myClient = nullptr;
	this->serverIo = nullptr;
	this->serverThread = nullptr;
	this->server = nullptr;
	this->clientConnectionState = false;
}

void networkManager::start(gameMetaData::gameMode modeFlag)
{
	curGameMode = modeFlag;
	curPlayerCnt = 0;

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
	
	clientIo = new boost::asio::io_service;
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
	//호스트면 바로 playerCnt를 요청
	//게스트면 layer변경이벤트를 먼저 호출한 후 playerCnt를 요청
	if (curGameMode == gameMetaData::gameMode::host)
	{
		//requestCurPlayerCnt();
	}
	else if (curGameMode == gameMetaData::gameMode::guest)
	{
		clientConnectionState = true;
		//requestCurPlayerCnt();
	}
}

void networkManager::runServer()
{
	serverIo = new boost::asio::io_service;
	server = new hostServer(*serverIo);
	server->start();
	
	//server On
	serverThread = new boost::thread(boost::bind(&boost::asio::io_service::run, serverIo));
}

std::string networkManager::getMyAddr()
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

bool networkManager::getClientConnectionState()
{
	return clientConnectionState;
}

void networkManager::requestCurPlayerCnt()
{
	netProtocol::PKT_REQ_CNT sendPkt;
	sendPkt.init();

	myClient->startSend(false, sendPkt.pktSize, (char&)sendPkt);
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
