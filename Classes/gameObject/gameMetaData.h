#pragma once
#include <string>
#include <vector>

#define SINGLE_MODE_BUILD 1
#define NETWORK_MODE_BUILD 2
#define RELEASE_MODE_BUILD 0
#define STDAXIS 384.0f
#define SECRETSTONE_BASESCALE 0.5f
#define SCORE_BASESCALE 0.5f

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
		"ms_bg.png",
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
		"ms_bg.png",
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
		"soundResource/msMoving00.wav",
		"soundResource/buljak00.wav",
		"soundResource/activateMagic00.wav"
	};

	//----tempNpcSound
	const std::vector<const char*> arrNpcSound = {
		"soundResource/tempSound/anduinEng/byTheLight.wav",
		"soundResource/tempSound/anduinEng/goodgame.wav",
		"soundResource/tempSound/anduinEng/greetings.wav",
		"soundResource/tempSound/anduinEng/theLightShallBringVictory.wav",
		"soundResource/tempSound/anduinEng/theLightShallBurnYou.wav",
		"soundResource/tempSound/vampyJp/doshiyokana.wav",
		"soundResource/tempSound/vampyJp/etoneetone.wav",
		"soundResource/tempSound/vampyJp/ikeike.wav",
		"soundResource/tempSound/vampyJp/sorejahajimeyoka.wav",
		"soundResource/tempSound/vampyJp/vampychansaikyodesushi.wav",
		"soundResource/tempSound/vampyJp/vampychansugoindakara.wav",
		"soundResource/tempSound/arisaEng/theForestIsAngry.wav",
		"soundResource/tempSound/arisaEng/thisEndHere.wav",
		"soundResource/tempSound/arisaEng/youAreDoneFall.wav",
		"soundResource/tempSound/arisaEng/whatShouldIdo.wav",
		"soundResource/tempSound/arisaEng/wowYouAreStong.wav"
	};

	const std::vector<const char*> arrNpcGreeting = {
		"soundResource/tempSound/vampyJp/sorejahajimeyoka.wav",
		"soundResource/tempSound/anduinKor/anduinKorStart.wav",
		"soundResource/tempSound/anduinEng/greetings.wav",
		"soundResource/tempSound/arisaEng/theForestIsAngry.wav"
	};

	const std::vector<const char*> arrNpcThreaten = {
		"soundResource/tempSound/vampyJp/vampychansaikyodesushi.wav",
		"soundResource/tempSound/anduinKor/anduinKorThreaten.wav",
		"soundResource/tempSound/anduinEng/theLightShallBurnYou.wav",
		"soundResource/tempSound/arisaEng/youAreDoneFall.wav"
	};

	const std::vector<const char*> arrBGSprite = {
		"bg01.jpg",
		"bg02.jpg",
		"bg03.jpg",
		"bg04.jpg"
	};

	enum npcCharacter
	{
		vampyJp,
		anduinKor,
		anduinEng,
		arisaEng
	};

	enum npcEmotion
	{
		greeting,
		threaten
	};

	enum variableMaxCnt
	{
		msTypeCnt = 9	//array use index 1 to 8
	};

	enum sfxName
	{
		msMoving00,
		buljak00,
		activateMagic00
	};

	enum msType
	{
		base = 0,	//= 0
		yongyong,	//= 1
		bangrang,	//= 2
		wind,		//= 3
		booung,		//= 4
		bunpok,		//= 5
		nungang,	//= 6
		buljak,		//= 7
		potion,		//= 8
		pass		//= 9
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

	enum msMovement
	{
		sharing,
		selected,
		reordering
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