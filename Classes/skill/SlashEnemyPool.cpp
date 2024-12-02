#include "SlashEnemyPool.h"
#include "main/Effect.h"
SlashEnemyPool::SlashEnemyPool(b2World* world, Scene* scene,  int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        SlashEnemy* value = new SlashEnemy(world, scene);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(value);
    }
}

SlashEnemyPool::~SlashEnemyPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (SlashEnemy* value : pool) {
        delete value;
    }
}

// Lấy một Slash từ pool
SlashEnemy* SlashEnemyPool::getFromPool() {
    for (SlashEnemy* value : pool) {
        if (!value->getIsActive()) {
            return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}