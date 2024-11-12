#include "GUI.h"
namespace gui {
    void GUI::createHeath(Scene* scene, int n) {
        int xBase = 100;
        for (int i = 1;i <= n;i++) {
            Sprite* heath = Sprite::create("ui/heath.png");
            heath->setScale(0.1);
            heath->setPosition(xBase, 1000);
            scene->addChild(heath);
            xBase += 10;
        }
    }
}
