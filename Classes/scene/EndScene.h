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

#ifndef __EndScene_H__
#define __EndScene_H__

#include <string>
#include "base/BaseScene.h"
#include "scene/boss/Boss4Scene.h"
#include "layer/InventoryLayer.h"
#include "item/Port.h"
#include "scene/MenuScene.h"

class EndScene : public Scene
{
public:
    static Scene* createScene();
    bool init();
private:
    void update(float dt) override;
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    Setting* settingInit = new Setting();
    int index = 0;
    vector<string> welcomeChatVector = {
        "Finally, you have arrived here...",
        "This place is Dreamworld, the forest that hides your fears...",
        "I am Miko, the guardian of this place...",
        "If you want to escape this forest, you must face the fears of your own self...",
        "Otherwise, it will consume everything you remember about this world...",
        "This door will lead you to your greatest fear. Be careful!"};
};

#endif // __EndScene_H__