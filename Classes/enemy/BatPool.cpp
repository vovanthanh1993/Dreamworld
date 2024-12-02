#include "BatPool.h"
#include "main/Effect.h"
BatPool::BatPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Bat* value = new Bat(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

BatPool::~BatPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Bat* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Bat* BatPool::getFromPool() {
    for (Bat* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<Bat*>& BatPool::getPool() const {
    return pool;
}

// Setter cho pool
void BatPool::setPool(const std::vector<Bat*>& newPool) {
    pool = newPool;
}