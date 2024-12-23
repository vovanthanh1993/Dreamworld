#pragma once

#include "audio/include/AudioEngine.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "gui/Setting.h"
class MusicManager {
private:
    static MusicManager* instance;            // Singleton instance
    unordered_map<std::string, int> soundEffectIds; // Lưu các effect đang phát

    // Constructor private để đảm bảo singleton
    MusicManager();

public:
    int currentMusicId;

    // Lấy instance của singleton
    static MusicManager* getInstance();

    // Quản lý nhạc nền
    void playBackgroundMusic(const std::string& filePath, bool loop = true);
    void stopBackgroundMusic();
    void setVolume(float volume);

    // Quản lý effect âm thanh
    int playEffect(const std::string& filePath, bool loop = false);
    void stopEffect(int soundId);
    void stopAllEffects();
    void setEffectVolume(int soundId, float volume);

    // Giải phóng tài nguyên
    void release();

    static void soundTing();
    static void enemyDie();
    static void playerJump();
    static void playerSlash();
    static void  chest();
    static void soundHealth();
    static void soundError();
    static void soundLaughter();
    static void soundCreepyLaughter();
    static void soundBoss1Hurt();
    static void soundMagicFire();
    static void soundDarkMagic();
    static void soundBoss3Hurt();
    static void enemyHurt();
    static void playerHurt();
};
