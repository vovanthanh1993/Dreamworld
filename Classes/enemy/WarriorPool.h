#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "Warrior.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class WarriorPool
{
private:
	std::vector<Warrior*> pool;
public:
	WarriorPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~WarriorPool();
	Warrior* getFromPool();
	const std::vector<Warrior*>& getPool() const;        // Getter
	void setPool(const std::vector<Warrior*>& newPool);  // Setter
};

