#include "popupLayer.h"
#include "gameObject\gameMetaData.h"
#include "managers\gameFlowManager.h"

popupLayer::popupLayer()
{
}

popupLayer::~popupLayer()
{
}

bool popupLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}
