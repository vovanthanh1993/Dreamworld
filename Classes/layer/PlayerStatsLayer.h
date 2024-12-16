#ifndef __PlayerStatsLayer_H__
#define __PlayerStatsLayer_H__

#include "cocos2d.h"
#include "main/Effect.h"
#include "base/BaseLayer.h"

class PlayerStatsLayer : public BaseLayer {
public:
    static PlayerStatsLayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuOKCallback(cocos2d::Ref* pSender);
};
#endif // __PlayerStatsLayer_H__
