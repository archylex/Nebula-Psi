#pragma once

#include <iostream>

enum SCENE { MAINMENU, GAME };

using namespace std;

class PublicVariables {
    public:        
        static PublicVariables* Instance();
        void setFirstTutorial(bool _tut);
        bool getFirstTutorial();
        void setHighScore(int _score);
        int getHighScore();
        void setSFX(bool _sfx);
        bool getSFX();
        void setMusic(bool _music);
        bool getMusic();
        void setAssistant(bool _assistant);
        bool getAssistant();
        void setGyroscope(bool _gyro);
        bool getGyroscope();
        float getEnemySpeed();
        float getPlayerSpeed();
        float getMissileSpeed();
        float getBackgroundSpeed();
        float getAsteroidBGSpeed();
        float getAsteroidSpeed();
        float getStorageBoxSpeed();
        float getEnemyTimer();
        float getEnemyMissileTimer();
        float getAsteroidBGTimer();
        float getPlayerMissileTimer();
        float getUpShieldTimer();
        float getDownShieldTimer();
        float getUpOverheatTimer();
        float getDownOverheatTimer();
        float getPlasmaPercent();
        int getStartRush();
        float getAutoCloseAssistant();
        float getEnemyMissileDamage();
        float getShieldProtection();
        float getAdTime();
        string *getDialogs();
        int tutorDialogCount();

    private:
        PublicVariables();        
        static PublicVariables* m_pInstance;
        int highScore;
        bool sfx;
        bool music;
        bool assistant;
        bool gyro;
        bool firstTutorial;
};
