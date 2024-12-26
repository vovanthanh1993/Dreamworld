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

#ifndef __MikoScene_H__
#define __MikoScene_H__

#include <string>
#include "base/BaseScene.h"
#include "scene/boss/Boss4Scene.h"
#include "npc/Miko.h"
#include "layer/InventoryLayer.h"
#include "item/Port.h"
#include "scene/VillageScene.h"

class MikoScene : public BaseScene
{
public:
    static Scene* createScene(string bg, string bgMusic, string mapName, bool isMoveCamera);
    bool init(string bg, string bgMusic, string mapName, bool isMoveCamera);
private:
    void update(float dt) override;
    void spawnObject();
    Miko* miko;
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    bool isHasGate = false;
    int index = 0;
    vector<string> welcomeChatVector = {
        "Finally, you have arrived here...",
        "This place is Dreamworld, it's created by your subconscious now...",
        "Before, it was a beautiful world...",
        "But since your mother passed away, it has been overshadowed by your fears...",
        "A stone of loneliness, a ghost of death, a deep darkness in your mind...",
        "You've been trapped here for a long time, you've been here many times... ",
        "But you still can't free yourself, forgetting everything once again...",
        "If you want to try once more, this is the gate leading to your bondage. Be careful!"};
    vector<string> completeChatVector = {
        "Originally, both you and I had already died and become trapped in this forest...",
        "We have made many mistakes in the past...",
        "I have been imprisoned here for over 1,000 years to guide people like you...",
        "If I want to leave, someone must accept to take my place..",
        "If you step into the gate on your right, you will be freed from here...",
        "If you step into the gate on your left, you will become me, waiting here for the next one...",
        "The choice is yours..." };
};

#endif // __MikoScene_H__