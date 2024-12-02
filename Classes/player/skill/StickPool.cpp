#include "StickPool.h"
#include "main/Effect.h"
StickPool::StickPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Stick* value = new Stick(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

StickPool::~StickPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Stick* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Stick* StickPool::getFromPool() {
    for (Stick* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}