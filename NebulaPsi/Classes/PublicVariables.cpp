#include "PublicVariables.h"
#include "Params.h"

PublicVariables* PublicVariables::m_pInstance = nullptr;

PublicVariables::PublicVariables() {
    sfx = true;
    music = true;
    assistant = true;
    gyro = false;
    firstTutorial = true;
    highScore = 0;
}

PublicVariables* PublicVariables::Instance() {
    if (!m_pInstance)
        m_pInstance = new PublicVariables;

    return m_pInstance;
}

void PublicVariables::setFirstTutorial(bool _tut) {
    firstTutorial = _tut;
}

bool PublicVariables::getFirstTutorial() {
    return firstTutorial;
}

void PublicVariables::setSFX(bool _sfx) {
    sfx = _sfx;
}

bool PublicVariables::getSFX() {
    return sfx;
}

void PublicVariables::setMusic(bool _music) {
    music = _music;
}

bool PublicVariables::getMusic() {
    return music;
}

void PublicVariables::setHighScore(int _score) {
    highScore = _score;
}

int PublicVariables::getHighScore() {
    return highScore;
}

void PublicVariables::setAssistant(bool _assistant) {
    assistant = _assistant;
}

bool PublicVariables::getAssistant() {
    return assistant;
}

void PublicVariables::setGyroscope(bool _gyro) {
    gyro = _gyro;
}

bool PublicVariables::getGyroscope() {
    return gyro;
}

// Configuration variables
float PublicVariables::getEnemySpeed() {
    return enemy_speed;
}

float PublicVariables::getMissileSpeed() {
    return rocket_speed;
}

float PublicVariables::getPlayerSpeed() {
    return player_speed;
}

float PublicVariables::getBackgroundSpeed() {
    return background_speed;
}

float PublicVariables::getAsteroidBGSpeed() {
    return asteroid_bg_speed;
}

float PublicVariables::getAsteroidSpeed() {
    return asteroid_speed;
}

float PublicVariables::getStorageBoxSpeed() {
    return storage_box_speed;
}

float PublicVariables::getEnemyTimer() {
    return enemy_timer;
}

float PublicVariables::getEnemyMissileTimer() {
    return enemy_rocket_timer;
}

float PublicVariables::getAsteroidBGTimer() {
    return asteroid_bg_timer;
}

float PublicVariables::getPlayerMissileTimer() {
    return player_rocket_timer;
}

float PublicVariables::getUpShieldTimer() {
    return up_shield_timer;
}

float PublicVariables::getDownShieldTimer() {
    return down_shield_timer;
}

float PublicVariables::getUpOverheatTimer() {
    return up_overheat_timer;
}

float PublicVariables::getDownOverheatTimer() {
    return down_overheat_timer;
}

float PublicVariables::getPlasmaPercent() {
    return plasma_percent;
}

int PublicVariables::getStartRush() {
    return start_rush;
}

float PublicVariables::getAutoCloseAssistant() {
    return auto_close_assistant;
}

float PublicVariables::getEnemyMissileDamage() {
    return enemy_rocket_damage;
}

float PublicVariables::getShieldProtection() {
    return shield_protection;
}

float PublicVariables::getAdTime() {
    return adware;
}

string *PublicVariables::getDialogs() {
    return dialogs;
}

int PublicVariables::tutorDialogCount() {
    return int(sizeof (dialogs) / sizeof (*dialogs)) - 2;
}














