#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "Heart.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class HeartPool
{
private:
	std::vector<Heart*> pool;
public:
	HeartPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~HeartPool();
	Heart* getFromPool();
	const std::vector<Heart*>& getPool() const;        // Getter
	void setPool(const std::vector<Heart*>& newPool);  // Setter
};

