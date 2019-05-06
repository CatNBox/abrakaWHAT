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
	gameStartedState = false;
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
	
	if (gameStartedState == false)
	{
		queSessionId.push(sessionID);
		noticePlayerOut(sessionID);

		if (serverAcceptingState == false)
		{
			acceptor.open(boost::asio::ip::tcp::v4());
			acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
			acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), netProtocol::port));
			acceptor.listen();
			startAccept();
		}
	}
	else
	{
		netProtocol::PKT_NOTICE_NPC sendPkt;
		sendPkt.init();
		sendPkt.npcId = sessionID;

		for (auto i : vecSessionPool)
		{
			if (i->getSocket().is_open())
			{
				i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
			}
		}
	}
}

void hostServer::processPacket(const int sessionId, const char & pData)
{
	//packet define in protocol.h
	netProtocol::PACKET_HEADER* pHeader = (netProtocol::PACKET_HEADER*)&pData;

	switch (pHeader->pktId)
	{
	case netProtocol::pktIdentt::REQ_NPC: //set npc session
		{
			netProtocol::PKT_REQ_NPC* pPacket = (netProtocol::PKT_REQ_NPC*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : REQ_NPC" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet npcSessionID : " << pPacket->npcSessionID << std::endl
				<< "packet sessionID : " << sessionId << std::endl
				<< "---------------------" << std::endl;

			auto npcSessionID = pPacket->npcSessionID;

			for (int i = 0; i < queSessionId.size(); i++)
			{
				auto frontSessionID = queSessionId.front();
				if (frontSessionID == npcSessionID)
				{
					
					if (vecSessionPool.at(frontSessionID)->getSocket().is_open())
					{
						vecSessionPool.at(frontSessionID)->getSocket().close();
					}
					
					queSessionId.pop();
					if (queSessionId.empty())
					{
						acceptor.cancel();
						acceptor.close();
						serverAcceptingState = false;
					}
					break;
				}
				else
				{
					queSessionId.pop();
					queSessionId.push(frontSessionID);
				}
			}
			//add error handleing when not exist sessionID in queue equal to npcSessionID

			//notify all session about setting npc
			netProtocol::PKT_NOTICE_NPC sendPkt;
			sendPkt.init();
			sendPkt.npcId = npcSessionID;

			for (auto i : vecSessionPool)
			{
				if (i->getSocket().is_open())
				{
					i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
				}
			}
		}
		break;
	case netProtocol::pktIdentt::REQ_ORDER: //start gameRoom packet
		{
			//notify turnOrder to all session 
			netProtocol::PKT_REQ_ORDER* pPacket = (netProtocol::PKT_REQ_ORDER*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : REQ_ORDER" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl;
			for (int i = 0; i < netProtocol::maxSessionCnt; i++)
			{
				std::cout << "packet turnOrder[" << i << "] : " << pPacket->turnOrderList[i] << std::endl;
			}
			std::cout
				<< "packet sessionID : " << sessionId << std::endl
				<< "---------------------" << std::endl;

			gameStartedState = true;

			netProtocol::PKT_NOTICE_ORDER sendPkt;
			sendPkt.init();
			memcpy(sendPkt.turnOrderList, pPacket->turnOrderList, sizeof(int)*netProtocol::maxSessionCnt);

			for (auto i : vecSessionPool)
			{
				if (i->getSocket().is_open())
				{
					i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
				}
			}
		}
		break;
	case netProtocol::pktIdentt::REQ_START: //start gameRoom packet
		{
			//notify all session game is start
			netProtocol::PKT_REQ_START* pPacket = (netProtocol::PKT_REQ_START*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : REQ_START" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet sessionID : " << sessionId << std::endl
				<< "---------------------" << std::endl;

			netProtocol::PKT_NOTICE_START sendPkt;
			sendPkt.init();
			
			for (auto i : vecSessionPool)
			{
				if (i->getSocket().is_open())
				{
					i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
				}
			}
		}
		break;
	case netProtocol::pktIdentt::REQ_READY: //start gameRoom packet
		{
			//notify all session game is start
			netProtocol::PKT_REQ_READY* pPacket = (netProtocol::PKT_REQ_READY*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : REQ_READY" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet sessionID : " << sessionId << std::endl
				<< "---------------------" << std::endl;

			netProtocol::PKT_NOTICE_READY sendPkt;
			sendPkt.init();
			sendPkt.readyId = sessionId;

			vecSessionPool[0]->startSend(false, sendPkt.pktSize, (char&)sendPkt);
		}
		break;
	case netProtocol::pktIdentt::HOST_SETROUND:
		{
			netProtocol::PKT_HOST_SETROUND* pPacket = (netProtocol::PKT_HOST_SETROUND*)&pData;

			std::cout
				<< "---------------------" << std::endl
				<< "packet ID : HOST_SETROUND" << std::endl
				<< "packet size : " << pPacket->pktSize << std::endl
				<< "packet sessionID : " << sessionId << std::endl;
			for (int i = 0; i < netProtocol::maxSecretCnt; i++)
			{
				std::cout << "packet secretDeck[" << i << "] : " << pPacket->secretMsList[i] << std::endl;
			}
			for (int i = 0; i < netProtocol::maxPlayerHandCnt; i++)
			{
				std::cout << "packet player1Hand[" << i << "] : " << pPacket->player1MsList[i] << std::endl
					<< "packet player2Hand[" << i << "] : " << pPacket->player2MsList[i] << std::endl
					<< "packet player3Hand[" << i << "] : " << pPacket->player3MsList[i] << std::endl
					<< "packet player4Hand[" << i << "] : " << pPacket->player4MsList[i] << std::endl
					<< "+++++" << std::endl;
			}
			std::cout
				<< "---------------------" << std::endl;

			netProtocol::PKT_NOTICE_SETROUND sendPkt;
			sendPkt.init();
			memcpy_s(sendPkt.secretMsList, sizeof(short)*netProtocol::maxSecretCnt, pPacket->secretMsList, sizeof(short)*netProtocol::maxSecretCnt);
			memcpy_s(sendPkt.player1MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, pPacket->player1MsList, sizeof(short)*netProtocol::maxPlayerHandCnt);
			memcpy_s(sendPkt.player2MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, pPacket->player2MsList, sizeof(short)*netProtocol::maxPlayerHandCnt);
			memcpy_s(sendPkt.player3MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, pPacket->player3MsList, sizeof(short)*netProtocol::maxPlayerHandCnt);
			memcpy_s(sendPkt.player4MsList, sizeof(short)*netProtocol::maxPlayerHandCnt, pPacket->player4MsList, sizeof(short)*netProtocol::maxPlayerHandCnt);

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

		//----------
		netProtocol::PKT_RES_IN resPkt;
		resPkt.init();
		resPkt.mySessionID = pSession->getSessionId();

		netProtocol::PKT_NOTICE_IN noticePkt;
		noticePkt.init();
		noticePkt.joinedUserId = pSession->getSessionId();

		for (int i = 0; i < netProtocol::maxSessionCnt; i++)
		{
			auto tempSession = vecSessionPool[i];
			if (tempSession->getSocket().is_open())
			{
				resPkt.userStateList[i] = true;
				if (tempSession != pSession)
				{
					tempSession->startSend(false, noticePkt.pktSize, (char&)noticePkt);
				}
			}
			else
			{
				resPkt.userStateList[i] = false;
			}
		}

		pSession->startSend(false, resPkt.pktSize, (char&)resPkt);
		//----------

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

void hostServer::noticePlayerOut(const int sessionID)
{
	netProtocol::PKT_NOTICE_OUT sendPkt;
	sendPkt.init();
	sendPkt.disconnectUserId = sessionID;

	for (auto i : vecSessionPool)
	{
		if (i->getSocket().is_open())
		{
			i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
		}
	}
}

/*
void hostServer::noticePlayerCnt()
{
	netProtocol::PKT_RES_CNT sendPkt;
	sendPkt.init();
	int tempCnt = gameMetaData::defaultPlayerCnt - queSessionId.size();
	if (tempCnt < 0) tempCnt = 0;
	sendPkt.playerCnt = tempCnt;

	for (auto i : vecSessionPool)
	{
		if (i->getSocket().is_open())
		{
			i->startSend(false, sendPkt.pktSize, (char&)sendPkt);
		}
	}
}
*/