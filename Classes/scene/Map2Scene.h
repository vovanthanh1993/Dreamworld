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

#ifndef __Map2Scene_H__
#define __Map2Scene_H__
#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include "base/BaseScene.h"
#include "main/Common.h"
#include "player/Player.h"
#include "enemy/Warrior.h"
#include "skill/SlashEnemy.h"
#include "enemy/BossMap2.h"
#include "enemy/Wraith.h"
#include "main/Constants.h"
#include "enemy/Acher.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
using namespace std;
class Map2Scene : public BaseScene
{
public:
    static Scene* createScene(string bg, string bgMusic, string mapName, bool isMoveCamera);
    bool init(string bg, string bgMusic, string mapName, bool isMoveCamera) override;
private:
        std::vector<SlashEnemy*> slashEnemyVector;
        std::vector<Warrior*>* warriorVector = new std::vector<Warrior*>();
        std::vector<Acher*>* acherVector = new std::vector<Acher*>();
        std::vector<Wraith*>* wraithVector = new std::vector<Wraith*>();
        void spawnObject();
        void update(float dt) override;
};

#endif // __Map2Scene_H__

