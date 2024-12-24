#include "MusicManager.h"

MusicManager* MusicManager::instance = nullptr;

// Constructor
MusicManager::MusicManager() : currentMusicId(-1) {}

// Singleton instance
MusicManager* MusicManager::getInstance() {
    if (!instance) {
        instance = new MusicManager();
    }
    return instance;
}

// Phát nhạc nền
void MusicManager::playBackgroundMusic(const std::string& filePath, bool loop) {
    if (currentMusicId != -1) {
        AudioEngine::stop(currentMusicId);
    }
    currentMusicId = AudioEngine::play2d(filePath, loop);
}

// Dừng nhạc nền
void MusicManager::stopBackgroundMusic() {
    if (currentMusicId != -1) {
        AudioEngine::stop(currentMusicId);
        currentMusicId = -1;
    }
}

// Thiết lập âm lượng cho nhạc nền
void MusicManager::setVolume(float volume) {
    if (currentMusicId != -1) {
        AudioEngine::setVolume(currentMusicId, volume);
    }
}

// Phát effect âm thanh
int MusicManager::playEffect(const std::string& filePath, bool loop) {
    int soundId = AudioEngine::play2d(filePath, loop);
    soundEffectIds[filePath] = soundId;
    return soundId;
}

// Dừng một effect
void MusicManager::stopEffect(int soundId) {
    AudioEngine::stop(soundId);
}

// Dừng tất cả các effect
void MusicManager::stopAllEffects() {
    for (const auto& effect : soundEffectIds) {
        AudioEngine::stop(effect.second);
    }
    soundEffectIds.clear();
}

// Thiết lập âm lượng cho một effect
void MusicManager::setEffectVolume(int soundId, float volume) {
    AudioEngine::setVolume(soundId, volume);
}

// Giải phóng tài nguyên
void MusicManager::release() {
    AudioEngine::end();
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}


// Sound
void MusicManager::soundTing() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/ting.mp3"), SettingManager::getInstance()->getGameplayVol());

}

void MusicManager::enemyDie() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/warrior/die.mp3"), SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::enemyHurt() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/enemyhurt.mp3"), SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::playerJump() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/jump.mp3"), SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::playerSlash() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/slash.mp3"), SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::chest() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/chest.mp3"), SettingManager::getInstance()->getGameplayVol());

}

void MusicManager::soundHealth() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/health.mp3"), 0.5 * SettingManager::getInstance()->getGameplayVol());
}
void MusicManager::soundError() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/error.mp3"), 0.5 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::soundLaughter() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap1/sound/laughter.mp3"), 0.08 * SettingManager::getInstance()->getGameplayVol());
}
void MusicManager::soundCreepyLaughter() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap2/sound/creepy-laughter.mp3"), 2 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::soundBoss1Hurt() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap1/sound/boss1hurt.mp3"), 0.5 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::soundBoss3Hurt() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap3/sound/hurt.mp3"), 0.5 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::soundMagicFire() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap1/sound/magic-fire.mp3"), 0.5 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::soundDarkMagic() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap2/sound/dark-magic.mp3"), 1 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::playerHurt() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/hurt.mp3"), 0.1 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::eagle() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/eagle.mp3"), 0.2 * SettingManager::getInstance()->getGameplayVol());
}

void MusicManager::getStick() {
    SettingManager::getInstance()->loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/get_stick.mp3"), 2 * SettingManager::getInstance()->getGameplayVol());
}
