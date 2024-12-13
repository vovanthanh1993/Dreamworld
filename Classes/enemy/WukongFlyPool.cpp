#include "WukongFlyPool.h"
#include "main/Effect.h"
WukongFlyPool::WukongFlyPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        WukongFly* value = new WukongFly(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

WukongFlyPool::~WukongFlyPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (WukongFly* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
WukongFly* WukongFlyPool::getFromPool() {
    for (WukongFly* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<WukongFly*>& WukongFlyPool::getPool() const {
    return pool;
}

// Setter cho pool
void WukongFlyPool::setPool(const std::vector<WukongFly*>& newPool) {
    pool = newPool;
}