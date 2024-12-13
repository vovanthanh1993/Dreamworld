#include "SoulPool.h"
#include "main/Effect.h"
SoulPool::SoulPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Soul* value = new Soul(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

SoulPool::~SoulPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Soul* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Soul* SoulPool::getFromPool() {
    for (Soul* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<Soul*>& SoulPool::getPool() const {
    return pool;
}

// Setter cho pool
void SoulPool::setPool(const std::vector<Soul*>& newPool) {
    pool = newPool;
}