#include "BaseEnemy.h"

BaseEnemy::BaseEnemy(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap): BaseNode(world, scene, bodyToSpriteMap) {  
}