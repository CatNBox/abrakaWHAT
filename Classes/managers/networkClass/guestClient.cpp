#include "guestClient.h"
#include <boost/bind.hpp>
#include "managers/networkManager.h"
#include <iostream>

guestClient::guestClient(boost::asio::io_service & ioServ)
	:socket(ioServ)
{
	loginState = false;
}

guestClient::~guestClient()
{
}

void guestClient::close()
{
	if (socket.is_open())
	{
		socket.shutdown(socket.shutdown_both);
		socket.close();
	}
}

void guestClient::connect(boost::asio::ip::tcp::endpoint endPoint)
{
	pktBufStartMarker = 0;
	socket.async_connect(
		endPoint,
		boost::bind(
			&guestClient::handleConnect, this,
			boost::asio::placeholders::error
		)
	);
}

void guestClient::startSend(const bool isImmediately, const int dataSize, char & pData)
{
	char* sendData = nullptr;

	if (isImmediately == false)
	{
		sendData = new char[dataSize];
		memcpy(sendData, &pData, dataSize);

		sendDataQue.push(sendData);
	}
	else
	{
		sendData = &pData;
	}

	if (isImmediately || sendDataQue.size() < 2)
	{
		boost::asio::async_write(
			socket, boost::asio::buffer(sendData, dataSize),
			boost::bind(
				&guestClient::handleWrite, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
}

void guestClient::handleConnect(const boost::system::error_code & errCode)
{
	std::cout << "Error Code : " << errCode.value() << " Error Message : " << errCode.message() << std::endl;

	if (errCode)
	{
		std::cout << "Connect fail " << std::endl;
		std::cout << "Error Code : " << errCode.value() << " Error Message : " << errCode.message() << std::endl;

		//popup connect error

		networkManager::getInstance()->clientConnectFail();
	}
	else
	{
		std::cout << "Server connect success" << std::endl;
		std::cout << "" << std::endl;

		networkManager::getInstance()->clientConnectSuccess(); 
		
		netProtocol::PKT_REQ_CNT sendPkt;
		sendPkt.init();

		this->startSend(false, sendPkt.pktSize, (char&)sendPkt);

		startReceive();
	}
}

void guestClient::startReceive()
{
	memset(&receiveBuffer, '\0', sizeof(receiveBuffer));

	socket.async_read_some(
		boost::asio::buffer(receiveBuffer),
		boost::bind(
			&guestClient::handleReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void guestClient::handleReceive(const boost::system::error_code & errCode, const size_t bytes_tranferred)
{
	//error check and call processPacket
	if (errCode)
	{
		if (errCode == boost::asio::error::eof)
		{
			//std::cout << "connect fail" << std::endl;
		}
		else
		{
			//std::cout << "error No : " << errCode.value() << " error Message : " << errCode.message() << std::endl;
		}

		this->close();
	}
	else
	{
		memcpy(&willProcessPktBuffer[pktBufStartMarker], receiveBuffer.data(), bytes_tranferred);

		int willProcessDataSize = pktBufStartMarker + bytes_tranferred;
		int readDataStartMarker = 0;

		while (willProcessDataSize > 0)
		{
			if (willProcessDataSize < sizeof(netProtocol::PACKET_HEADER))
			{
				break;
			}

			netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)&willProcessPktBuffer[readDataStartMarker];

			if (pHeader->pktSize <= willProcessDataSize)
			{
				this->processPacket(willProcessPktBuffer[readDataStartMarker]);

				willProcessDataSize -= pHeader->pktSize;
				readDataStartMarker += pHeader->pktSize;
			}
			else
			{
				break;
			}
		}

		if (willProcessDataSize > 0)
		{
			char tempBuffer[netProtocol::maxReceiveBufLen] = { 0, };
			memcpy(&tempBuffer[0], &willProcessPktBuffer[readDataStartMarker], willProcessDataSize);
			memcpy(&willProcessPktBuffer[0], &tempBuffer[0], willProcessDataSize);
		}

		pktBufStartMarker = willProcessDataSize;

		this->startReceive();
	}
}

void guestClient::processPacket(const char & pData)
{
	netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)&pData;

	switch (pHeader->pktId)
	{
	case netProtocol::pktIdentt::RES_CNT:
		{
			netProtocol::PKT_RES_CNT* pPacket = (netProtocol::PKT_RES_CNT*)&pData;

			int tempPlayerCnt = pPacket->playerCnt;
			std::cout << "tempPlayerCnt : " << tempPlayerCnt << std::endl;

			//updatePlayerCnt call waitingUILayer::updatePlayerLabel
			networkManager::getInstance()->updatePlayerCnt(tempPlayerCnt);
		}
		break;
	default:
		break;
	}
}

void guestClient::handleWrite(const boost::system::error_code, size_t bytes_transferred)
{
	delete[] sendDataQue.front();
	sendDataQue.pop();

	char* pData = nullptr;

	if (sendDataQue.empty() == false)
	{
		pData = sendDataQue.front();
	}

	if (pData != nullptr)
	{
		netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)pData;
		startSend(true, pHeader->pktSize, *pData);
	}
}
