#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include "managers\gameFlowManager.h"

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
	gameFlowManager::getInstance();
    return Application::getInstance()->run();
}
