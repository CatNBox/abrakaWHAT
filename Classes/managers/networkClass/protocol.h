#pragma once
#include <stdint.h>

namespace netProtocol
{
	const std::string testIP = "127.0.0.1";
	const unsigned short port = 41638;
	//const uint32_t maxPacketSize = 1024;

	const int maxReceiveBufLen = 512;
	const int maxSessionCnt = 4;

	struct PACKET_HEADER
	{
		short pktId;
		short pktSize;
	};

	enum pktIdentt
	{
		REQ_CNT = 1,
		RES_CNT = 2,
		NOTICE_IN = 3,
		NOTICE_START = 4
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

	struct PKT_REQ_CNT : public PACKET_HEADER
	{
		//char userName[maxNameLen];

		void init()
		{
			pktId = pktIdentt::REQ_CNT;
			pktSize = sizeof(PKT_REQ_CNT);
			//memset(userName, 0, maxNameLen);
		}
	};

	struct PKT_RES_CNT : public PACKET_HEADER
	{
		int playerCnt;

		void init()
		{
			pktId = pktIdentt::RES_CNT;
			pktSize = sizeof(PKT_RES_CNT);
			playerCnt = 0;
		}
	};

	struct PKT_NOTICE_IN : public PACKET_HEADER
	{
		void init()
		{
			pktId = pktIdentt::NOTICE_IN;
			pktSize = sizeof(PKT_NOTICE_IN);
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
}