#pragma once
#include "cocos2d.h"

class popupLayer : public cocos2d::Layer
{
public:
	popupLayer();
	~popupLayer();
	virtual bool init();

	CREATE_FUNC(popupLayer);

private:
};

