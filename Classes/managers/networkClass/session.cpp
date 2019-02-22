#include "session.h"
#include "hostServer.h"
#include <boost\bind.hpp>
#include <iostream>

session::session(int id, boost::asio::io_service& refIoServ, hostServer* refServer)
	:sessionId(id),
	sessionSocket(refIoServ),
	pServer(refServer)
{
	init();
}

session::~session()
{
	while (sendDataQue.empty() == false)
	{
		delete[] sendDataQue.front();
		sendDataQue.pop();
	}
}

void session::init()
{
	pktBufStartMarker = 0;
}

boost::asio::ip::tcp::socket & session::getSocket()
{
	return sessionSocket;
}

const int session::getSessionId()
{
	return sessionId;
}

void session::startReceive()
{
	sessionSocket.async_read_some(
		boost::asio::buffer(receiveBuffer),
		boost::bind(
			&session::handleReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void session::startSend(const bool isImmediatly, const int dataSize, char & pData)
{
	char* pSendData = nullptr;
	
	if (isImmediatly == false)
	{
		pSendData = new char[dataSize];
		memcpy(pSendData, &pData, dataSize);

		sendDataQue.push(pSendData);
	}
	else
	{
		pSendData = &pData;
	}

	if (isImmediatly == false && sendDataQue.size() >= 2)
	{
		return;
	}

	boost::asio::async_write(
		sessionSocket, boost::asio::buffer(pSendData, dataSize),
		boost::bind(
			&session::handleWrite, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void session::handleReceive(const boost::system::error_code & errCode, size_t bytes_transferred)
{
	if (errCode)
	{
		//error handling
		if (errCode.value() == boost::asio::error::eof)
		{
			//disconnect
			std::cout << "Disconnect client id : " << sessionId << std::endl; //log will be write txtfile
		}
		else if (errCode.value() == boost::asio::error::connection_reset)
		{
			//disconnect
			std::cout << "Disconnect client id : " << sessionId << std::endl; //log will be write txtfile

			//call session return
			pServer->closeSession(sessionId);
		}
		else
		{
			//etc error log
			std::cout << "error No : " << errCode.value() << " error Message : " << errCode.message() << std::endl;
		}
	}
	else
	{
		//receiveBuffer is registed on async_read_some, then it have rawData
		//willProcessPktBuffer will be process to valid packet, then it have to copy from receiveBuffer
		memcpy(&willProcessPktBuffer[pktBufStartMarker], receiveBuffer.data(), bytes_transferred);

		int willProcessDataSize = pktBufStartMarker + bytes_transferred;
		int readDataMarker = 0; // it was read data location

		while (willProcessDataSize > 0)
		{
			if (willProcessDataSize < sizeof(netProtocol::PACKET_HEADER))
			{
				//not enough size to process valid packet
				break;
			}

			netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)&willProcessPktBuffer[readDataMarker];

			if (pHeader->pktSize <= willProcessDataSize)
			{
				//call processPacket
				pServer->processPacket(sessionId, willProcessPktBuffer[readDataMarker]);

				willProcessDataSize -= pHeader->pktSize;
				readDataMarker += pHeader->pktSize;
			}
			else
			{
				//not enough remain size to process valid packet
				break;
			}
		}

		//remain some data
		if (willProcessDataSize > 0)
		{
			//remain Data write willProcessPktBuffer
			char tempBuffer[netProtocol::maxReceiveBufLen] = { 0, };
			memcpy(&tempBuffer[0], &willProcessPktBuffer[readDataMarker], willProcessDataSize);
			memcpy(&willProcessPktBuffer[0], &tempBuffer[0], willProcessDataSize);
		}

		pktBufStartMarker = willProcessDataSize; //remain data size //[remain]+[receiveData]

		startReceive();
	}
}

void session::handleWrite(const boost::system::error_code & errCode, size_t bytes_transferred)
{
	delete[] sendDataQue.front();
	sendDataQue.pop();

	if (sendDataQue.empty() == false)
	{
		char* pData = sendDataQue.front();
		netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)pData;

		startSend(true, pHeader->pktSize, *pData);
	}
}
