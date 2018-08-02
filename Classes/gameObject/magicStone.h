#pragma once
#include "cocos2d.h"

class actionManager;
class soundManager;
namespace gameMetaData
{
	enum msStatus;
	enum msType;
}

/*----------------------------------------


magicStone


-----------------------------------------*/
class magicStone : public cocos2d::Sprite
{
public:
	int getMagic();
	int getStatus();
	void setStatus(const gameMetaData::msStatus newStatus);
	void initObjData();

	bool initMsSprite();	//magic�� �´� ��������Ʈ�� ����
	bool setBaseSprite();	//�޸�� ��������Ʈ�� ����

	void actionMove(const float priorDelay, const cocos2d::Vec2 targetPos, const int movementEnum);
	void actionActivated();
	void actionRevealedSecret();

	magicStone();
	magicStone(const gameMetaData::msType msType);
	~magicStone();
	magicStone* clone();

private:
	actionManager* actManager;
	soundManager* sndManager;

	gameMetaData::msType magic;
	gameMetaData::msStatus status;
		
	float curX;
	float curY;

	float tX;
	float tY;
};
