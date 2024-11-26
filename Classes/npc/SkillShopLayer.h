
#ifndef __SkillShopLayer_H__
#define __SkillShopLayer_H__

#include "cocos2d.h"
#include "player/Player.h"

USING_NS_CC;
class SkillShopLayer : public cocos2d::Layer {
public:
    CREATE_FUNC(SkillShopLayer); // Macro tạo lớp
    Player* player;
    LayerColor* shopFrame;
    SkillShopLayer();
private:
    void menuCloseCallback(cocos2d::Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(cocos2d::Ref* pSender);
    void purchaseGourd(cocos2d::Ref* sender); // Xử lý mua vật phẩm
    void purchaseEyesSkill(cocos2d::Ref* sender);

};
#endif // __SkillShopLayer_H__