#include "BaseLayer.h"

using namespace std;
USING_NS_CC;
BaseLayer::BaseLayer() {
    // Khởi tạo các thành phần riêng cho GameScene nếu cần
}

BaseLayer::~BaseLayer() {
    // Giải phóng tài nguyên của GameScene nếu cần
}

Layer* BaseLayer::createLayer(Player* player, Scene* scene)
{
    BaseLayer* ret = new BaseLayer();
    if (ret && ret->init(player, scene)) {
        ret->autorelease();  // Tự động giải phóng bộ nhớ
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

//on "init" you need to initialize your instance
bool BaseLayer::init(Player* player, Scene* scene)
{
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    return true;
}







