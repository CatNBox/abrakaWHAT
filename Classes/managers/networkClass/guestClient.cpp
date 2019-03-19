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
	if (errCode)
	{
		std::cout
			<< "---------------------" << std::endl 
			<< "Connect fail " << std::endl;
		std::cout << "Error Code : " << errCode.value() << " Error Message : " << errCode.message() << std::endl
			<< "---------------------" << std::endl;

		//popup connect error

		networkManager::getInstance()->clientConnectFail();
	}
	else
	{
		std::cout
			<< "---------------------" << std::endl 
			<< "Server connect success" << std::endl
			<< "---------------------" << std::endl;

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
			std::cout
				<< "---------------------" << std::endl 
				<< "error No : " << errCode.value() << " error Message : " << errCode.message() << std::endl
				<< "---------------------" << std::endl;
		}
		else
		{
			std::cout
				<< "---------------------" << std::endl 
				<< "error No : " << errCode.value() << " error Message : " << errCode.message() << std::endl
				<< "---------------------" << std::endl;
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
	case netProtocol::pktIdentt::RES_IN:
		{
			netProtocol::PKT_RES_IN* pPacket = (netProtocol::PKT_RES_IN*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : RES_IN" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet mySessionID : " << pPacket->mySessionID << std::endl;
			for (int i = 0; i < netProtocol::maxSessionCnt; i++)
			{
				std::cout << "packet userState[" << i << "] : " << pPacket->userStateList[i] << std::endl;
			}
			std::cout << "---------------------" << std::endl;

			bool curUserLoginState[netProtocol::maxSessionCnt];
			memcpy(curUserLoginState, pPacket->userStateList, sizeof(bool)*netProtocol::maxSessionCnt);
			networkManager::getInstance()->setCurPlayersLoginState(curUserLoginState);

			int myClientID = pPacket->mySessionID;
			networkManager::getInstance()->setMyClientID(myClientID);

			networkManager::getInstance()->clientConnectSuccess();
		}
		break;
	case netProtocol::pktIdentt::NOTICE_IN:
		{
			netProtocol::PKT_NOTICE_IN* pPacket = (netProtocol::PKT_NOTICE_IN*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : NOTICE_IN" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet joinedUserId : " << pPacket->joinedUserId << std::endl
				<< "---------------------" << std::endl;

			int joinedPlayerId = pPacket->joinedUserId;

			networkManager::getInstance()->setJoinedNewPlayer(joinedPlayerId);
		}
		break;
	case netProtocol::pktIdentt::NOTICE_OUT:
		{
			netProtocol::PKT_NOTICE_OUT* pPacket = (netProtocol::PKT_NOTICE_OUT*)&pData;
			
			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : NOTICE_OUT" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet joinedUserId : " << pPacket->disconnectUserId << std::endl
				<< "---------------------" << std::endl;

			int disconnectPlayerId = pPacket->disconnectUserId;

			networkManager::getInstance()->setDisconnectPlayer(disconnectPlayerId);
		}
		break;
	case netProtocol::pktIdentt::NOTICE_NPC:
		{
			netProtocol::PKT_NOTICE_NPC* pPacket = (netProtocol::PKT_NOTICE_NPC*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : NOTICE_NPC" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet npcID : " << pPacket->npcId << std::endl
				<< "---------------------" << std::endl;

			int npcId = pPacket->npcId;

			networkManager::getInstance()->setNpc(npcId);
		}
		break;
	case netProtocol::pktIdentt::NOTICE_ORDER:
		{
			netProtocol::PKT_NOTICE_ORDER* pPacket = (netProtocol::PKT_NOTICE_ORDER*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : NOTICE_ORDER" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl;
			for (int i = 0; i < netProtocol::maxSessionCnt; i++)
			{
				std::cout << "packet turnOrder[" << i << "] : " << pPacket->turnOrderList[i] << std::endl;
			}
			std::cout
				<< "---------------------" << std::endl;

			int receiveTurnOrder[netProtocol::maxSessionCnt];
			memcpy(receiveTurnOrder, pPacket->turnOrderList, sizeof(int)*netProtocol::maxSessionCnt);
			networkManager::getInstance()->setTurnOrder(receiveTurnOrder);
		}
		break;
	case netProtocol::pktIdentt::NOTICE_START:
		{
			netProtocol::PKT_NOTICE_START* pPacket = (netProtocol::PKT_NOTICE_START*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : NOTICE_START" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "---------------------" << std::endl;

			networkManager::getInstance()->setFlagReadyLoadingGameRoom();
		}
		break;
	case netProtocol::pktIdentt::NOTICE_READY:
		{
			netProtocol::PKT_NOTICE_READY* pPacket = (netProtocol::PKT_NOTICE_READY*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : NOTICE_READY" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet readyID : " << pPacket->readyId << std::endl
				<< "---------------------" << std::endl;

			networkManager::getInstance()->setGameRoomSceneReady(pPacket->readyId);
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
