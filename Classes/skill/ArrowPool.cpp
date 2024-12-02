#include "ArrowPool.h"
#include "main/Effect.h"
ArrowPool::ArrowPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Arrow* value = new Arrow(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

ArrowPool::~ArrowPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Arrow* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Arrow* ArrowPool::getFromPool() {
    for (Arrow* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}