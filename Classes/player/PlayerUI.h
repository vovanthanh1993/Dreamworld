#pragma once
#ifndef __PlayerUI_H__
#define __PlayerUI_H__

#include "cocos2d.h"
#include "main/Effect.h"
#include "base/BaseLayer.h"

class PlayerUI : public BaseLayer {
public:
    static PlayerUI* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
    Label* stickLabel;
    Label* gemLabel;
    Label* healthPotionLabel;
    Label* manaPotionLabel;
    Sprite* healthBar; // Sprite cho thanh máu
    Sprite* healthBarBg; // Sprite cho nền thanh máu
    Sprite* manaBar; // Sprite cho thanh máu
    Sprite* manaBarBg; // Sprite cho nền thanh máu
    Label* healthLabel;
    void createHealthBar();
    void createManaBar();
    void updateHealthBar(float health);
    void updateManaBar(float mana);
    void setSpriteCharm(Charm* charm);
    void update(float dt);
    Scene* scene;
};
#endif // __PlayerUI_H__
