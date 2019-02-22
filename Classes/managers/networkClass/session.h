#pragma once
#include <boost\asio.hpp>
#include <boost\system\error_code.hpp>
#include <queue>
#include "managers\networkClass\protocol.h"

class hostServer;

class session
{
public:
	session(int id, boost::asio::io_service& refIoServ, hostServer* refServer);
	~session();
	void init();
	boost::asio::ip::tcp::socket& getSocket();
	const int getSessionId();

	void startReceive();
	void startSend(const bool isImmediatly, const int dataSize, char& pData);

private:
	void handleReceive(const boost::system::error_code& errCode, size_t bytes_transferred);
	void handleWrite(const boost::system::error_code& errCode, size_t bytes_transferred);

	//session info
	int sessionId;
	hostServer* pServer;
	boost::asio::ip::tcp::socket sessionSocket;

	//for receive
	std::array<char, netProtocol::maxReceiveBufLen> receiveBuffer;
	int pktBufStartMarker;
	char willProcessPktBuffer[netProtocol::maxReceiveBufLen * 2];

	//for send
	std::queue<char*> sendDataQue;
};