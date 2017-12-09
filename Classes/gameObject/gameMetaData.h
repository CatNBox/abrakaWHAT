#pragma once

#define SINGLE_MODE_BUILD 1
#define NETWORK_MODE_BUILD 2
#define RELEASE_MODE_BUILD 0
#define stdAxis 384.0f
#define secretScale 0.5f

namespace gameMetaData
{
	//0 = ms_bg, 1~8 = ms1~8
	const std::string arrMsSpriteName[] = {
		"ms_bg.png",
		"ms1_yongyong.png",
		"ms2_bangrang.png",
		"ms3_wind.png",
		"ms4_booung.png",
		"ms5_bunpok.png",
		"ms6_nungang.png",
		"ms7_buljack.png",
		"ms8_potion.png"
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
	const std::string arrLpTokenName[] = {
		"lpToken00.png",
		"lpToken01.png",
		"lpToken02.png",
		"lpToken03.png",
		"lpToken04.png",
		"lpToken05.png"
	};

	//----using Enum sfxName
	const std::vector<const char*> arrSfxPath = {
		"soundResource/msMoving00",
		"soundResource/buljak00"
	};

	//----tempNpcSound
	const std::vector<const char*> arrNpcSound = {
		"soundResource/tempSound/byTheLight.ogg"
		"soundResource/tempSound/goodgame.ogg"
		"soundResource/tempSound/greetings.ogg"
		"soundResource/tempSound/theLightShallBringVictory.ogg"
		"soundResource/tempSound/theLightShallBurnYou.ogg"
	};

	enum sfxName
	{
		msMoving00,
		buljak00
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
		potion,		//8
		pass		//9
	};

	enum layerZOrder
	{
		backGroundZ,
		objZ0,			//----secretStone Zorder
		objZ1,			//----LP Zorder
		objZ2,
		effectZ			//----Effect Zorder
	};

	enum msStatus
	{
		notUse,
		owned,
		secret,
		discard
	};

	enum msActionState
	{
		msWait,
		msMove,
		msDiscard,
		msActive
	};

	enum curScene
	{
		setupScene = 0,
		mainMenuScene = 1,
		gameRoom
	};

	enum npcState
	{
		npcWait,
		npcTurnOn
	};
}