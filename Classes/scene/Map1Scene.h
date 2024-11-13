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

#ifndef __NewScene_H__
#define __NewScene_H__
#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include "main/MyContactListener.h"

#include "player/Player.h"
#include "enemy/Warrior.h"
#include "skill/Stick.h"
#include "skill/SlashEnemy.h"
#include "item/Chest.h"
#include "Item/heart.h"
#include "enemy/Acher.h"
#include "map/Bridge.h"
#include "map/BridgeBreak.h"
#include "map/Box.h"
#include "enemy/BossMap1.h"
#include "scene/Map2Scene.h"
#include "main/Common.h"
#include "gui/Setting.h"
#include "gui/Setting.h"
#include "map/SizeLimit.h"
#include "item/MapItem.h"

using namespace common;
using namespace cocos2d;
using namespace std;
class Map1Scene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    bool onMouseDown(Event* event);
    bool onMouseUp(Event* event);
    std::map<EventKeyboard::KeyCode, bool> keys;
    CREATE_FUNC(Map1Scene);

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
       
        float time = 1.0 / 60.0f;
        Player* player;
        int status = 1;
        std::vector<Slash*> slashVector;
        std::vector<SlashEnemy*> slashEnemyVector;
        std::vector<Warrior*>* warriorVector = new std::vector<Warrior*>();
        std::vector<Acher*>* acherVector = new std::vector<Acher*>();
        void spawnObject();
        Setting* settingInit = new Setting();
        SizeLimit* startLimit;
        SizeLimit* endLimit;
        b2Body* boundaryBodyStart;
        b2Body* boundaryBodyEnd;
};

#endif // __NewScene_H__

