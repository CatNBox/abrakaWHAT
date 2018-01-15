#include "setupScene.h"
#include "setupScene\setupLoadingLayer.h"

using namespace cocos2d;

Scene* setupScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = setupScene::create();
    
    // 'layer' is an autorelease object
    auto layer = setupLodingLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool setupScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    return true;
}
