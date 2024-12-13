#include "BackStickPool.h"
#include "main/Effect.h"
BackStickPool::BackStickPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        BackStick* value = new BackStick(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

BackStickPool::~BackStickPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (BackStick* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
BackStick* BackStickPool::getFromPool() {
    for (BackStick* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<BackStick*>& BackStickPool::getPool() const {
    return pool;
}

// Setter cho pool
void BackStickPool::setPool(const std::vector<BackStick*>& newPool) {
    pool = newPool;
}