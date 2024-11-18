#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "player/skill/Slash.h"
#include <base/BaseCharacter.h>
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
using namespace std;
class Player : public BaseCharacter
{
private:
	float health = Constants::MAX_HEALTH;
	float maxHealth = Constants::MAX_HEALTH;
	float maxMana = Constants::MAX_MANA;
	float mana = 5;
	int stickNum = 5;
	int gourdNum = 1;
	cocos2d::Sprite* healthBar; // Sprite cho thanh máu
	cocos2d::Sprite* healthBarBg; // Sprite cho nền thanh máu
	cocos2d::Sprite* manaBar; // Sprite cho thanh máu
	cocos2d::Sprite* manaBarBg; // Sprite cho nền thanh máu
	Label* healthLabel;
	map<EventKeyboard::KeyCode, bool> keys;
public:
	bool isAlive = true;
	Node* uiNode = Node::create();
	Label* stickLabel;
	Label* gemLabel;
	Label* gourdLabel;
	void walk();
	void die();
	void idle();
	void jump();
	Slash* hit();
	void init(bool isNew);
	void throwStick();
	void eagle();
	void setHealth(int h);
	int getHealth();
	void setStickNum(int h);
	int getStickNum();
	std::vector<Sprite*> healthVector;
	void initHealth();
	void getDamage(int damage);
	void healing(int num);
	void updateStickNum(int stick);
	void initItem();
	int maxStickNum = 10;
	int gem = 0;
	void updateGem(int i);
	Player(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	bool isEnable = true;
	int nextLevel = 0;
	void hurt();
	void savePlayerDataInit();
	void loadPlayerDataInit(bool isNew);
	void updateGourd(int i);
	void useGourd();
	void updateHealth(int damage);
	void updateHealthBar(float health);
	void createHealthBar();
	void healing();
	void createManaBar();
	void useMana(int mana);
	void updateManaBar(float mana);
	void updateMove();//std::map<EventKeyboard::KeyCode, bool> keys,
	void actionKey(EventKeyboard::KeyCode keyCode);

	void addMana(int num);
	// Getter cho maxMana
	float getMaxMana() const;

	// Setter cho maxMana
	void setMaxMana(float newMaxMana);

	// Getter cho mana
	float getMana() const;

	// Setter cho mana
	void setMana(float newMana);

	void setMaxHealth(float health);
	float getMaxHealth() const;
	bool isInVillage = false;

	float attackCooldown = 0.5f; // Khoảng thời gian chờ giữa các lần tấn công
	float lastAttackTime = 0; // Thời điểm của lần tấn công cuối cùng

	std::vector<Slash*> slashVector;

	void updateSlashVector(float dt);
	void initMouseEvent();
	void initKeyEvent();
};

