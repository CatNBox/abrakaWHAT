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
	�ʿ�����Ŷ����
	*playerCnt ��û��Ŷ
	*playerCnt ������Ŷ
	*�ٸ��÷��̾����Ӿ˸���Ŷ
	*���ӽ�ŸƮ�˸���Ŷ
	�����Է���Ŷ - ��ſö����� �Է�����
	�����Է°��ó����Ŷ - ������ ��� Ŭ���̾�Ʈ�� notice
	 + ������ �Է°��ó��
	 + �������� ���ó��
	 + �������� ���ó�� - ȣ��Ʈ ������
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