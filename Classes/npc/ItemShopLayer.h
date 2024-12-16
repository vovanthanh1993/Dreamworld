#ifndef __ItemShopLayer_H__
#define __ItemShopLayer_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "base/BaseLayer.h"
USING_NS_CC;
using namespace cocos2d::ui;
class ItemShopLayer : public BaseLayer {
public:
    static ItemShopLayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuCloseCallback(Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(Ref* pSender);
    void purchaseHealthPotion(Ref* sender); // Xử lý mua vật phẩm
    void purchaseManaPotion(Ref* sender);
};

#endif // __ItemShopLayer_H__
