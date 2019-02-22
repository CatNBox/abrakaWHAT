#pragma once
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <vector>
#include <queue>

class session;

class hostServer
{
public:
	hostServer(boost::asio::io_service& ioServ);
	~hostServer();
	void initSessionPool(const int sessionMaxCnt);
	void start();
	void closeSession(const int sessionID);
	void processPacket(const int sessionId, const char& pData);

private:
	void startAccept();
	void handleAccept(session* pSession, const boost::system::error_code& errCode);
	bool postAcceptSuccess();

	bool serverAcceptingState;

	//used "new" in initSessionPool. must release memory when close server.
	std::vector<session*> vecSessionPool;
	std::queue<int> queSessionId;

	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::io_service* serverIo;
};