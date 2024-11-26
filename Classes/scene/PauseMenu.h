#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include "cocos2d.h"
#include "scene/MenuScene.h"
#include "SettingScene.h"

class PauseMenu : public cocos2d::Layer {
public:
    PauseMenu(Vec2 position);
    Menu* menu;

private:
    void resumeGame(cocos2d::Ref* sender);
    void quitGame(cocos2d::Ref* sender);
    void returnMenu(Ref* sender);
    void setting(Ref* sender);
};

#endif // __PAUSE_MENU_H__
