#ifndef __NEBULAGAME_H__
#define __NEBULAGAME_H__

#pragma once

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"
#include "HeadLayer.h"
#include "ControlLayer.h"
#include "StorageBox.h"
#include "Assistant.h"
#include "Explosions.h"

using namespace cocos2d;

enum explosionType { FMISSILE, PMISSILE, SPACESHIP };
enum rocketType { FIREROCKET, PLASMAROCKET };
enum assistantMode { LEARN, ATTACK, SCORE, ADWARE };

class NebulaGame: public Scene {
    public:
        static Scene *createScene();
        virtual bool init();
        virtual void update(float dt);
        virtual void onTouchesMoved(Touch* touch, Event* event);
        virtual bool onTouchesBegan(Touch* touch, Event* event);
        virtual void onTouchesEnded(Touch* touch, Event* event);
        void onAcceleration(Acceleration* acc, Event* event);
        void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
        void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
        void updateMusic();
        void pauseGame();
        void resumeGame();
        void createPlayerRocket(int _rtype);
        void ShowMyHUD();
        void HideMyHUD();
        void scheduleFireRocket();
        void schedulePlasmaRocket();
        void unscheduleFireRocket();
        void unschedulePlasmaRocket();
        void callFireRocket(float dt);
        void callPlasmaRocket(float dt);
        void startShield();
        void DeleteAsteroid(Asteroid *_aster);
        void stopAllSounds();
        void callMenuGame();

        SpriteFrameCache * spritecache;
        Player *player;
        bool pauseMode;

    private:        
        int score;
        string *mdialogs;
        int nowDialog;
        bool isHiScore;
        bool rush;
        bool isGyro;
        HeadLayer *headLayer;
        ControlLayer *controlLayer;
        GameOver *gameOverHUD;
        BackgroundLayer *backgroundLayer;
        Assistant *assistant;
        Size visibleSize;
        Vector<Enemy *> enemies;       
        Vector<Weapon *> enemyWeapons;
        Vector<Weapon *> playerWeapons;
        Vector<StorageBox *> storageboxes;
        Vector<Asteroid*> asteroids;
        Vector<Enemy *> recyclebinEnemies;
        Vector<Weapon *> recyclebinEnemyMissiles;
        Vector<Weapon *> recyclebinPlayerMissiles;
        Vector<StorageBox *> recyclebinStorageBoxes;
        bool isFireRocketSchedule;
        bool isDownOverheatSchedule;
        bool isUpOverheatSchedule;
        bool nowAssistant;

        bool checkCollision(Sprite *boxA, Sprite *boxB);
        void randomEnemySpawn(float dt);
        void createAsteroid();
        void createEnemy();
        void createEnemyRocket(float dt);
        void createStorageBox(int boxType, Vec2 _pos);
        void createExplosion(Vec2 _pos, explosionType);
        void ShowGameOver(float dt);
        void downShield(float dt);
        void upShield(float dt);
        void downOverheat(float dt);
        void upOverheat(float dt);
        void AsteroidMonitorning(float dt);
        void EnemydMonitorning(float dt);
        void StorageBoxMonitoring(float dt);
        void EnemyMissileMonitoring(float dt);
        void PlayerMissileMonitoring(float dt);
        void touchDialogAssistant();
        void showAssistant(int _mode);
        void showMyAssistant(string _text, int _mode, bool _autoclose);
        void hideAssistant();
        void lineOfEnemies();
        void callAssistant(Ref *pSender);
        void callCloseAssistant(float dt);
        void callUnPause(Ref *pSender);
        void DeadPlayer();
        void callAdWare(float dt);
    
    CREATE_FUNC(NebulaGame)
};

#endif // __NEBULAGAME_H__
