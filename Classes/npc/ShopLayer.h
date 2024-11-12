#ifndef __SHOP_LAYER_H__
#define __SHOP_LAYER_H__

#include "cocos2d.h"
#include "player/Player.h"

class ShopLayer : public cocos2d::Layer {
public:
    virtual bool init(); // Khởi tạo layer
    
    bool isEnable = false;
    CREATE_FUNC(ShopLayer); // Macro tạo lớp
    Player* player;
private:
    void menuCloseCallback(cocos2d::Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(cocos2d::Ref* pSender);
    void purchaseStickItem(cocos2d::Ref* sender); // Xử lý mua vật phẩm
    void purchaseHealthItem(cocos2d::Ref* sender);
    void upgradeManaItem(cocos2d::Ref* pSender);
};

#endif // __SHOP_LAYER_H__
