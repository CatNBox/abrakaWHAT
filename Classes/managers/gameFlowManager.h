#pragma once
#include "cocos2d.h"
#include "managers\soundManager.h"

#define CURRENT_BUILD_MODE SINGLE_MODE_BUILD	//static variable

class networkManager;

class gameFlowManager
{
public:
	static gameFlowManager& getInstance();

	void changeScene2MainMenu();	//setupScene
	void changeScene2SingleMode();	//mainMenuScene
	void changeScene2HostMode();	//mainMenuScene
	void changeScene2JoinMode();	//mainMenuScene

	cocos2d::Rect getNumSprRect(const int num); //0-9 only	//numSpriteClass

	//---------------------------------------------
	void setPlayerCount(int cnt);
	int getPlayerCount() const;

	//for gameOption
	void setYongyongCount(int cnt);
	int getYongyongCount() const;
	void setBangrangCount(int cnt);
	int getBangrangCount() const;
	void setWindCount(int cnt);
	int getWindCount() const;
	void setBooungCount(int cnt);
	int getBooungCount() const;
	void setBunpokCount(int cnt);
	int getBunpokCount() const;
	void setNungangCount(int cnt);
	int getNungangCount() const;
	void setBuljakCount(int cnt);
	int getBuljakCount() const;
	void setPotionCount(int cnt);
	int getPotionCount() const;
	void setSecretCount(int cnt);
	int getSecretCount() const;
	void setLifePoint(int cnt);
	int getMaxLifePoint() const;
	//----------------option File R/W create----------

	int getRandomInt(int min, int max);	//randomGenClass
	soundManager* getSoundManager();	//delete

	void endGame();	//case by case

	void incRunningActionCnt();	//actionManagerClass
	void decRunningActionCnt();	//actionManagerClass
	int getRunningActionCnt();	//actionManagerClass

	cocos2d::Sequence* wrapActions(cocos2d::FiniteTimeAction* action01, ...);	//actionManagerClass


private:
	gameFlowManager();
	virtual ~gameFlowManager();

	int playerCount;
	int yongyongCnt;
	int bangrangCnt;
	int windCnt;
	int booungCnt;
	int bunpokCnt;
	int nungangCnt;
	int buljakCnt;
	int potionCnt;
	int secretCnt;
	int maxLifePoint = 0;

	int runningActionCnt = 0;

	int curSceneState;

	soundManager* objSoundManager;
	networkManager* objNetworkManager;
};