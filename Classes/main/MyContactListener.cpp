#include "main/MyContactListener.h"

void MyContactListener::BeginContact(b2Contact* contact) {

    // Lấy hai vật thể va chạm
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    // Truy xuất Sprite từ UserData
    Sprite* spriteA = static_cast<Sprite*>(bodyA->GetUserData());
    Sprite* spriteB = static_cast<Sprite*>(bodyB->GetUserData());

    if (spriteA != nullptr && spriteB != nullptr) {
        int tagA = spriteA->getTag();
        int tagB = spriteB->getTag();

        // Tag Box
        if (tagA == Constants::TAG_BOX && tagB == Constants::TAG_WALL) {
            bodyA->SetLinearVelocity(b2Vec2_zero);
        }
        if (tagB == Constants::TAG_BOX && tagA == Constants::TAG_WALL) {
            bodyB->SetLinearVelocity(b2Vec2_zero);
        }

        // Huy stick, xu ly phong trung ke dich
        if (tagB == Constants::TAG_STICK) {
            bodiesToRemove.insert(bodyB);
            if (tagA != Constants::TAG_WALL && tagA != Constants::TAG_BRIDGE_BREAK) {
                player->addMana(1);
            }
            if (tagA == Constants::TAG_WAR) static_cast<Warrior*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_ACHER) static_cast<Acher*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_BOSSMAP1) bossmap1->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(player->getStickDamage());
            else if (tagA == Constants::TAG_BOSSMAP3) bossmap3->updateHealth(player->getStickDamage());
            else if (tagA == Constants::TAG_HED)  static_cast<Hedgehog*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_GOLEM)  static_cast<Golem*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_BAT)  static_cast<Bat*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_WUKONG)  static_cast<Wukong*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_WUKONG_FLY)  static_cast<WukongFly*>(spriteA->getUserData())->getDamage(player->getStickDamage());
            else if (tagA == Constants::TAG_BOSSMAP4)  static_cast<BossMap4*>(spriteA->getUserData())->getDamage(player->getStickDamage());
        }
        if (tagA == Constants::TAG_STICK) {
            bodiesToRemove.insert(bodyA);
            if (tagB != Constants::TAG_WALL && tagB != Constants::TAG_BRIDGE_BREAK) {
                player->addMana(1);
            }
            if (tagB == Constants::TAG_WAR) static_cast<Warrior*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_ACHER) static_cast<Acher*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_BOSSMAP1) bossmap1->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(player->getStickDamage());
            else if (tagB == Constants::TAG_BOSSMAP3) bossmap3->updateHealth(player->getStickDamage());
            else if (tagB == Constants::TAG_HED)  static_cast<Hedgehog*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_GOLEM)  static_cast<Golem*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_BAT)  static_cast<Bat*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_WUKONG)  static_cast<Wukong*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_WUKONG_FLY)  static_cast<WukongFly*>(spriteB->getUserData())->getDamage(player->getStickDamage());
            else if (tagB == Constants::TAG_BOSSMAP4)  static_cast<BossMap4*>(spriteB->getUserData())->getDamage(player->getStickDamage());
        }
        

        // Tha qua trung ke dich
        if (tagB == Constants::TAG_EAGLE) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_WAR) static_cast<Warrior*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_ACHER) static_cast<Acher*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_BOSSMAP1) bossmap1->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(player->getEagleDamage());
            else if (tagA == Constants::TAG_BOSSMAP3) bossmap3->updateHealth(player->getEagleDamage());
            else if (tagA == Constants::TAG_HED)  static_cast<Hedgehog*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_GOLEM)  static_cast<Golem*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_BAT)  static_cast<Bat*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_WUKONG)  static_cast<Wukong*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_WUKONG_FLY)  static_cast<WukongFly*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
            else if (tagA == Constants::TAG_BOSSMAP4)  static_cast<BossMap4*>(spriteA->getUserData())->getDamage(player->getEagleDamage());
        }
        
        if (tagA == Constants::TAG_EAGLE) {
            bodiesToRemove.insert(bodyA);

            if (tagB == Constants::TAG_WAR) static_cast<Warrior*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_ACHER) static_cast<Acher*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_BOSSMAP1) bossmap1->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(player->getEagleDamage());
            else if (tagB == Constants::TAG_BOSSMAP3) bossmap3->updateHealth(player->getEagleDamage());
            else if (tagB == Constants::TAG_HED)  static_cast<Hedgehog*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_GOLEM)  static_cast<Golem*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_BAT)  static_cast<Bat*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_WUKONG)  static_cast<Wukong*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_WUKONG_FLY)  static_cast<WukongFly*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
            else if (tagB == Constants::TAG_BOSSMAP4)  static_cast<BossMap4*>(spriteB->getUserData())->getDamage(player->getEagleDamage());
        }
        

        // chem trung ke dich
        if (tagA == Constants::TAG_SLASH || tagB == Constants::TAG_SLASH) {
            player->addMana(1);
            if (tagA == Constants::TAG_WUKONG) static_cast<Wukong*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_WUKONG) static_cast<Wukong*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_WAR) static_cast<Warrior*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_WAR) static_cast<Warrior*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_ACHER) static_cast<Acher*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_ACHER) static_cast<Acher*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_HED) static_cast<Hedgehog*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_HED) static_cast<Hedgehog*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_GOLEM)  static_cast<Golem*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_GOLEM)  static_cast<Golem*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_BAT)  static_cast<Bat*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_BAT)  static_cast<Bat*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_WUKONG_FLY)  static_cast<WukongFly*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_WUKONG_FLY)  static_cast<WukongFly*>(spriteB->getUserData())->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_BOSSMAP4)  static_cast<BossMap4*>(spriteA->getUserData())->getDamage(player->getSlashDamage());
            else if (tagB == Constants::TAG_BOSSMAP4)  static_cast<BossMap4*>(spriteB->getUserData())->getDamage(player->getSlashDamage());


            else if (tagA == Constants::TAG_BOSSMAP1 || tagB == Constants::TAG_BOSSMAP1) bossmap1->getDamage(player->getSlashDamage());
            else if (tagA == Constants::TAG_BOSSMAP2 || tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(player->getSlashDamage());
            else if (tagA == Constants::TAG_BOSSMAP3 || tagB == Constants::TAG_BOSSMAP3) bossmap3->updateHealth(player->getSlashDamage());
        }

        // Huy arrow, xu ly phong trung player
        if (tagA == Constants::TAG_ARROW) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_PLAYER) {
                player->getDamage(Constants::DAMAGE_ARROW);
               
            }
        }
        if (tagB == Constants::TAG_ARROW) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_PLAYER) {
                player->getDamage(Constants::DAMAGE_ARROW);
                
            }
        }

        // Player va cham voi Bat
        if (tagA == Constants::TAG_PLAYER && tagB == Constants::TAG_BAT) {
            static_cast<Bat*>(spriteB->getUserData())->getDamage(1);
            player->getDamage(Constants::DAMAGE_BOOM);
        }
        else if (tagB == Constants::TAG_PLAYER && tagA == Constants::TAG_BAT) {
            static_cast<Bat*>(spriteA->getUserData())->getDamage(1);
            player->getDamage(Constants::DAMAGE_BOOM);
        }

        // Player va cham voi Bat
        if (tagA == Constants::TAG_PLAYER && tagB == Constants::TAG_WUKONG_FLY) {
            static_cast<WukongFly*>(spriteB->getUserData())->getDamage(1);
            player->getDamage(Constants::DAMAGE_BOOM);
        }
        else if (tagB == Constants::TAG_PLAYER && tagA == Constants::TAG_WUKONG_FLY) {
            static_cast<WukongFly*>(spriteA->getUserData())->getDamage(1);
            player->getDamage(Constants::DAMAGE_BOOM);
        }
        if (tagA == Constants::TAG_RAIN) {
            if (tagB == Constants::TAG_PLAYER) {
                player->getDamage(Constants::DAMAGE_BOOM);
            }
            bodiesToRemove.insert(bodyA);
            static_cast<PoisonRain*>(spriteA->getUserData())->reset();
        }
        if (tagB == Constants::TAG_RAIN) {
            if (tagA == Constants::TAG_PLAYER) {
                player->getDamage(Constants::DAMAGE_BOOM);
            }
            bodiesToRemove.insert(bodyB);
            static_cast<PoisonRain*>(spriteB->getUserData())->reset();
        }

        // Huy rain
        if (tagA == Constants::TAG_FIRE || tagA == Constants::Constants::TAG_BONE_RAIN || tagA == Constants::TAG_FIRE_RAIN) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_PLAYER) {
                player->getDamage(Constants::DAMAGE_BOOM);
            }
        }
        if (tagB == Constants::TAG_FIRE|| tagB == Constants::Constants::TAG_BONE_RAIN || tagB == Constants::TAG_FIRE_RAIN) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_PLAYER) {
                player->getDamage(Constants::DAMAGE_BOOM);
            }
        }

        // chem chest
        if (tagA == Constants::TAG_SLASH || tagB == Constants::TAG_SLASH) {
            if (tagA == Constants::TAG_CHEST) {
                bodiesToRemove.insert(bodyA);
            }
            else if (tagB == Constants::TAG_CHEST)
            {
                bodiesToRemove.insert(bodyB);
            }
        }

        // check enemy and limit
        if (tagB == Constants::TAG_WAR && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            Warrior* w = static_cast<Warrior*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(w->direction * w->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check enemy and limit
        if (tagB == Constants::TAG_WUKONG && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            Wukong* w = static_cast<Wukong*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(w->direction * w->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check golem and limit
        if (tagB == Constants::TAG_GOLEM && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            Golem* w = static_cast<Golem*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(w->direction * w->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check golem and limit
        if (tagB == Constants::TAG_BAT && tagA == Constants::TAG_LIMIT) {
            Bat* w = static_cast<Bat*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(w->direction * w->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check npc and limit
        if (tagB == Constants::TAG_NPC && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            NPCMonkey* w = static_cast<NPCMonkey*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(w->direction * w->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check boss and limit
        if (tagB == Constants::TAG_BOSSMAP1 && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            bossmap1->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(bossmap1->direction * bossmap1->speed* Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check boss and limit
        if (tagB == Constants::TAG_BOSSMAP2 && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            bossmap2->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(bossmap2->direction * bossmap2->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }

        // check golem and limit
        if (tagB == Constants::TAG_BOSSMAP3 && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            bossmap3->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(bossmap3->direction * bossmap3->speed * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(-spriteB->getScaleX());
        }


        // check enemy and limit
        if (tagB == Constants::TAG_BRIDGE && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            Bridge* w = static_cast<Bridge*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(0, w->direction* w->speed* Common::scaleSizeXY()));
        }

        // check player and bridgebreak
        if (tagB == Constants::TAG_PLAYER || tagA == Constants::TAG_PLAYER) {
            if (tagA == Constants::TAG_BRIDGE_BREAK) {
                bodyA->SetLinearVelocity(b2Vec2(0, -Constants::SPEED_BRIDGE_BREAK* Common::scaleSizeXY()));
            }
            if (tagB == Constants::TAG_BRIDGE_BREAK) {
                bodyB->SetLinearVelocity(b2Vec2(0, -Constants::SPEED_BRIDGE_BREAK* Common::scaleSizeXY()));
            }
            if (tagA == Constants::TAG_GEM) {
                player->updateGem(1);
                bodiesToRemove.insert(bodyA);
            }
            if (tagB == Constants::TAG_GEM) {
                player->updateGem(1);
                bodiesToRemove.insert(bodyB);
            }
            if (tagA == Constants::TAG_EQUIPMENT) {
                player->addCharm(static_cast<Charm*>(spriteA->getUserData()));
                bodiesToRemove.insert(bodyA);
            }
            if (tagB == Constants::TAG_EQUIPMENT) {
                player->addCharm(static_cast<Charm*>(spriteB->getUserData()));
                bodiesToRemove.insert(bodyB);
            }
            if (tagA == Constants::TAG_HEART) {
                player->healing(1);
                bodiesToRemove.insert(bodyA);
                
            }

            if (tagB == Constants::TAG_HEART) {
                player->healing(1);
                bodiesToRemove.insert(bodyB);
            }

            if (tagA == Constants::TAG_STICK_ITEM) {
                player->updateStickNum(1);
                bodiesToRemove.insert(bodyA);
            }
            if (tagB == Constants::TAG_STICK_ITEM) {
                player->updateStickNum(1);
                bodiesToRemove.insert(bodyB);
            }

            if (tagA == Constants::TAG_STONE_BALL) {
                player->getDamage(Constants::DAMAGE_STONE_BALL);
                bodiesToRemove.insert(bodyA);
                static_cast<StoneBall*>(spriteA->getUserData())->reset();
            }
            if (tagB == Constants::TAG_STONE_BALL) {
                player->getDamage(Constants::DAMAGE_STONE_BALL);
                bodiesToRemove.insert(bodyB);
                static_cast<StoneBall*>(spriteB->getUserData())->reset();
            }


            // chem trung ke dich
            if (tagA == Constants::TAG_SLASH_ENEMY || tagB == Constants::TAG_SLASH_ENEMY) {
                player->getDamage(Constants::DAMAGE_SLASH_ENEMY);
            }
            // chem trung ke dich
            if (tagA == Constants::TAG_HED || tagB == Constants::TAG_HED) {
                player->getDamage(Constants::DAMAGE_BOOM);
            }

            // chem trung ke dich
            if (tagA == Constants::TAG_BOSSMAP1 || tagB == Constants::TAG_BOSSMAP1) {
                player->getDamage(Constants::DAMAGE_BOOM);
            }
        }

        // Qua man
        if (tagA == Constants::TAG_ENDGATE && tagB == Constants::TAG_PLAYER || tagA == Constants::TAG_PLAYER && tagB == Constants::TAG_ENDGATE) {
            isNext = true;
        }
    }

}

// Phương thức để xóa đối tượng
void MyContactListener::removeObject() {
    for (b2Body* body : bodiesToRemove) {

            // Xóa dữ liệu người dùng (Sprite) khỏi hệ thống đồ họa
            Sprite* sprite = static_cast<Sprite*>(body->GetUserData());
            int tag = sprite->getTag();
            Vec2 pos = sprite->getPosition();
            int type = 1;
            if (Constants::TAG_STICK == tag) {
                Effect::destroyStick(world, scene, sprite->getPosition());
                type = 2;
            } else if (Constants::TAG_EAGLE == tag) {
                Effect::smoke(world, scene, pos);
                type = 2;
            }
            else if (Constants::TAG_STONE_BALL == tag) {
                Effect::smoke(world, scene, pos);
                type = 2;
            }
            else if (Constants::TAG_ARROW == tag) {
                Effect::destroyArrow(world, scene, sprite->getPosition());
                type = 2;
            } 
            else if (Constants::TAG_CHEST == tag) {
                MusicManager::getInstance()->chest();
                Common::spawnGem(world, scene, pos, bodyToSpriteMap, Common::randomNum(0, 6));
                Common::spawnCharm(world, scene, pos, bodyToSpriteMap, 1);
            }
            else if (Constants::TAG_RAIN == tag) {
                Effect::destroyRain(world, scene, pos);
                type = 2;
            }
            else if (Constants::TAG_BONE_RAIN == tag) {
                type = 2;
                Effect::smoke(world, scene, pos);
            }
            else if (Constants::TAG_HEART == tag || Constants::TAG_STICK_ITEM == tag || Constants::TAG_BOSSMAP4 == tag) {
                type = 2;
            }
            
            if (type == 2) {
                static_cast<BaseNode*>(sprite->getUserData())->reset();
            }
            (*bodyToSpriteMap).erase(body);
            world->DestroyBody(body);
            body = nullptr;
            sprite->removeFromParentAndCleanup(true);
            sprite = nullptr; 
    }
    // Xóa danh sách body đã xóa
    bodiesToRemove.clear();
}

MyContactListener::MyContactListener(Player* player, Scene* scene, b2World* world)
    : player(player), scene(scene), world(world){
    // Khởi tạo thêm nếu cần
}

MyContactListener::MyContactListener(Player* playerIn) :player(player) {

}