#ifndef __GUILayer_H__
#define __GUILayer_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "main/Effect.h"

class GUILayer : public cocos2d::Layer {
public:
    LayerColor* shopFrame;
    Player* player;
    CREATE_FUNC(GUILayer); // Macro tạo lớp
    GUILayer();
private:
    void menuOKCallback(cocos2d::Ref* pSender);
};
#endif // __GUILayer_H__
