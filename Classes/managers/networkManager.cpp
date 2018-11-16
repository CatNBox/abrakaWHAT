#include "managers\networkManager.h"
#include "gameObject\gameMetaData.h"
#include <boost\system\error_code.hpp>
#include <iostream>

using namespace boost;

void networkManager::init(gameMetaData::gameMode modeFlag)
{
	if (modeFlag == gameMetaData::gameMode::single)
	{
		//nothing
	}
	else
	{
		
		unsigned short port = 41638;
		asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port);
		boost::system::error_code ec;
		asioSock = new boost::asio::ip::tcp::socket(ioServ);

		std::cout << "nm : " << asio::ip::host_name() << std::endl;
		setMyAddr();
		/*
		if (modeFlag == gameMetaData::gameMode::host)
		{
			//server on
			asioAcept = new asio::ip::tcp::acceptor(ioServ, ep.protocol());
			asioAcept->bind(ep, ec);
			if (ec.value())
			{

			}
			asioAcept->accept(*asioSock);
			setMyAddr();
		}
		else if (modeFlag == gameMetaData::gameMode::guest)
		{
			//client on
			asioSock->open(ep.protocol(), ec);
			if (ec.value())
			{

			}
		}
		*/
	}
}

std::string networkManager::getMyAddr()
{
	return myAddr;
}

void networkManager::write(outputMemoryStream & stream) const
{
	//전송할 데이터
}

void networkManager::setMyAddr()
{
	asio::ip::tcp::resolver resovler(ioServ);
	asio::ip::tcp::resolver::query query(asio::ip::host_name(), "");
	asio::ip::tcp::resolver::iterator it = resovler.resolve(query);

	while (it != asio::ip::tcp::resolver::iterator())
	{
		asio::ip::address addr = (it++)->endpoint().address();
		if (addr.is_v4())
		{
			std::cout << addr.to_string() << std::endl;
			myAddr = addr.to_string();
			break;
		}
	}
}

void networkManager::setHostAddr()
{
}
