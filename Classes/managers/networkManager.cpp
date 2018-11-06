#include "managers\networkManager.h"
#include "gameObject\gameMetaData.h"

void networkManager::init(gameMetaData::gameMode modeFlag)
{
	if (modeFlag == gameMetaData::gameMode::single)
	{
		//nothing
	}
	else
	{
		asioSock = new boost::asio::ip::tcp::socket(ioServ);
	}


	if (modeFlag == gameMetaData::gameMode::host)
	{
		//server on
	}
	else if (modeFlag == gameMetaData::gameMode::guest)
	{
		//client on
	}
}

void networkManager::write(outputMemoryStream & stream) const
{
	//전송할 데이터
}
