#pragma once
#include "cocos2d.h"
#include "managers\singleton.h"

class actionManager final : public singleton<actionManager>
{
public:
	//get current running action count
	const int getRunningActionCnt() const;
	void initCnt();

	//auto wrap action between inc/decRunningActionCnt
	cocos2d::Sequence* wrapActions4Cnt(cocos2d::FiniteTimeAction* action01, ...);

private:
	void incRunningActionCnt();
	void decRunningActionCnt();

	int runningActionCnt = 0;
};