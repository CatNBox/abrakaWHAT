#pragma once
#include "cocos2d.h"

class actionManager;
class soundManager;
namespace gameMetaData
{
	enum msState;
	enum msType;
	enum class msMovement;
}

/*----------------------------------------


magicStone


-----------------------------------------*/
class magicStone : public cocos2d::Sprite
{
public:
	int getMagic();
	int getStatus();
	void setState(const gameMetaData::msState newStatus);
	void initObjData();

	bool initMsSprite();	//magic에 맞는 스프라이트로 갱신
	bool setBaseSprite();	//뒷모습 스프라이트로 갱신

	void actionMove(const float priorDelay, const cocos2d::Vec2 targetPos, const gameMetaData::msMovement movementEnum);
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
	gameMetaData::msState status;
		
	float curX;
	float curY;

	float tX;
	float tY;
};
