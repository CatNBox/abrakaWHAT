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
	//�ڱ��ڽ��� �׻� 1�� �÷��̾� arrPlayer[0]
	std::vector<magicStone*> arrStones;
	std::vector<player*> arrPlayers;

	cocos2d::EventListenerCustom* uiListener;

	void checkArrStones(); //for debug
	void checkArrPlayers(); //for debug

	//��Ƽ������ ���������� ���� �� ��ο� ����
	//Ŭ�󿡼��� �������� ����ؾ��� ������ ����
};