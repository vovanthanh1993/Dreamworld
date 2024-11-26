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
        if (tagA == Constants::TAG_STICK) {
            bodiesToRemove.insert(bodyA);
            if (tagB != Constants::TAG_WALL && tagB != Constants::TAG_BRIDGE_BREAK) {
                player->addMana(1);
            }
            if (tagB == Constants::TAG_WAR) static_cast<Warrior*>(spriteB->getUserData())->die();
            else if (tagB == Constants::TAG_ACHER) static_cast<Acher*>(spriteB->getUserData())->die();
            else if (tagB == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            else if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
            
        }
        if (tagB == Constants::TAG_STICK) {
            bodiesToRemove.insert(bodyB);
            if (tagA != Constants::TAG_WALL && tagA!=Constants::TAG_BRIDGE_BREAK) {
                player->addMana(1);
            }
            if (tagA == Constants::TAG_WAR) static_cast<Warrior*>(spriteA->getUserData())->die();
            else if (tagA == Constants::TAG_ACHER) static_cast<Acher*>(spriteA->getUserData())->die();
            else if (tagA == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            else if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }

        // Huy ban sao
        if (tagA == Constants::TAG_EAGLE) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_WAR) static_cast<Warrior*>(spriteB->getUserData())->die();
            else if (tagB == Constants::TAG_ACHER) static_cast<Acher*>(spriteB->getUserData())->die();
            else if (tagB == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            else if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);

        }
        if (tagB == Constants::TAG_EAGLE) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_WAR) static_cast<Warrior*>(spriteA->getUserData())->die();
            else if (tagA == Constants::TAG_ACHER) static_cast<Acher*>(spriteA->getUserData())->die();
            else if (tagA == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            else if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }

        // chem trung ke dich
        if (tagA == Constants::TAG_SLASH || tagB == Constants::TAG_SLASH) {
            player->addMana(1);
            if (tagA == Constants::TAG_WAR) static_cast<Warrior*>(spriteA->getUserData())->die();
            else if (tagA == Constants::TAG_ACHER) static_cast<Acher*>(spriteA->getUserData())->die();
            else if (tagB == Constants::TAG_WAR) static_cast<Warrior*>(spriteB->getUserData())->die();
            else if (tagB == Constants::TAG_ACHER) static_cast<Acher*>(spriteB->getUserData())->die();

            if (tagA == Constants::TAG_BOSSMAP1 || tagB == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            if (tagA == Constants::TAG_BOSSMAP2 || tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }

        // Huy arrow, xu ly phong trung player
        if (tagA == Constants::TAG_ARROW) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_PLAYER) {
                player->getDamage(1);
               
            }
        }
        if (tagB == Constants::TAG_ARROW) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_PLAYER) {
                player->getDamage(1);
                
            }
        }

        // Huy arrow, xu ly phong trung ke dich
        if (tagA == Constants::TAG_SKULL) {
            if (tagB == Constants::TAG_PLAYER) {
                bodiesToRemove.insert(bodyA);
                player->getDamage(3);
            }
        }
        if (tagB == Constants::TAG_SKULL) {
            if (tagA == Constants::TAG_PLAYER) {
                bodiesToRemove.insert(bodyB);
                player->getDamage(3);
            }
        }

        // Huy rain
        if (tagA == Constants::TAG_FIRE|| tagA == Constants::TAG_RAIN || tagA == Constants::Constants::TAG_BONE_RAIN || tagA == Constants::TAG_FIRE_RAIN) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_PLAYER) {
                player->getDamage(2);
            }
        }
        if (tagB == Constants::TAG_FIRE|| tagB == Constants::TAG_RAIN|| tagB == Constants::Constants::TAG_BONE_RAIN || tagB == Constants::TAG_FIRE_RAIN) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_PLAYER) {
                player->getDamage(2);  
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

        // chem trung ke dich
        if (tagA == Constants::TAG_SLASH_ENEMY && tagB == Constants::TAG_PLAYER || tagB == Constants::TAG_SLASH_ENEMY && tagA == Constants::TAG_PLAYER) {
            player->getDamage(1);
           
        }

        // check enemy and limit
        if (tagB == Constants::TAG_WAR && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            Warrior* w = static_cast<Warrior*>(spriteB->getUserData());
            w->direction *= -1;
            bodyB->SetLinearVelocity(b2Vec2(w->direction * -Constants::SPEED_ENEMY * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(w->direction * Constants::WARRIOR_SCALE * Common::scaleSizeXY());
            
        }

        // check npc and limit
        if (tagB == Constants::TAG_NPC && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            int* storedData = static_cast<int*>(spriteB->getUserData());
            *storedData = -*storedData;
            bodyB->SetLinearVelocity(b2Vec2(*storedData * -Constants::SPEED_ENEMY* Common::scaleSizeXY(), 0));
            Common::scaleAll(spriteB, 0.03);
            spriteB->setScaleX(*storedData * spriteB->getScale());
            spriteB->setUserData(storedData);
        }

        // check boss and limit
        if (tagB == Constants::TAG_BOSSMAP1 && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            int* storedData = static_cast<int*>(spriteB->getUserData());
            *storedData = -*storedData;
            bodyB->SetLinearVelocity(b2Vec2(*storedData * -Constants::SPEED_BOSS1 * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(*storedData * Constants::BOSSMAP1_SCALE * Common::scaleSizeXY());
            spriteB->setUserData(storedData);
        }

        // check boss and limit
        if (tagB == Constants::TAG_BOSSMAP2 && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            int* storedData = static_cast<int*>(spriteB->getUserData());
            *storedData = -*storedData;
            bodyB->SetLinearVelocity(b2Vec2(*storedData * -Constants::SPEED_BOSS2 * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(*storedData * Constants::BOSSMAP2_SCALE * Common::scaleSizeXY());
            spriteB->setUserData(storedData);
        }


        // check enemy and limit
        if (tagB == Constants::TAG_BRIDGE && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            int* storedData = static_cast<int*>(spriteB->getUserData());
            *storedData = -*storedData;
            bodyB->SetLinearVelocity(b2Vec2(0, *storedData * -Constants::SPEED_BRIDGE * Common::scaleSizeXY()));
            spriteB->setUserData(storedData);
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
        }

        // Qua man 2
        if (tagA == Constants::TAG_PORT && tagB == Constants::TAG_PLAYER || tagA == Constants::TAG_PLAYER && tagB == Constants::TAG_PORT) {
            // Khởi tạo scene mới
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
            if (Constants::TAG_STICK == tag) {
                Effect::destroyStick(world, scene, sprite->getPosition());
            } else if (Constants::TAG_EAGLE == tag) {
                Effect::smoke(world, scene, pos);
            }
            else if (Constants::TAG_ARROW == tag) {
                Effect::destroyArrow(world, scene, sprite->getPosition());
            } else if (Constants::TAG_FIRE == tag) {
                Effect::destroyFireRain(world, scene, Vec2(sprite->getPositionX() + sprite->getScaleX() * sprite->getContentSize().width / 2, sprite->getPositionY()));
            }
            else if (Constants::TAG_CHEST == tag) {
                Common::spawnGem(world, scene, pos, bodyToSpriteMap, Common::randomNum(3, 10));
                Effect::chest();
            }
            else if (Constants::TAG_RAIN == tag) {
                Effect::destroyRain(world, scene, pos);
            }
            else if (Constants::TAG_BONE_RAIN == tag) {
                Effect::smoke(world, scene, pos);
            }
            else if (Constants::TAG_FIRE_RAIN == tag) {
                Effect::destroyFireRain(world, scene, pos);
            }
            else if (Constants::TAG_SKULL == tag) {
                Effect::smoke(world, scene, pos);
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