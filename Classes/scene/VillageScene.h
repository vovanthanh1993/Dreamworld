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


#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include "main/MyContactListener.h"
#include "main/Common.h"
#include "player/Player.h"
#include "enemy/Warrior.h"
#include "player/skill/Stick.h"
#include "skill/SlashEnemy.h"
#include "item/Chest.h"
#include "Item/heart.h"
#include "enemy/Acher.h"
#include "map/Bridge.h"
#include "map/BridgeBreak.h"
#include "map/Box.h"
#include "enemy/BossMap1.h"
#include "npc/ShopLayer.h"
#include "npc/NPC2.h"
#include "npc/GUILayer.h"
#include "gui/Setting.h"
#include "npc/NPC3.h"
#include "npc/SkillShopLayer.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
using namespace std;
class VillageScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    std::map<EventKeyboard::KeyCode, bool> keys;
   CREATE_FUNC(VillageScene);
private:
        bool isPaused = false;
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

        TMXTiledMap* map;
        MyContactListener* contactListener;
       
        float time = 1.0 / 60.0f;
        Player* player;
        int status = 1;
        std::vector<Slash*> slashVector;
        void spawnObject();
        bool isEndMap1 = true;
        bool isEnable = true;
        NPC1* npc1;
        NPC3* npc3;
        NPC2* npc2;
        ShopLayer* shopLayer;
        GUILayer* guiLayer;
        Setting* settingInit = new Setting();
        SkillShopLayer* skillShopLayer;
        void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

