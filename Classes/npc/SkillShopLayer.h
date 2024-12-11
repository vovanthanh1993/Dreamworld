#ifndef __SkillShopLayer_H__
#define __SkillShopLayer_H__

#include "cocos2d.h"
#include "player/Player.h"
#include "base/BaseLayer.h"
USING_NS_CC;
class SkillShopLayer : public BaseLayer {
public:
    static SkillShopLayer* createLayer(Player* player, Scene* scene);
    bool init(Player* player, Scene* scene);
private:
    void menuCloseCallback(Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(Ref* pSender);
    void purchaseEyesSkill(Ref* sender); // Xử lý mua vật phẩm
    void purchaseGourd(Ref* sender);
};

#endif // __SkillShopLayer_H__
