#include "PoisonRainPool.h"
#include "main/Effect.h"
PoisonRainPool::PoisonRainPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        PoisonRain* value = new PoisonRain(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

PoisonRainPool::~PoisonRainPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (PoisonRain* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
PoisonRain* PoisonRainPool::getFromPool() {
    for (PoisonRain* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}