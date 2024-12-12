#include "WukongPool.h"
#include "main/Effect.h"
WukongPool::WukongPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Wukong* value = new Wukong(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

WukongPool::~WukongPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Wukong* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Wukong* WukongPool::getFromPool() {
    for (Wukong* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<Wukong*>& WukongPool::getPool() const {
    return pool;
}

// Setter cho pool
void WukongPool::setPool(const std::vector<Wukong*>& newPool) {
    pool = newPool;
}