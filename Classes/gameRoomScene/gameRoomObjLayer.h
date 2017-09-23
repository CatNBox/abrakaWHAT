#pragma once
#include "cocos2d.h"
#include "Classes\gameObject\magicStone.h"
#include "Classes\gameObject\player.h"

class gameRoomObjLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	void initRound();

	CREATE_FUNC(gameRoomObjLayer);
private:
	void screenUpdate(float d);

	cocos2d::Sprite* getMSSprite(const int magicEnum);
	void setStartOrder();

	magicStone* pickAStone(const int stateEnum);
	bool isAllUsed() const; //check to discard all

	int getRandomIndex();

	int playerCnt;
	int yongyongCnt;
	int bangrangCnt;
	int windCnt;
	int booungCnt;
	int bunpokCnt;
	int nungangCnt;
	int buljakCnt;
	int postionCnt;
	int secretCnt;

	int stoneMinCnt = 0;
	int stoneMaxCnt;

	int starter;
	//자기자신이 항상 1번 플레이어 arrPlayer[0]
	std::vector<magicStone*> arrStones;
	std::vector<player*> arrPlayers;

	cocos2d::EventListenerCustom* uiListener;

	void checkArrStones(); //for debug
	void checkArrPlayers(); //for debug

	//멀티에서는 서버에서만 세팅 및 드로우 동작
	//클라에서는 서버에서 출력해야할 정보만 얻어옴
};