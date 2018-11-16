#pragma once
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\array.hpp>
#include "managers\singleton.h"
#include "managers\networkClass\protocol.h"
#include "managers\networkClass\stream.h"


namespace gameMetaData
{
	enum class gameMode;
}

class networkManager final : public singleton<networkManager>
{
public:
	void init(gameMetaData::gameMode modeFlag);

	std::string getMyAddr();

	//

private:
	void write(outputMemoryStream& stream) const;

	void setMyAddr();
	void setHostAddr();

	std::string myAddr;
	std::string hostAddr;

	boost::asio::io_service ioServ;
	boost::asio::ip::tcp::socket* asioSock;
	boost::asio::ip::tcp::acceptor* asioAcept;
};