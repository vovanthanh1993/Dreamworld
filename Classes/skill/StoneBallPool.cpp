#include "StoneBallPool.h"
#include "main/Effect.h"
StoneBallPool::StoneBallPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        StoneBall* value = new StoneBall(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

StoneBallPool::~StoneBallPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (StoneBall* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
StoneBall* StoneBallPool::getFromPool() {
    for (StoneBall* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}