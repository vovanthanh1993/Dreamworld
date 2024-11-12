

#include "cocos2d.h"
#include "player/Player.h"

class SkillShopLayer : public cocos2d::Layer {
public:
    virtual bool init(); // Khởi tạo layer
    
    bool isEnable = false;
    CREATE_FUNC(SkillShopLayer); // Macro tạo lớp
    Player* player;
private:
    void menuCloseCallback(cocos2d::Ref* pSender); // Callback cho nút trở về
    void menuOKCallback(cocos2d::Ref* pSender);
    void purchaseGourd(cocos2d::Ref* sender); // Xử lý mua vật phẩm
    void purchaseEyesSkill(cocos2d::Ref* sender);

};
