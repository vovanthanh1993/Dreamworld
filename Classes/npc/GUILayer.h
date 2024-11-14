
#include "cocos2d.h"
#include "player/Player.h"

class GUILayer : public cocos2d::Layer {
public:
    LayerColor* shopFrame;
    Player* player;
    CREATE_FUNC(GUILayer); // Macro tạo lớp
    GUILayer();
private:
    void menuOKCallback(cocos2d::Ref* pSender);
};

