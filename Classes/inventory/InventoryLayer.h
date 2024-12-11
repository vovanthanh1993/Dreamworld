#ifndef __InventoryLayer_H__
#define __InventoryLayer_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "base/BaseLayer.h"
#include "Inventory/charm.h"

class InventoryLayer : public BaseLayer {
public:
    static InventoryLayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuCloseCallback(cocos2d::Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(cocos2d::Ref* pSender);
    void selectCharm(cocos2d::Ref* sender); // Xử lý mua vật phẩm
    Vector<MenuItem*> menuItems;
};

#endif // __InventoryLayer_H__
