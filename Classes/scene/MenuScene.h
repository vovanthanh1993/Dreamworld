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
#ifndef __MenuScene_H__
#define __MenuScene_H__

#include "cocos2d.h"
#include "GUIScene.h"
#include "main/SettingManager.h"
#include <string>
#include <thread>
#include "ui\CocosGUI.h"
#include "LoadingScene.h"
#include "scene/Map2Scene.h"
#include "scene/Map1Scene.h"
#include "SettingScene.h"
#include "ShopScene.h"
#include "main/Common.h"
#include "scene/boss/Boss1Scene.h"
#include "scene/boss/Boss2Scene.h"
#include "scene/boss/Boss3Scene.h"

#include "scene/Map3Scene.h"

using namespace cocos2d;
using namespace common;
using namespace std;
class MenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void loadingbar();
    void update(float dt) override;
    void onClickMenuItem(Ref* sender);
    void zoomAction(Menu* item);
    void setting(Ref* sender);
    void guide(Ref* sender);
    void shop(Ref* sender);
    CREATE_FUNC(MenuScene);

};
#endif // __MenuScene_H__