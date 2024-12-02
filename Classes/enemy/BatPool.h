#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "Bat.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BatPool
{
private:
	std::vector<Bat*> pool;
public:
	BatPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~BatPool();
	Bat* getFromPool();
	const std::vector<Bat*>& getPool() const;        // Getter
	void setPool(const std::vector<Bat*>& newPool);  // Setter
};

