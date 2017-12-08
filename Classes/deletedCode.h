#pragma once

void seenCheckUpdate();
void stoneObjUpdate();


void gameRoomObjLayer::seenCheckUpdate()
{
	//205,400 / 16*32 / y+40 x+23 / +35	
	Sprite* seenMs;
	for (int msNum = 0; msNum < 8; msNum++)
	{
		//seenChecker 스프라이트들의 좌표계산용
		int defaultX = 205, defaultY = 400, recalibX = 23, recalibY = 40;
		if (msNum > 3)
			defaultX = 205 + recalibX * 3 + 35;

		//seenCnt의 값만큼 seen스프라이트 그림, 그 외에는 notSeen스프라이트
		for (int seenObjNum = 0; seenObjNum < msNum + 1; seenObjNum++)
		{
			if (seenObjNum < seenCnt[msNum])
				seenMs = Sprite::createWithSpriteFrameName(gameMetaData::arrSeenSpriteName[msNum]);
			else
				seenMs = Sprite::createWithSpriteFrameName(gameMetaData::arrNotSeenSpriteName[msNum]);

			seenMs->setPosition(Vec2(defaultX + seenObjNum * recalibX, defaultY + (msNum % 4) * recalibY));
			this->addChild(seenMs);
		}
	}
}

void gameRoomObjLayer::stoneObjUpdate()
{
	//check player's deck then draw magicStone
	int delayRevision = 0;
	for (int i = 0; i < (int)arrPlayers.size(); i++)
	{
		//basic screen size 768
		auto curPlayer = arrPlayers[i];

		int defaultX = 384;
		int defaultY = 0;
		if (i == 0)
		{
			defaultY = 160;
		}
		else if (i == 1)
		{
			defaultX = 700;
			defaultY = 434;
		}
		else if (i == 2)
		{
			defaultY = 700;
		}
		else if (i == 3)
		{
			defaultX = 68;
			defaultY = 434;
		}

		int stoneCnt = curPlayer->getStoneListSize();
		for (int j = 0; j < stoneCnt; j++)
		{
			magicStone* ptempMS;
			ptempMS = curPlayer->getMagicStone(j);

			//set Texture
			//if (i == myPlayerNum) 
			//	ptempMS->setBaseSprite();
			//else 
			ptempMS->initMsSprite();

			int revisionValue = 38 * (stoneCnt - 1 - (j * 2));
			auto resultVec2 = Vec2();

			//setPosition and rotation
			if (i == 1) {//right player
				ptempMS->setRotation(90.0f);
				resultVec2.setPoint(defaultX, defaultY + revisionValue);
			}
			else if (i == 3) {//left player
				ptempMS->setRotation(-90.0f);
				resultVec2.setPoint(defaultX, defaultY + revisionValue);
			}
			else {
				ptempMS->setRotation(0);
				resultVec2.setPoint(defaultX + revisionValue, defaultY);
			}
			auto moveAction0 = MoveTo::create(0.5f, resultVec2);
			auto delayAction0 = DelayTime::create(0.5f + (delayRevision * 0.3));
			auto showAction = Show::create();
			auto actionSeq = Sequence::create(delayAction0, showAction, moveAction0, NULL);
			ptempMS->setVisible(true);
			//ptempMS->setPosition(resultVec2);
			ptempMS->runAction(actionSeq);
			//this->pause();
			//this->pauseSchedulerAndActions();
			delayRevision++;
		}
	}

	//draw secretStones
	int sceretRevisionValue = 0;
	for (auto &i : arrStones)
	{
		if (i->getStatus() != gameMetaData::msStatus::secret)
			continue;

		//230 + 100, 600
		i->setBaseSprite();

		auto moveAction0 = MoveTo::create(0.5f, Vec2(230 + sceretRevisionValue * 100, 600));
		auto delayAction0 = DelayTime::create(0.5f + (delayRevision * 0.3));
		auto showAction = Show::create();
		auto actionSeq = Sequence::create(delayAction0, showAction, moveAction0, NULL);

		i->runAction(actionSeq);

		//i->setPosition(Vec2(230 + sceretRevisionValue * 100, 600));
		//i->setVisible(true);
		sceretRevisionValue++;
	}
	//if a secretStone belong to me, initMsTexture to magic
	auto me = arrPlayers[myPlayerNum];
	int myBooungSize = me->getBooungListSize();
	for (int i = 0; i < myBooungSize; i++)
	{
		auto tempMS = me->getBooungMS(i);
		tempMS->initMsSprite();
	}
}



{
	auto sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprYongyong = Sprite::createWithSpriteFrameName("ms1_yongyong.png");
	if (sprYongyong == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprYongyong is nullptr ");
		std::abort();
	}
	auto btnYongyong = MenuItemImage::create();

	btnYongyong->initWithNormalSprite(
		sprYongyong,
		sprStoneBackward,
		sprYongyong,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::yongyong
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBangrang = Sprite::createWithSpriteFrameName("ms2_bangrang.png");
	if (sprBangrang == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBangrang is nullptr ");
		std::abort();
	}
	auto btnBangrang = MenuItemImage::create();
	btnBangrang->initWithNormalSprite(
		sprBangrang,
		sprStoneBackward,
		sprBangrang,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::bangrang
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprWind = Sprite::createWithSpriteFrameName("ms3_wind.png");
	if (sprWind == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprWind is nullptr ");
		std::abort();
	}
	auto btnWind = MenuItemImage::create();
	btnWind->initWithNormalSprite(
		sprWind,
		sprStoneBackward,
		sprWind,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::wind
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBooung = Sprite::createWithSpriteFrameName("ms4_booung.png");
	if (sprBooung == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBooung is nullptr ");
		std::abort();
	}
	auto btnBooung = MenuItemImage::create();
	btnBooung->initWithNormalSprite(
		sprBooung,
		sprStoneBackward,
		sprBooung,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::booung
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBunpok = Sprite::createWithSpriteFrameName("ms5_bunpok.png");
	if (sprBunpok == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBunpok is nullptr ");
		std::abort();
	}
	auto btnBunpok = MenuItemImage::create();
	btnBunpok->initWithNormalSprite(
		sprBunpok,
		sprStoneBackward,
		sprBunpok,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::bunpok
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprNungang = Sprite::createWithSpriteFrameName("ms6_nungang.png");
	if (sprNungang == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprNungang is nullptr ");
		std::abort();
	}
	auto btnNungang = MenuItemImage::create();
	btnNungang->initWithNormalSprite(
		sprNungang,
		sprStoneBackward,
		sprNungang,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::nungang
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprBuljak = Sprite::createWithSpriteFrameName("ms7_buljack.png");
	if (sprBuljak == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprBuljak is nullptr ");
		std::abort();
	}
	auto btnBuljak = MenuItemImage::create();
	btnBuljak->initWithNormalSprite(
		sprBuljak,
		sprStoneBackward,
		sprBuljak,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::buljak
		)
	);

	sprStoneBackward = Sprite::createWithSpriteFrameName("ms_bg.png");
	auto sprPotion = Sprite::createWithSpriteFrameName("ms8_potion.png");
	if (sprPotion == nullptr)
	{
		log("### err ### gameRoomLayer::initUI::sprPotion is nullptr ");
		std::abort();
	}
	auto btnPotion = MenuItemImage::create();
	btnPotion->initWithNormalSprite(
		sprPotion,
		sprStoneBackward,
		sprPotion,
		CC_CALLBACK_0(
			gameRoomUILayer::checkMagic,
			this,
			(const int)gameMetaData::msType::potion
		)
	);

}