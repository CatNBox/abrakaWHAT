#include "hostServer.h"
#include "session.h"
#include "managers\networkClass\protocol.h"
#include <iostream>
#include "gameObject/gameMetaData.h"

hostServer::hostServer(boost::asio::io_service& ioServ)
	:acceptor(ioServ, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),netProtocol::port))
{
	initSessionPool(netProtocol::maxSessionCnt);
	serverIo = &ioServ;
	serverAcceptingState = true;
}

hostServer::~hostServer()
{
	for (auto &i : vecSessionPool)
	{
		if (i->getSocket().is_open() == true)
		{
			i->getSocket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			i->getSocket().close();
		}

		delete i;
	}
}

void hostServer::initSessionPool(const int sessionMaxCnt)
{
	for (int i = 0; i < sessionMaxCnt; i++)
	{
		session* tempSession = new session(i, acceptor.get_io_service(), this);
		vecSessionPool.push_back(tempSession);
		queSessionId.push(i);
	}
}

void hostServer::start()
{
	startAccept();
}

void hostServer::closeSession(const int sessionID)
{
	vecSessionPool.at(sessionID)->getSocket().close();
	queSessionId.push(sessionID);

	if (serverAcceptingState == false)
	{
		acceptor.open(boost::asio::ip::tcp::v4());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), netProtocol::port));
		startAccept();
	}
}

void hostServer::processPacket(const int sessionId, const char & pData)
{
	//packet define in protocol.h
	netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)&pData;

	switch (pHeader->pktId)
	{
	case netProtocol::pktIdentt::REQ_CNT : //login packet
		{
			netProtocol::PKT_REQ_CNT* pPacket = (netProtocol::PKT_REQ_CNT*)&pData;
			//set user info
			std::cout << "$$ LOG IN session : " << sessionId << std::endl;

			//response playerCnt message
			netProtocol::PKT_RES_CNT sendPkt;
			sendPkt.init();
			int tempCnt = gameMetaData::defaultPlayerCnt - queSessionId.size();
			if (tempCnt < 0) tempCnt = 0;
			sendPkt.playerCnt = tempCnt;
			std::cout << "$$ playerCnt : " << sendPkt.playerCnt << std::endl;
			std::cout << "$$ defaultPlayerCnt : " << gameMetaData::defaultPlayerCnt << std::endl;
			std::cout << "$$ queSessionId : " << queSessionId.size() << std::endl;

			for (auto i : vecSessionPool)
			{
				if (i->getSocket().is_open())
				{
					i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
				}
			}
		}
		break;
	default:
		break;
	}
}

void hostServer::startAccept()
{
	if (queSessionId.empty() == false)
	{
		serverAcceptingState = true;
		auto curSession = vecSessionPool[queSessionId.front()];
		acceptor.async_accept(
			curSession->getSocket(),
			boost::bind(&hostServer::handleAccept, this,
				curSession,
				boost::asio::placeholders::error
			)
		);
	}
}

void hostServer::handleAccept(session* pSession, const boost::system::error_code& errCode)
{
	if (!errCode)
	{
		//connect success session work
		std::cout << "$$ Connect SUCCESS Client" << std::endl;

		pSession->init();
		pSession->startReceive();

		postAcceptSuccess();
	}
	else
	{
		std::cout << "$$ Connect FAIL errCode : " << errCode.value()
			<< " errMessage : " << errCode.message()
			<< std::endl;
	}
}

bool hostServer::postAcceptSuccess()
{
	queSessionId.pop();

	if (queSessionId.empty())
	{
		acceptor.close();
		serverAcceptingState = false;
		return false;
	}

	startAccept();

	return true;
}
