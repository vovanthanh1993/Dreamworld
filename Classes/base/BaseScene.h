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
#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__
#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include "main/MyContactListener.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "main/Common.h"
#include "gui/Setting.h"
#include "item/MapItem.h"
#include "main/Constants.h"
#include <vector>
#include "audio/include/AudioEngine.h"
#include "main/MusicManager.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
using namespace std;
class BaseScene : public cocos2d::Scene
{
public:
    static Scene* createScene(string bg, string bgMusic, string mapName, bool isMoveCamera);
    bool init(string bg, string bgMusic, string mapName, bool isMoveCamera);
    BaseScene();  // Constructor
    ~BaseScene(); // Destructor

protected:
    bool isEnable = true;
    b2World* world;
    void update(float dt) override;
    TMXTiledMap* map;
    MyContactListener* contactListener;
    float time = 1.0 / 60.0f;
    Player* player;
    virtual void spawnObject();
    Setting* settingInit = new Setting();
    b2Body* boundaryBodyStart;
    b2Body* boundaryBodyEnd;
    MapItem* item;
    bool isMoveCamera;
    unordered_map<b2Body*, Sprite*>* bodyToSpriteMap = new unordered_map<b2Body*, Sprite*>();
    bool isEndMap = false;
};

#endif // __BASE_SCENE_H__