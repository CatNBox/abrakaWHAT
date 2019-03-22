#include "spriteManager.h"
#include <cocos2d.h>

cocos2d::Rect spriteManager::getNumSprRect(const int num)
{
	cocos2d::Rect tempRect;
	switch (num)
	{
	case 1:
		tempRect = cocos2d::Rect(5, 8, 35, 75);
		return tempRect;
	case 2:
		tempRect = cocos2d::Rect(52, 5, 47, 81);
		return tempRect;
	case 3:
		tempRect = cocos2d::Rect(100, 5, 45, 81);
		return tempRect;
	case 4:
		tempRect = cocos2d::Rect(147, 7, 46, 81);
		return tempRect;
	case 5:
		tempRect = cocos2d::Rect(194, 5, 45, 81);
		return tempRect;
	case 6:
		tempRect = cocos2d::Rect(242, 5, 43, 81);
		return tempRect;
	case 7:
		tempRect = cocos2d::Rect(289, 5, 45, 81);
		return tempRect;
	case 8:
		tempRect = cocos2d::Rect(336, 5, 45, 81);
		return tempRect;
	case 9:
		tempRect = cocos2d::Rect(381, 5, 47, 81);
		return tempRect;
	default:
		tempRect = cocos2d::Rect(428, 5, 46, 81);
		return tempRect;
	}
	return tempRect;
}

void spriteManager::preLoadingSprites()
{
	auto spriteCache = cocos2d::SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("magicStones.plist", "magicStones.png");
	spriteCache->addSpriteFramesWithFile("seenChecker.plist", "seenChecker.png");

	auto tempSpr = cocos2d::Sprite::create("UISprite/spr_number.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "spr_number.png");

	tempSpr->initWithFile("lpToken/lpToken00.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken00.png");
	tempSpr->initWithFile("lpToken/lpToken01.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken01.png");
	tempSpr->initWithFile("lpToken/lpToken02.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken02.png");
	tempSpr->initWithFile("lpToken/lpToken03.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken03.png");
	tempSpr->initWithFile("lpToken/lpToken04.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken04.png");
	tempSpr->initWithFile("lpToken/lpToken05.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "lpToken05.png");

	tempSpr->initWithFile("msEffectSpr/yongyongRedScreen.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "yongyongRedScreen.png");

	tempSpr->initWithFile("UISprite/sprScore.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprScore.png");
	tempSpr->initWithFile("UISprite/sprPlayer.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprPlayer.png");

	tempSpr->initWithFile("popupLayerSpr/popupLayerBG.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popupLayerBG.png");
	tempSpr->initWithFile("popupLayerSpr/popup400500.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popup400500.png");
	tempSpr->initWithFile("popupLayerSpr/popup400245.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "popup400245.png");
	tempSpr->initWithFile("UISprite/uiPopMessage.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "uiPopMessage.png");

	tempSpr->initWithFile("UISprite/sprCancel.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprCancel.png");
	tempSpr->initWithFile("UISprite/sprOk.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprOk.png");
	tempSpr->initWithFile("UISprite/sprLive.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprLive.png");
	tempSpr->initWithFile("UISprite/sprWin.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprWin.png");
	tempSpr->initWithFile("UISprite/sprBooung.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprBooung.png");
	tempSpr->initWithFile("UISprite/sprDot.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprDot.png");
	tempSpr->initWithFile("UISprite/sprUpArrow.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprUpArrow.png");
	tempSpr->initWithFile("UISprite/btnCopyFromClip.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "btnCopyFromClip.png");
	tempSpr->initWithFile("UISprite/btnCopyToClip.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "btnCopyToClip.png");

	tempSpr->initWithFile("UISprite/btnHostNormal.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "btnHostNormal.png");
	tempSpr->initWithFile("UISprite/btnJoinNormal.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "btnJoinNormal.png");
	
	tempSpr->initWithFile("UISprite/sprError.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprError.png");

	tempSpr->initWithFile("UISprite/sprNPC.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprNPC.png");

	tempSpr->initWithFile("UISprite/sprRound.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprRound.png");

	tempSpr->initWithFile("UISprite/sprRank.png");
	spriteCache->addSpriteFrame(tempSpr->getSpriteFrame(), "sprRank.png");
}
