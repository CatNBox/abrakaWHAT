#pragma once
#include "cocos2d.h"
#include "gameObject\gameMetaData.h"

#define CURRENT_BUILD_MODE SINGLE_MODE_BUILD

class networkManager;
class soundManager;

class gameFlowManager:public cocos2d::Object
{
public:
	static gameFlowManager* getInstance();
	
	gameFlowManager();

	void changeScene2MainMenu();
	void changeScene2SingleMode();
	void changeScene2HostMode();
	void changeScene2JoinMode();

	void preloadSprites();

	void setPlayerCount(int cnt);
	int getPlayerCount() const;

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
	void setPostionCount(int cnt);
	int getPostionCount() const;
	void setSecretCount(int cnt);
	int getSecretCount() const;

	void endGame();

private:
	//gameFlowManager();
	//SpriteFrameCache* spriteCache;

	const char* MY_IP;

	int playerCount;
	int yongyongCnt;
	int bangrangCnt;
	int windCnt;
	int booungCnt;
	int bunpokCnt;
	int nungangCnt;
	int buljakCnt;
	int postionCnt;
	int secretCnt;

	int curSceneState;

	soundManager* objSoundManager;
	networkManager* objNetworkManager;
};