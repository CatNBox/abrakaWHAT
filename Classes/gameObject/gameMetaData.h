#pragma once

#define SINGLE_MODE_BUILD 1
#define NETWORK_MODE_BUILD 2
#define RELEASE_MODE_BUILD 0
#define stdAxis 150.0f

namespace gameMetaData
{
	const std::string arrMsSpriteName[] = {
		"ms_bg.png",
		"ms1_yongyong.png",
		"ms2_bangrang.png",
		"ms3_wind.png",
		"ms4_booung.png",
		"ms5_bunpok.png",
		"ms6_nungang.png",
		"ms7_buljack.png",
		"ms8_postion.png"
	}; 
	const std::string arrNotSeenSpriteName[] = {
		"ms1_notSeen.png",
		"ms2_notSeen.png",
		"ms3_notSeen.png",
		"ms4_notSeen.png",
		"ms5_notSeen.png",
		"ms6_notSeen.png",
		"ms7_notSeen.png",
		"ms8_notSeen.png"
	};
	const std::string arrSeenSpriteName[] = {
		"ms1_seen.png",
		"ms2_seen.png",
		"ms3_seen.png",
		"ms4_seen.png",
		"ms5_seen.png",
		"ms6_seen.png",
		"ms7_seen.png",
		"ms8_seen.png"
	};

	enum msType
	{
		base = 0,	//0
		yongyong,	//1
		bangrang,	//2
		wind,		//3
		booung,		//4
		bunpok,		//5
		nungang,	//6
		buljak,		//7
		postion,	//8
		pass		//9
	};

	enum msStatus
	{
		notUse,
		owned,
		secret,
		discard,
		active
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