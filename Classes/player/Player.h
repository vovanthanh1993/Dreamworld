#pragma once
#ifndef __Player_H__
#define __Player_H__
#include "cocos2d.h"
#include "main/Common.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "player/skill/Slash.h"
#include "base/BaseNode.h"
#include "main/Constants.h"
#include "item/charm.h"
#include "base/BasePool.h"
#include "skill/Stick.h"
#include "skill/Eagle.h"


using namespace constants;
using namespace common;
using namespace cocos2d;
using namespace std;

class Player : public BaseNode
{
private:
	float health = Constants::MAX_HEALTH;
	float maxHealth = Constants::MAX_HEALTH;
	float maxMana = Constants::MAX_MANA;
	float mana = 5;
	int stickNum = 5;
	int healthPotionNum = 1;
	int manaPotionNum = 1;
	int gem;
	int maxStickNum;
	float slashDamage;
	float stickDamage;
	float eagleDamage;

	map<EventKeyboard::KeyCode, bool> keys;
	float attackCooldown = 0.5f; // Khoảng thời gian chờ giữa các lần tấn công
	float lastAttackTime = 0; // Thời điểm của lần tấn công cuối cùng
	float attackCooldownStick = 0.5f; // Khoảng thời gian chờ giữa các lần tấn công
	float lastAttackTimeStick = 0; // Thời điểm của lần tấn công cuối cùng
	float attackCooldownEagle = 0.5f; // Khoảng thời gian chờ giữa các lần tấn công
	float lastAttackTimeEagle = 0; // Thời điểm của lần tấn công cuối cùng
	BasePool<Slash>* slashPool = new BasePool<Slash>();
	BasePool<Eagle>* eaglePool = new BasePool<Eagle>();
	BasePool<Stick>* stickPool = new BasePool<Stick>();
	int direction = 1;

	
public:
	Player(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool isEnable;
	bool isComplete = false;
	bool isAlive;
	bool isInVillage = false;

	Charm* currentCharm;
	Vector<Charm*> charmVector;
	
	void walk();
	void die();
	void idle();
	void jump();
	void hit();
	bool init(bool isNew);
	void throwStick();
	void throwEagle();
	void setHealth(int h);
	int getHealth();
	void setStickNum(int h);
	int getStickNum();
	void getDamage(int damage);
	void healing(int num);
	void updateStickNum(int stick);
	void initGUI();
	void updateGem(int i);
	void hurt();
	void savePlayerDataInit();
	void loadPlayerDataInit(bool isNew);
	void addHealthPotion(int i);
	void useHealthPotion();
	void addManaPotion(int i);
	void useManaPotion();
	void updateHealth(int damage);
	void healing();
	void useMana(int mana);
	void update(float dt);
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
	
	void initMouseEvent();
	void initKeyEvent();
	void addEquipment(Charm* eq);
	void changeCharm(Charm* charm);
	void readCharmFromFile();
	void writeCharmToFile();
	void setSpriteCharm(Charm* charm);

	// Getters
	int getHealthPotionNum() const;
	int getManaPotionNum() const;
	int getGem() const;
	int getMaxStickNum() const;
	float getSlashDamage() const;
	float getStickDamage() const;
	float getEagleDamage() const;

	// Setters
	void setHealthPotionNum(int num);
	void setManaPotionNum(int num);
	void setGem(int value);
	void setMaxStickNum(int value);
	void setSlashDamage(float value);
	void setStickDamage(float value);
	void setEagleDamage(float value);
};

#endif // __Player_H__