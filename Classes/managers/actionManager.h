#pragma once
#include "cocos2d.h"

class actionManager 
{
public:
	const int getRunningActionCnt() const;
	//auto wrap action between inc/decRunningActionCnt
	cocos2d::Sequence* wrapActions4Cnt(cocos2d::FiniteTimeAction* action01, ...);

	~actionManager();

private:
	actionManager();

	void incRunningActionCnt();
	void decRunningActionCnt();

	int runningActionCnt;
};