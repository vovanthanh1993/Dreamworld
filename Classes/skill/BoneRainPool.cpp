#include "BoneRainPool.h"
#include "main/Effect.h"
BoneRainPool::BoneRainPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        BoneRain* value = new BoneRain(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

BoneRainPool::~BoneRainPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (BoneRain* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
BoneRain* BoneRainPool::getFromPool() {
    for (BoneRain* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}