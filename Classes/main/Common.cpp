#include "audio/include/AudioEngine.h"
#include "main/Common.h"
#include "scene/PauseMenu.h"
#include <iostream>
#include <fstream>

namespace common {
    Animation* Common::createAnimation(string prefixName, int pFramesOrder, float delay) {

        Vector<SpriteFrame*> animFrames;
        for (int i = 0;i <= pFramesOrder; i++) {
            char buffer[20] = { 0 };

            sprintf(buffer, "%d.png", i);

            string str = prefixName + buffer;//ten anh trong sprite sheet

            // Tao frame
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, delay);
        return animation;
    }

    // Hàm để lấy tất cả các b2Body từ thế giới Box2D
    void Common::updatePosition(b2World* world, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
        // Cập nhật sprite dựa trên body
        for (auto& pair : (*_bodyToSpriteMap)) {
            b2Body* body = pair.first;
            Sprite* sprite = pair.second;

            // Lấy vị trí và góc từ body
            b2Vec2 position = body->GetPosition();
            float angle = body->GetAngle();

            // Cập nhật sprite
            sprite->setPosition(position.x*Constants::PIXELS_PER_METER, position.y * Constants::PIXELS_PER_METER);
            sprite->setRotation(angle * (180.0f / b2_pi)); // Chuyển đổi radian sang độ
        }
    }

    void Common::destroyObject(b2World* world, b2Body* body, unordered_map<b2Body*, Sprite*>& _bodyToSpriteMap) {
        Sprite* sprite = static_cast<Sprite*>(body->GetUserData());
        if (sprite) {
            sprite->removeFromParentAndCleanup(true);
            body->SetUserData(nullptr);
        }
        _bodyToSpriteMap.erase(body); // Xóa body khỏi ánh xạ
        // Xóa body khỏi thế giới Box2D
        world->DestroyBody(body);
    }

    int Common::randomNum(int from, int to) {
        // Khởi tạo seed cho bộ sinh số ngẫu nhiên
        std::random_device rd;  // Thiết bị ngẫu nhiên để tạo seed
        std::mt19937 gen(rd());  // Sử dụng Mersenne Twister
        // Đặt khoảng giá trị ngẫu nhiên (ví dụ từ 1 đến 100)
        std::uniform_int_distribution<> distrib(from, to);

        // Tạo và in ra một số nguyên ngẫu nhiên
        int random_number = distrib(gen);
        return random_number;
    }

    void Common::togglePause(bool& isEnable, Scene* scene) {
        if (scene->getChildByName("Menu") == nullptr) {
            //AudioEngine::stopAll();
            isEnable = false;
            // Lấy Camera
            auto camera = scene->getDefaultCamera();
            Director::getInstance()->pause();
            auto pauseMenu = new PauseMenu(camera->getPosition());
            scene->addChild(pauseMenu->menu, 100); // Đặt z-order cao hơn
        }
        else {
            Director::getInstance()->resume(); // Tiếp tục game
            // Nếu có menu pause, xóa nó
            Setting setting;
            setting.loadSettingData();
            //playBackgroundMusic(setting.getVolume(), "sound/background2.mp3");
            scene->removeChildByName("Menu"); // Xóa menu pause
            if (scene->getChildByName("shop") == nullptr) isEnable = true;
        }
    }
    void Common::zoomAction(MenuItemLabel* item) {
        auto zoomIn = ScaleBy::create(1.0f, 1.1f);
        auto acSq = Sequence::create(zoomIn, zoomIn->reverse(), nullptr);
        auto actionRepeat = RepeatForever::create(acSq);
        item->runAction(actionRepeat);

    }

    void Common::zoomAction(MenuItemImage* item) {
        auto zoomIn = ScaleBy::create(1.0f, 1.1f);
        auto acSq = Sequence::create(zoomIn, zoomIn->reverse(), nullptr);
        auto actionRepeat = RepeatForever::create(acSq);
        item->runAction(actionRepeat);

    }

    void Common::zoomAction(Sprite* item) {
        auto zoomIn = ScaleBy::create(.7f, 1.3f);
        // Tạo hành động xoay
        auto acSq = Sequence::create(zoomIn, zoomIn->reverse(), nullptr);
        auto actionRepeat = RepeatForever::create(acSq);
        item->runAction(actionRepeat);
    }

    int Common::playBackgroundMusic(float volume, string path) {
        cocos2d::AudioEngine::stopAll();
        int bgMusicId = AudioEngine::play2d(path, true); // true để lặp lại
        AudioEngine::setVolume(bgMusicId, volume);
        return bgMusicId;
    }
    
    void Common::changeSpriteColor(Sprite* sprite, bool& isHit) {
        isHit = true;
        sprite->setColor(Color3B(255, 0, 0));
        // Đặt lại màu sắc sau một khoảng thời gian
        auto delay = DelayTime::create(0.1f); // 0.1 giây
        auto resetColor = CallFunc::create([sprite, &isHit]() {
            sprite->setColor(Color3B(255, 255, 255)); // Đặt lại màu về trắng
            isHit = false;
            });

        // Tạo một chuỗi hành động
        auto sequence = Sequence::create(delay, resetColor, nullptr);
        sprite->runAction(sequence);
    }
    void Common::showBossText(Scene* scene, string text) {
        Size screenSize = Director::getInstance()->getVisibleSize();
        auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 60);
        label->setPosition(scene->getDefaultCamera()->getPosition());
        

        label->setOpacity(0);
        scene->addChild(label,2);
        auto fadeIn = FadeIn::create(3.0f); // 2 giây cho hiệu ứng fade in
        auto fadeOut = FadeOut::create(3.0f); // 5 giây cho hiệu ứng fade out
        auto sequence = Sequence::create(fadeIn, fadeOut, nullptr);
        label->runAction(sequence);
    }

    void Common::saveRound(int round) {
        std::ofstream outFile("round.txt");
        if (outFile.is_open()) {
            outFile << round << "\n"; 
            outFile.close();
            std::cout << "Data saved successfully." << std::endl;
        }
        else {
            std::cerr << "Unable to open file for writing." << std::endl;
        }
    }

    int Common::loadRound() {
        std::ifstream inFile("round.txt");
        int round = 1;
        if (inFile.is_open()) {
            inFile >> round; // Đọc máu  
        }
        return round;
    }

    Vec2 Common::scalePos(Vec2 pos) {
        return Vec2(pos.x * scaleSizeX(), pos.y * scaleSizeY());
    }

    void Common::checkAndRemoveSprite(cocos2d::Sprite* sprite, cocos2d::Camera* camera) {
        // Lấy kích thước màn hình
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        // Kiểm tra xem sprite có nằm trong tầm nhìn không
        auto spritePosition = sprite->getPosition();

        if (spritePosition.x < origin.x ||
            spritePosition.x > origin.x + visibleSize.width ||
            spritePosition.y < origin.y ||
            spritePosition.y > origin.y + visibleSize.height) {
            // Nếu không còn trong tầm nhìn, hủy sprite
            sprite->removeFromParentAndCleanup(true);
        }
    }

    float Common::scaleSizeX() {
        // Lấy kích thước màn hình
        return cocos2d::Director::getInstance()->getVisibleSize().width/1920;   
    }
    float Common::scaleSizeY() {
        // Lấy kích thước màn hình
        return cocos2d::Director::getInstance()->getVisibleSize().height / 1080;
    }
    float Common::scaleSizeXY() {
        // Tính toán tỷ lệ
        auto screenSize = Director::getInstance()->getVisibleSize();
        float scaleX = screenSize.width / 1920;
        float scaleY = screenSize.height / 1080;
        return std::min(scaleX, scaleY);
    }

    b2Body* Common::createBoundary(b2World* world, bool isFirst) {
        Size screenSize = Director::getInstance()->getVisibleSize();
        float screenWidth = screenSize.width;
        float screenHeight = screenSize.height;
        b2Body* boundaryBody;
        b2BodyDef boundaryBodyDef;
        boundaryBodyDef.type = b2_kinematicBody;
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        if (!isFirst)
            boundaryBodyDef.position.Set((origin.x+screenWidth + 140) / Constants::PIXELS_PER_METER, 0.0f);
        else
            boundaryBodyDef.position.Set((origin.x- 140) / Constants::PIXELS_PER_METER, 0.0f);
        boundaryBody = world->CreateBody(&boundaryBodyDef);

        // Tạo fixture definition
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = Constants::CATEGORY_BLOCK;
        fixtureDef.filter.maskBits = Constants::CATEGORY_ARROW | Constants::CATEGORY_STICK;

        // Tạo các hình chữ nhật cho các bức tường
        b2PolygonShape boundaryShape;


        // Bức tường phải
        Sprite* sprite = new Sprite();
        sprite->setTag(Constants::TAG_WALL);
        boundaryShape.SetAsBox(1.0f, screenHeight / 2.0f);
        fixtureDef.shape = &boundaryShape;
        boundaryBody->CreateFixture(&fixtureDef);
        boundaryBody->SetUserData(sprite);
        return boundaryBody;
    }
    Vec2 Common::getCenter() {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        return Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    }

    bool Common::isCollision(b2Body* body, int compareTag) {
        if (body && body->GetContactList() != nullptr) {

            // body là con trỏ đến b2Body đã được tạo
            b2ContactEdge* contactEdge = body->GetContactList();  // Lấy danh sách va chạm

            // Duyệt qua các va chạm
            while (contactEdge != nullptr)
            {
                b2Contact* contact = contactEdge->contact;  // Lấy đối tượng va chạm
                b2Body* otherBody = contactEdge->other;    // Lấy b2Body đối diện trong va chạm
                Sprite* sprite = static_cast<Sprite*>(otherBody->GetUserData());
                int tag = sprite->getTag();
                if (tag == compareTag) return true;

                // Tiếp tục duyệt qua các va chạm khác (nếu có)
                contactEdge = contactEdge->next;
            }
        }
        return false;
    }

    void Common::spawnGem(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int num) {
        int x = 0;
        if (num > 1) {
            x = -(num / 2)*30* Common::scaleSizeXY();
        }
        for (int i = 0; i < num; i++) {
            Gem* gem = new Gem(world, scene, Vec2(position.x +x, position.y), bodyToSpriteMap);
            gem->init();
            b2Vec2 velocity(0, 50 * Common::scaleSizeXY());
            float newAngle = 0.0f;
            gem->getBody()->SetLinearVelocity(velocity);
            x += 30 * Common::scaleSizeXY();
        }
    }
}



