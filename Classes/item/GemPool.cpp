#include "GemPool.h"
#include "main/Effect.h"
GemPool::GemPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Gem* value = new Gem(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

GemPool::~GemPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Gem* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Gem* GemPool::getFromPool() {
    for (Gem* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}