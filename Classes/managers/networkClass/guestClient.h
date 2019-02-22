#pragma once
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>
#include <queue>
#include "protocol.h"

class guestClient
{
public:
	guestClient(boost::asio::io_service& ioServ);
	~guestClient();

	void close();
	void connect(boost::asio::ip::tcp::endpoint endPoint);
	void startSend(const bool isImmediately, const int dataSize, char& pData);

private:
	void handleConnect(const boost::system::error_code& errCode);
	void startReceive();
	void handleReceive(const boost::system::error_code& errCode, const size_t bytes_tranferred);
	void processPacket(const char& pData);

	void handleWrite(const boost::system::error_code, size_t bytes_transferred);

	bool loginState;

	int pktBufStartMarker;
	std::array<char, netProtocol::maxReceiveBufLen> receiveBuffer;
	char willProcessPktBuffer[netProtocol::maxReceiveBufLen * 2];

	std::queue<char*> sendDataQue;

	boost::asio::ip::tcp::socket socket;
};