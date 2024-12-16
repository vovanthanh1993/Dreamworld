#ifndef __UpgradeShopLayer_H__
#define __UpgradeShopLayer_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "base/BaseLayer.h"
USING_NS_CC;
class UpgradeShopLayer : public BaseLayer {
public:
    static UpgradeShopLayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuCloseCallback(Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(Ref* pSender);
    void upgradeHealth(Ref* sender); // Xử lý mua vật phẩm
    void upgradeMana(Ref* sender);
    void upgradeStick(Ref* pSender);
};

#endif // __UpgradeShopLayer_H__
