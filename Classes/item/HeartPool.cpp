#include "HeartPool.h"
#include "main/Effect.h"
HeartPool::HeartPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Heart* value = new Heart(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

HeartPool::~HeartPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Heart* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Heart* HeartPool::getFromPool() {
    for (Heart* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<Heart*>& HeartPool::getPool() const {
    return pool;
}

// Setter cho pool
void HeartPool::setPool(const std::vector<Heart*>& newPool) {
    pool = newPool;
}