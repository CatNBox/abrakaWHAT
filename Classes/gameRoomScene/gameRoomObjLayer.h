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
	void settingEventListener();
	void settingCntValues();
	void createPlayers();
	void createMagicStones();

	void selSecretStone();
	void shareStone2Player();

	void layerUpdate(float d);
	void dataUpdate();
	void seenCheckUpdate();
	void stoneObjUpdate();
	void curLPUpdate();
	void checkOwnedMagic(cocos2d::EventCustom* checkOwnedMagicEvent);
	void activeMagic(magicStone* activeStone);
	void passTurn();

	cocos2d::Sprite* getMSSprite(const int magicEnum);
	void setStartOrder();

	magicStone* pickAStone(const int stateEnum);
	bool isAllUsed() const; //check to discard all

	int getRandomIndex();
	int getMsPosRevision(int msListSize, int msOrder);

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

	int seenCnt[8];

	int stoneMinCnt = 0;
	int stoneMaxCnt;

	int starterNum;
	int curPlayerNum;
	int myPlayerNum = 0;

	bool isChanged = true;

	std::vector<magicStone*> arrStones;
	std::vector<player*> arrPlayers;

	cocos2d::EventListenerCustom* uiListener;

	void checkArrStones(); //for debug
	void checkArrPlayers(); //for debug

	//��Ƽ������ ���������� ���� �� ��ο� ����
	//Ŭ�󿡼��� �������� ����ؾ��� ������ ����
};