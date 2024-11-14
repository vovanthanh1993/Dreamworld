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

        // Tag BOx
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
            if (tagB == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyB);
            if (tagB == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
            
        }
        if (tagB == Constants::TAG_STICK) {
            if (tagA != Constants::TAG_WALL && tagA!=Constants::TAG_BRIDGE_BREAK) {
                player->addMana(1);
            }
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyA);
            if (tagA == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }

        // Huy ban sao
        if (tagA == Constants::TAG_COPY) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyB);
            if (tagB == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);

        }
        if (tagB == Constants::TAG_COPY) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyA);
            if (tagA == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }

        // Huy cloud
        if (tagA == Constants::TAG_CLOUD) {
            bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyB);
            
            if (tagB == Constants::TAG_BOSSMAP1) {
                bossmap1->updateHealth(1);
            } 
            if (tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);

        }
        if (tagB == Constants::TAG_CLOUD) {
            bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyA);
           
           
            if (tagA == Constants::TAG_BOSSMAP1) {
                bossmap1->updateHealth(1);
            }
            if (tagA == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }


        // chem trung ke dich
        if (tagA == Constants::TAG_SLASH || tagB == Constants::TAG_SLASH) {
            player->addMana(1);
            if (tagA == Constants::TAG_ENEMY)  bodiesToRemove.insert(bodyA);
            if (tagB == Constants::TAG_ENEMY) bodiesToRemove.insert(bodyB);
            if (tagA == Constants::TAG_BOSSMAP1 || tagB == Constants::TAG_BOSSMAP1) bossmap1->updateHealth(1);
            if (tagA == Constants::TAG_BOSSMAP2 || tagB == Constants::TAG_BOSSMAP2) bossmap2->updateHealth(1);
        }

        // Huy arrow, xu ly phong trung ke dich
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
        if (tagB == Constants::TAG_ENEMY && tagA == Constants::TAG_LIMIT) {
            // Lấy vận tốc hiện tại của đối tượng
            int* storedData = static_cast<int*>(spriteB->getUserData());
            *storedData = -*storedData;
            bodyB->SetLinearVelocity(b2Vec2(*storedData * -Constants::SPEED_ENEMY * Common::scaleSizeXY(), 0));
            spriteB->setScaleX(*storedData * Constants::WARRIOR_SCALE * Common::scaleSizeXY());
            spriteB->setUserData(storedData);
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
        if (tagA == Constants::TAG_PORT && tagB == Constants::TAG_PLAYER || tagA == Constants::TAG_PLAYER&& tagB == Constants::TAG_PORT) {
            // Khởi tạo scene mới
            isNext = true;
        }
    }

}

// Phương thức để xóa đối tượng
void MyContactListener::removeObject() {
    for (b2Body* body : bodiesToRemove) {
        if (body) {
            
            int check = 0;
            // Xoa khoi danh sach warrior
            for (auto it = warriorVector->begin(); it != warriorVector->end(); ) {
                if ((*it)->getBody() == body) {
                    
                    (*it)->die();
                    //delete* it;
                    it = warriorVector->erase(it); // Loại bỏ sprite khỏi danh sách
                    check = 1;
                }
                else {
                    ++it;
                }
            }
            // Xoa khoi danh sach acher
            for (auto it = acherVector->begin(); it != acherVector->end(); ) {
                if ((*it)->getBody() == body) {
                    (*it)->die();
                    // Xóa sprite khỏi danh sách và bộ nhớ
                    //delete* it;
                    it = acherVector->erase(it); // Loại bỏ sprite khỏi danh sách
                    check = 1;
                }
                else {
                    ++it;
                }
            }

            if (check == 0) {
                // Xóa dữ liệu người dùng (Sprite) khỏi hệ thống đồ họa
                Sprite* sprite = static_cast<Sprite*>(body->GetUserData());
                if (Constants::TAG_STICK == sprite->getTag()) {
                    Effect::destroyStick(world, scene, sprite->getPosition());
                } else if (Constants::TAG_CLOUD == sprite->getTag()) {
                    Effect::smoke(world, scene, sprite->getPosition());
                }
                else if (Constants::TAG_ARROW == sprite->getTag()) {
                    Effect::destroyArrow(world, scene, sprite->getPosition());
                } else if (Constants::TAG_FIRE == sprite->getTag()) {
                   // Effect::destroyFire(world, scene, Vec2(sprite->getPositionX() + sprite->getScaleX() * sprite->getContentSize().width/2, sprite->getPositionY()));
                    Effect::destroyFireRain(world, scene, Vec2(sprite->getPositionX() + sprite->getScaleX() * sprite->getContentSize().width / 2, sprite->getPositionY()));
                }
                else if (Constants::TAG_CHEST == sprite->getTag()) {
                    Gem* gem = new Gem();
                    gem->init(world, scene, sprite->getPosition(), _bodyToSpriteMap, Common::randomNum(3,10));
                    Effect::chest();
                }
                else if (Constants::TAG_RAIN == sprite->getTag()) {
                    Effect::destroyRain(world, scene, sprite->getPosition());
                }
                else if (Constants::TAG_BONE_RAIN == sprite->getTag()) {
                    Effect::smoke(world, scene, sprite->getPosition());
                }
                else if (Constants::TAG_FIRE_RAIN == sprite->getTag()) {
                    Effect::destroyFireRain(world, scene, sprite->getPosition());
                }
                else if (Constants::TAG_SKULL == sprite->getTag()) {
                    Effect::smoke(world, scene, sprite->getPosition());
                }

                if (sprite) {
                    sprite->removeFromParentAndCleanup(true);
                    body->SetUserData(nullptr);
                }
                (*_bodyToSpriteMap).erase(body); // Xóa body khỏi ánh xạ
                // Xóa body khỏi thế giới Box2D
                world->DestroyBody(body);
                
            }
        }
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