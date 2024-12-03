#include "WarriorPool.h"
#include "main/Effect.h"
WarriorPool::WarriorPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Warrior* value = new Warrior(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

WarriorPool::~WarriorPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Warrior* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
Warrior* WarriorPool::getFromPool() {
    for (Warrior* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}

// Getter cho pool
const std::vector<Warrior*>& WarriorPool::getPool() const {
    return pool;
}

// Setter cho pool
void WarriorPool::setPool(const std::vector<Warrior*>& newPool) {
    pool = newPool;
}