#include "setupScene.h"
#include "setupScene\setupLoadingLayer.h"
//#include "SimpleAudioEngine.h"

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

// on "init" you need to initialize your instance
bool setupScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    return true;
}
