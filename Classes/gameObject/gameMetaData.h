#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <random>

#define SINGLE_MODE_BUILD 1
#define NETWORK_MODE_BUILD 2
#define RELEASE_MODE_BUILD 0
#define STDAXIS 384.0f
#define SECRETSTONE_BASESCALE 0.5f
#define SCORE_BASESCALE 0.5f

#define CURRENT_BUILD_MODE NETWORK_MODE_BUILD

//-------------------
//header for manage
//enum and const, inline function
//
//	 ONLY CONST VALUE
//
//-------------------

namespace inlineFunc 
{
	inline int getRandomInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937_64 rnd(rd());

		std::uniform_int_distribution<int> range(min, max);

		return range(rnd);
	}
}

namespace gameMetaData
{
	//option.ini defaultValue
	const int defaultPlayerCnt = 4;
	const int defaultSecretCnt = 4;
	const int defaultYongCnt = 1;
	const int defaultBangrangCnt = 2;
	const int defaultWindCnt = 3;
	const int defaultBooungCnt = 4;
	const int defaultBunpokCnt = 5;
	const int defaultNungangCnt = 6;
	const int defaultBuljakCnt = 7;
	const int defaultPotionCnt = 8;
	const int defaultMaxLifePoint = 6;

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

	enum class popupBoardSize
	{
		popup400245,
		popup400500,
		unknown
	};

	const std::unordered_map<popupBoardSize, const char*> mapPopupBoardSpr =
	{
		{ popupBoardSize::popup400245, "popup400245.png" },
		{ popupBoardSize::popup400500, "popup400500.png" }
	};

	enum class npcCharacter
	{
		vampyJp,
		anduinKor,
		anduinEng,
		arisaEng,
		unknown
	};

	enum class npcEmotion
	{
		greeting,
		threaten,
		unknown
	};

	enum variableMaxCnt
	{
		msTypeCnt = 9	//array use index 1 to 8
	};

	enum sfxName
	{
		msMoving00,
		buljak00,
		activateMagic00,
		unknownSfx
	};
	
	enum class warningCode
	{
		infinityLoopWarning,
		exitBtnWarning,
		unknown
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
		effectZ		//----Effect Zorder
	};

	enum msStatus
	{
		notUse,
		owned,
		secret,
		discard,
		unknownStatus
	};

	enum class msMovement
	{
		sharing,
		selected,
		reordering,
		unknownMovement
	};

	enum class msActionState
	{
		msWait,
		msMove,
		msDiscard,
		msActive,
		unknown
	};

	enum class curScene
	{
		setupScene = 0,
		mainMenuScene = 1,
		gameRoom,
		unknown
	};

	enum class gameMode
	{
		single,
		host,
		guest,
		unknown
	};

	enum class npcState
	{
		npcWait,
		npcTurnOn,
		unknown
	};
}