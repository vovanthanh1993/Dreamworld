#include "SlashPool.h"
#include "main/Effect.h"
SlashPool::SlashPool(b2World* world, Scene* scene, int poolSize) {
    // Tạo một danh sách các Slash trống
    for (int i = 0; i < poolSize; ++i) {
        Slash* slash = new Slash(world, scene);  // Tạo các đối tượng Slash chưa kích hoạt
        pool.push_back(slash);
    }
}

SlashPool::~SlashPool() {
    // Giải phóng tất cả các đối tượng trong pool
    for (Slash* slash : pool) {
        delete slash;
    }
}

// Lấy một Slash từ pool
Slash* SlashPool::getFromPool() {
    for (Slash* slash : pool) {
        if (!slash->getIsActive()) {
            return slash;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
        }
    }
    return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
}