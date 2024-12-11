#ifndef __SHOP_LAYER_H__
#define __SHOP_LAYER_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "base/BaseLayer.h"
USING_NS_CC;
class ShopLayer : public BaseLayer {
public:
    static ShopLayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuCloseCallback(Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(Ref* pSender);
    void purchaseStickItem(Ref* sender); // Xử lý mua vật phẩm
    void purchaseHealthItem(Ref* sender);
    void upgradeManaItem(Ref* pSender);
};

#endif // __SHOP_LAYER_H__
