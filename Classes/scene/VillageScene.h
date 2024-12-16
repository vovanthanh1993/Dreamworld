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

#ifndef __VillageScene_H__
#define __VillageScene_H__

#include <string>
#include "npc/NPC2.h"
#include "npc/GUILayer.h"
#include "npc/NPC3.h"
#include "npc/ItemShopLayer.h"
#include "base/BaseScene.h"
#include "npc/NPC1.h"
#include "npc/UpgradeShopLayer.h"
#include "scene/Map1Scene.h"
#include "npc/NPCMonkey.h"
#include "inventory/InventoryLayer.h"

class VillageScene : public BaseScene
{
public:
    static Scene* createScene(string bg, string bgMusic, string mapName, bool isMoveCamera);
    bool init(string bg, string bgMusic, string mapName, bool isMoveCamera);
private:
        void update(float dt) override;
        void spawnObject();
        NPC1* npc1;
        NPC3* npc3;
        NPC2* npc2;
        UpgradeShopLayer* upgradeShopLayer;
        GUILayer* guiLayer;
        ItemShopLayer* itemShopLayer;
        void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // __VillageScene_H__