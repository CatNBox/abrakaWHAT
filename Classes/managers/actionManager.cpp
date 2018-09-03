#include "actionManager.h"

void actionManager::incRunningActionCnt()
{
	runningActionCnt++;
}

void actionManager::decRunningActionCnt()
{
	runningActionCnt--;
}

const int actionManager::getRunningActionCnt() const
{
	return runningActionCnt;
}

cocos2d::Sequence * actionManager::wrapActions4Cnt(cocos2d::FiniteTimeAction * action01, ...)
{
	va_list ap;
	va_start(ap, action01);

	cocos2d::FiniteTimeAction * now = action01;
	cocos2d::Vector<cocos2d::FiniteTimeAction*> vecAction;
	auto callNotifyActionStart = cocos2d::CallFunc::create([=]() {incRunningActionCnt(); });
	vecAction.pushBack(callNotifyActionStart);
	while (action01)
	{
		vecAction.pushBack(now);
		now = va_arg(ap, cocos2d::FiniteTimeAction*);
		if (now == NULL)
			break;
	}
	auto callNotifyActionEnd = cocos2d::CallFunc::create([=]() {decRunningActionCnt(); });
	vecAction.pushBack(callNotifyActionEnd);

	va_end(ap);

	return cocos2d::Sequence::create(vecAction);
}
