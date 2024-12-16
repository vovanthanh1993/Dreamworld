#ifndef __GUILayer_H__
#define __GUILayer_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "main/Effect.h"
#include "base/BaseLayer.h"

class GUILayer : public BaseLayer {
public:
    static GUILayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuOKCallback(cocos2d::Ref* pSender);
};
#endif // __GUILayer_H__
