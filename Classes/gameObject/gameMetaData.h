#pragma once

#define SINGLE_MODE_BUILD 1
#define NETWORK_MODE_BUILD 2
#define RELEASE_MODE_BUILD 0

namespace gameMetaData
{
	enum magicStones
	{
		base = 0,
		yongyong = 1,
		bangrang,
		wind,
		booung,
		bunpok,
		nungang,
		buljak,
		postion,
		pass
	};

	enum stoneState
	{
		notUse,
		owned,
		secret,
		ownedSecret,
		discard,
		active,
		mine
	};

	enum curScene
	{
		setupScene = 0,
		mainMenuScene = 1,
		gameRoom
	};

	enum npcState
	{
		wait,
		turnOn
	};
}