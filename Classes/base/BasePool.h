#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"


using namespace constants;
using namespace common;
using namespace cocos2d;

// Định nghĩa lớp template
template <typename T>
class BasePool
{
private:
	vector<T*> pool;
public:
	//BasePool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	//~BasePool();
	//T* getFromPool();
	//const vector<T*>& getPool() const;        // Getter
	//void setPool(vector<T*>& newPool);  // Setter

    // Định nghĩa lớp template

    void createPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize) {
        // Tạo một danh sách các Slash trống
        for (int i = 0; i < poolSize; ++i) {
            T* value = new T(world, scene, bodyToSpriteMap);  // Tạo các đối tượng Slash chưa kích hoạt
            pool.push_back(value);
        }
    }

    BasePool() {};

    // Định nghĩa lớp template
    ~BasePool() {
        // Giải phóng tất cả các đối tượng trong pool
        for (T* value : pool) {
            delete value;
        }
    }

    // Lấy một Slash từ pool
    T* getFromPool() {
        for (T* value : pool) {
            if (!value->getIsActive()) {
                return value;  // Nếu Slash không còn hiển thị, có thể tái sử dụng
            }
        }
        return nullptr; // Nếu không có Slash nào có thể tái sử dụng, trả về nullptr
    }

    // Getter cho pool
    const vector<T*>& getPool() const {
        return pool;
    }

    // Setter cho pool
    void setPool(const vector<T*>& newPool) {
        pool = newPool;
    }
};

