#pragma once

namespace cocos2d
{
	class Rect;
}

class spriteManager
{
public:
	cocos2d::Rect getNumSprRect(const int num);
	void preLoadingSprites();
private:
};