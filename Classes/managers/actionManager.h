#pragma once
#include "cocos2d.h"

class actionManager 
{
public:
	//singleton
	static actionManager *getInstance();

	//get current running action count
	const int getRunningActionCnt() const;

	//auto wrap action between inc/decRunningActionCnt
	cocos2d::Sequence* wrapActions4Cnt(cocos2d::FiniteTimeAction* action01, ...);

	//release
	void releaseInstance();
private:
	~actionManager();
	actionManager() = default;
	actionManager(const actionManager&) = delete;
	actionManager &operator=(const actionManager&) = delete;

	static std::unique_ptr<actionManager> instance;

	void incRunningActionCnt();
	void decRunningActionCnt();

	int runningActionCnt;
};