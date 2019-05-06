#pragma once
#include <stdint.h>

namespace netProtocol
{
	const std::string testIP = "127.0.0.1";
	const unsigned short port = 41638;
	//const uint32_t maxPacketSize = 1024;

	const int maxReceiveBufLen = 512;
	const int maxSessionCnt = 4; // equal gameMetaData::defaultPlayerCnt
	const int maxSecretCnt = 4;
	const int maxPlayerHandCnt = 5;

	struct PACKET_HEADER
	{
		short pktId;
		short pktSize;
	};

	enum pktIdentt
	{
		REQ_NPC = 3,
		REQ_ORDER = 4,
		REQ_START = 5,
		REQ_READY = 6,
		RES_IN = 10,
		NOTICE_IN = 11,
		NOTICE_OUT = 12,
		NOTICE_NPC = 13,
		NOTICE_ORDER = 14,
		NOTICE_START = 15,
		NOTICE_READY = 16,
		NOTICE_SETROUND = 17,
		HOST_SETROUND = 30
	};

	/*
	필요한패킷종류
	*playerCnt 요청패킷
	*playerCnt 답장패킷
	*다른플레이어접속알림패킷
	*게임스타트알림패킷
	게임입력패킷 - 답신올때까지 입력정지
	게임입력결과처리패킷 - 접속한 모든 클라이언트에 notice
	 + 라운드중 입력결과처리
	 + 라운드종료 결과처리
	 + 게임종료 결과처리 - 호스트 해제됨
	*/

	struct PKT_RES_IN : public PACKET_HEADER
	{
		int mySessionID;
		bool userStateList[maxSessionCnt]; //session No

		void init()
		{
			pktId = pktIdentt::RES_IN;
			pktSize = sizeof(PKT_RES_IN);
			mySessionID = -1;
			for (int i = 0; i < maxSessionCnt; i++)
			{
				userStateList[i] = false;
			}
		}
	};

	struct PKT_REQ_NPC : public PACKET_HEADER
	{
		int npcSessionID;

		void init()
		{
			pktId = pktIdentt::REQ_NPC;
			pktSize = sizeof(PKT_REQ_NPC);
			npcSessionID = -1;
		}
	};

	struct PKT_REQ_ORDER : public PACKET_HEADER
	{
		int turnOrderList[maxSessionCnt];

		void init()
		{
			pktId = pktIdentt::REQ_ORDER;
			pktSize = sizeof(PKT_REQ_ORDER);
			for (int i = 0; i < maxSessionCnt; i++)
			{
				turnOrderList[i] = -1;
			}
		}
	};

	struct PKT_REQ_START : public PACKET_HEADER
	{
		void init()
		{
			pktId = pktIdentt::REQ_START;
			pktSize = sizeof(PKT_REQ_START);
		}
	};

	struct PKT_REQ_READY : public PACKET_HEADER
	{
		void init()
		{
			pktId = pktIdentt::REQ_READY;
			pktSize = sizeof(PKT_REQ_READY);
		}
	};

	struct PKT_HOST_SETROUND : public PACKET_HEADER
	{
		short secretMsList[maxSecretCnt];
		short player1MsList[maxPlayerHandCnt];
		short player2MsList[maxPlayerHandCnt];
		short player3MsList[maxPlayerHandCnt];
		short player4MsList[maxPlayerHandCnt];

		void init()
		{
			pktId = pktIdentt::HOST_SETROUND;
			pktSize = sizeof(PKT_HOST_SETROUND);
			for (int i = 0; i < maxSecretCnt; i++)
			{
				secretMsList[i] = -1;
			}
			for (int i = 0; i < maxPlayerHandCnt; i++)
			{
				player1MsList[i] = -1;
				player2MsList[i] = -1;
				player3MsList[i] = -1;
				player4MsList[i] = -1;
			}
		}
	};

	struct PKT_NOTICE_IN : public PACKET_HEADER
	{
		int joinedUserId;

		void init()
		{
			pktId = pktIdentt::NOTICE_IN;
			pktSize = sizeof(PKT_NOTICE_IN);
			joinedUserId = -1;
		}
	};

	struct PKT_NOTICE_OUT : public PACKET_HEADER
	{
		int disconnectUserId;

		void init()
		{
			pktId = pktIdentt::NOTICE_OUT;
			pktSize = sizeof(PKT_NOTICE_OUT);
			disconnectUserId = -1;
		}
	};

	struct PKT_NOTICE_NPC : public PACKET_HEADER
	{
		int npcId;

		void init()
		{
			pktId = pktIdentt::NOTICE_NPC;
			pktSize = sizeof(PKT_NOTICE_NPC);
			npcId = -1;
		}
	};

	struct PKT_NOTICE_ORDER : public PACKET_HEADER
	{
		int turnOrderList[maxSessionCnt];

		void init()
		{
			pktId = pktIdentt::NOTICE_ORDER;
			pktSize = sizeof(PKT_NOTICE_ORDER);
			for (int i = 0; i < maxSessionCnt; i++)
			{
				turnOrderList[i] = -1;
			}
		}
	};

	struct PKT_NOTICE_START : public PACKET_HEADER
	{
		void init()
		{
			pktId = pktIdentt::NOTICE_START;
			pktSize = sizeof(PKT_NOTICE_START);
		}
	};

	struct PKT_NOTICE_READY : public PACKET_HEADER
	{
		int readyId;

		void init()
		{
			pktId = pktIdentt::NOTICE_READY;
			pktSize = sizeof(PKT_NOTICE_READY);
			readyId = -1;
		}
	};

	struct PKT_NOTICE_SETROUND : public PACKET_HEADER
	{
		short secretMsList[maxSecretCnt];
		short player1MsList[maxPlayerHandCnt];
		short player2MsList[maxPlayerHandCnt];
		short player3MsList[maxPlayerHandCnt];
		short player4MsList[maxPlayerHandCnt];

		void init()
		{
			pktId = pktIdentt::NOTICE_SETROUND;
			pktSize = sizeof(PKT_NOTICE_SETROUND);
			for (int i = 0; i < maxSecretCnt; i++)
			{
				secretMsList[i] = -1;
			}
			for (int i = 0; i < maxPlayerHandCnt; i++)
			{
				player1MsList[i] = -1;
				player2MsList[i] = -1;
				player3MsList[i] = -1;
				player4MsList[i] = -1;
			}
		}
	};
}