/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __Boss1Scene_H__
#define __Boss1Scene_H__
#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include "MyContactListener.h"

#include "player/Player.h"
#include "Warrior.h"
#include "Stick.h"
#include "SlashEnemy.h"
#include "Chest.h"
#include "Item/heart.h"
#include "enemy/Acher.h"
#include <Bridge.h>
#include "BridgeBreak.h"
#include "map/Box.h"
#include "BossMap1.h"
#include "Map2Scene.h"
#include "Common.h"
#include "gui/Setting.h"

using namespace common;
using namespace cocos2d;
using namespace std;
class Boss1Scene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    bool onMouseDown(Event* event);
    bool onMouseUp(Event* event);
    std::set<EventKeyboard::KeyCode> _keysPressed;
    
    CREATE_FUNC(Boss1Scene);

private:
        bool isEnable = true;
        b2World* world;
        void setWorld(b2World* m_world) {
            world = m_world;
        }
        void update(float dt) override;
        Sprite* wizardSprite;
        Size vis;
        SpriteBatchNode* wizardSpriteNode;
        bool isJumping = false;
        
        std::unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap = new unordered_map<b2Body*, Sprite*>();
        void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
        void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
        
       
        TMXTiledMap* map;
        TMXLayer* wallLayer;
        MyContactListener* contactListener;
        Player* player;
        std::vector<Slash*> slashVector;
        BossMap1* bossmap1;
        void spawnObject();
        Setting* settingInit = new Setting();
};

#endif // __Boss1Scene_H__