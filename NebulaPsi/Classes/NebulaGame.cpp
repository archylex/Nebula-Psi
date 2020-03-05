#include "NebulaGame.h"
#include "SimpleAudioEngine.h"
#include <unistd.h>
#include <ctime>
#include "PublicVariables.h"
#include "PauseLayer.h"
#include "PsiToast.h"
#include "Gyroscope.h"
#include "CheckNet.h"

USING_NS_CC;

Scene* NebulaGame::createScene() {
    return NebulaGame::create();
}

bool NebulaGame::init() {
    if(!Scene::init())
        return false;

    // Pause game
    pauseMode = false;

    // High score
    isHiScore = false;

    // Gyroscope device
    isGyro = false;

    // Score
    score = 0;

    // What assistant saying
    nowDialog = 0;

    // Get dialogs
    mdialogs = PublicVariables::Instance()->getDialogs();

    // Screen size
    visibleSize = Director::getInstance()->getVisibleSize();

    // Background layer
    backgroundLayer = new BackgroundLayer();
    this->addChild(backgroundLayer, 0);

    // Player spaceship
    player = Player::create("PlayerShip.png");
    this->addChild(player, 2);

    // Head layer
    headLayer = new HeadLayer();
    headLayer->ResetMe();
    headLayer->UpdateHealth(player->getHealth());
    headLayer->UpdateEnergy(player->getEnergy());
    headLayer->UpdateOverheat(player->getOverheat());
    headLayer->UpdatePlasma(player->getPlasmaRocket());
    this->addChild(headLayer, 20);

    // Control panel layer
    controlLayer = new ControlLayer();
    controlLayer->ResetMe();
    this->addChild(controlLayer, 20);

    // Show HUD animation
    ShowMyHUD();

    // Update scene
    this->scheduleUpdate();

    // Start enemy spawn
    this->schedule(schedule_selector(NebulaGame::randomEnemySpawn), PublicVariables::Instance()->getEnemyTimer());

    // Start enemy missiles
    this->schedule(schedule_selector(NebulaGame::createEnemyRocket), PublicVariables::Instance()->getEnemyMissileTimer());

    // Check Network
    this->schedule(schedule_selector(NebulaGame::callAdWare), PublicVariables::Instance()->getAdTime());

    // Play music
    updateMusic();

    // Keyboard listener
    auto listenerK = EventListenerKeyboard::create();
    listenerK->onKeyPressed = CC_CALLBACK_2(NebulaGame::onKeyPressed, this);
    listenerK->onKeyReleased = CC_CALLBACK_2(NebulaGame::onKeyReleased, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerK, this);

    // Gyroscope / Accelerometer listener
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (PublicVariables::Instance()->getGyroscope()) {
        if (!Gyroscope::startProcess()) {
            PsiToast::showToast("No gyroscope. Using accelerometer");
            Device::setAccelerometerEnabled(true);
            auto listenerA = EventListenerAcceleration::create(CC_CALLBACK_2(NebulaGame::onAcceleration, this));
            this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerA, this);
        } else {
            isGyro = true;
        }
    }
    #endif

    // Touch listener
    auto listenerT = EventListenerTouchOneByOne::create();
    listenerT->setSwallowTouches(true);
    listenerT->onTouchBegan = CC_CALLBACK_2(NebulaGame::onTouchesBegan, this);
    listenerT->onTouchMoved = CC_CALLBACK_2(NebulaGame::onTouchesMoved, this);
    listenerT->onTouchEnded = CC_CALLBACK_2(NebulaGame::onTouchesEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerT, this);

    // Show tutorial
    if (PublicVariables::Instance()->getAssistant() && PublicVariables::Instance()->getFirstTutorial()) {
        showMyAssistant(mdialogs[0], LEARN, false);    
        PublicVariables::Instance()->setFirstTutorial(false);
    }

    return true;
}

// Show UI with animation
void NebulaGame::ShowMyHUD() {
    sleep(0.5f);
    headLayer->ShowMe();
    controlLayer->ShowMe();
}

// Hide UI with animation
void NebulaGame::HideMyHUD() {
    headLayer->HideMe();
    controlLayer->HideMe();
}

// Control from keyboard
void NebulaGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if(!pauseMode && !player->getDead()) {
        if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) player->setVX(-1);
        if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) player->setVX(1);
        if(keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) player->setVY(1);
        if(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) player->setVY(-1);

        if(keyCode == EventKeyboard::KeyCode::KEY_C)
            scheduleFireRocket();

        if(keyCode == EventKeyboard::KeyCode::KEY_X)
            schedulePlasmaRocket();

        if(keyCode == EventKeyboard::KeyCode::KEY_Z)
            startShield();
    }
}

void NebulaGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    if((keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW  && player->getVX() == -1) ||
            (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW  && player->getVX() == 1))
        player->setVX(0);
    if((keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW  && player->getVY() == 1) ||
            (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW  && player->getVY() == -1))
        player->setVY(0);
    if(keyCode == EventKeyboard::KeyCode::KEY_C) {
        unscheduleFireRocket();
    }

    if(keyCode == EventKeyboard::KeyCode::KEY_X) {
        unschedulePlasmaRocket();
    }

    if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
        callMenuGame();


    if(keyCode == EventKeyboard::KeyCode::KEY_ENTER && nowAssistant)
            touchDialogAssistant();
}


// Accelerometer
void NebulaGame::onAcceleration(Acceleration* acc, Event* event) {
    if (!player->getDead()) {
        player->setVX(float(acc->x));
        player->setVY(float(acc->y));
    }
}

// Touch control
bool NebulaGame::onTouchesBegan(Touch *touch, Event *event) {
    if (!nowAssistant) {
        if (controlLayer->CrossZone(touch->getLocation())) {
            Vec2 gpj = controlLayer->GetPosCross();

            if (!player->getDead()) {
                player->setVX(gpj.x);
                player->setVY(gpj.y);
            }
        }
    } else {
        touchDialogAssistant();
    }

    return true;
}

void NebulaGame::onTouchesEnded(Touch* touch, Event* event) {    
    if (!nowAssistant) {
        controlLayer->ResetPosCross();

        if (!player->getDead()) {
            player->setVX(0);
            player->setVY(0);
        }
    }
}

void NebulaGame::onTouchesMoved(Touch* touch, Event* event) {
    if (controlLayer->CrossZone(touch->getLocation())) {
        controlLayer->SetPosCross(touch->getLocation());
        Vec2 gpj = controlLayer->GetPosCross();

        if (!player->getDead()) {
            player->setVX(gpj.x);
            player->setVY(gpj.y);
        }
    }
}

// Update scene
void NebulaGame::update(float dt) {    
    if(!pauseMode) {
        // Update background layer
        backgroundLayer->update(dt);

        // Update player
        if (!player->getDead())
            player->update(dt);

        // Gyroscope
        #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if (isGyro && !player->getDead()) {
            Vec2 gyroPos = Gyroscope::getCoordinates();

            player->setVX(gyroPos.x/5);
            player->setVY(gyroPos.y/5);
        }
        #endif

        // Rush hour or Shock troops
        if (!nowAssistant && rush && !player->getDead()) {
            rush = false;
            lineOfEnemies();
        }

        // Player missiles life
        PlayerMissileMonitoring(dt);

        // Foreground asteroids life
        AsteroidMonitorning(dt);

        // Enemies life
        EnemydMonitorning(dt);

        // Enemy missiles life
        EnemyMissileMonitoring(dt);

        // Storage boxes life
        StorageBoxMonitoring(dt);
    }
}

// Show assistant layer
void NebulaGame::showMyAssistant(string _text, int _mode, bool _autoclose) {
    assistant = new Assistant();
    this->addChild(assistant, 50);
    assistant->setText(_text);
    assistant->setMode(_mode);

    if (_autoclose) {
        this->schedule(schedule_selector(NebulaGame::callCloseAssistant), PublicVariables::Instance()->getAutoCloseAssistant());
    } else {
            pauseGame();
    }

    nowAssistant = true;
}

// Hide assistant layer
void NebulaGame::hideAssistant() {
    if(pauseMode)
        resumeGame();

    assistant->removeFromParentAndCleanup(true);

    nowAssistant = false;
}

// Touch action by assistant layer
void NebulaGame::touchDialogAssistant() {
    if (nowAssistant) {
        // Tutorial
        if (assistant->getMode() == LEARN) {
            // dialog increment
            nowDialog++;

            if (nowDialog < PublicVariables::Instance()->tutorDialogCount()) {
                // Set assistant text
                assistant->setText(mdialogs[nowDialog]);

                // Lighting and unlighting elements
                switch (nowDialog) {
                    case 3:
                        controlLayer->lightFireRocket(true);
                        break;
                    case 4:
                        controlLayer->lightFireRocket(false);
                        controlLayer->lightPlasmaRocket(true);
                        break;
                    case 5:
                        controlLayer->lightPlasmaRocket(false);
                        controlLayer->lightShield(true);
                        break;
                    case 6:
                        controlLayer->lightShield(false);
                        controlLayer->lightCross(true);
                        break;
                    case 7:
                        controlLayer->lightCross(false);
                        controlLayer->lightMenu(true);
                        break;
                    case 8:
                        controlLayer->lightMenu(false);
                        break;
                    case 9:
                        headLayer->lightHealth();
                        break;
                    case 10:
                        headLayer->lightEnergy();
                        headLayer->UpdateHealth(player->getHealth());
                        break;
                    case 11:
                        headLayer->lightOverheat();
                        headLayer->UpdateEnergy(player->getEnergy());
                        break;
                    case 12:
                        headLayer->lightPlasma();
                        headLayer->UpdateOverheat(player->getOverheat());
                        break;
                    case 13:
                        headLayer->UpdatePlasma(player->getPlasmaRocket());
                        headLayer->lightScore();
                        break;
                    case 14:
                        headLayer->unlightScore();
                        break;
                }
            } else {                
                nowDialog = 0;
                nowAssistant = false;
                hideAssistant();
            }
        }

        // Shock troops
        if(assistant->getMode() == ATTACK) {
            nowAssistant = false;
            hideAssistant();
        }

        // Shock troops
        if(assistant->getMode() == ADWARE) {
            nowAssistant = false;
            hideAssistant();
            this->schedule(schedule_selector(NebulaGame::callAdWare), 10.0f);
        }
    }
}


// couple of enemies
void NebulaGame::lineOfEnemies() {

    pauseGame();

    int numlines = 3;
    string name[] = { "EnemyShip01.png", "EnemyShip02.png", "EnemyShip03.png" };

    for (int a=0; a < numlines; a++) {
        for (int b=0; b < 4; b++) {
            Enemy *_enemy;
            int rc = rand() % 3;

            _enemy = Enemy::create(name[rc]);
            _enemy->setPosition(Vec2( visibleSize.width/2 +  (a+1)*visibleSize.width*sin((270-b*60)*3.14f/180),
                                      visibleSize.height/2 + (a+1)*visibleSize.height*sin((b*60)*3.14f/180) ));

            this->addChild(_enemy, 2);
            enemies.pushBack(_enemy);

            auto anim = MoveTo::create(1.5f, Vec2(visibleSize.width/5 + b*visibleSize.width/5, visibleSize.height*2/3 + a*_enemy->getContentSize().height*1.15f));
            if (a == (numlines - 1) && b == 3) {
                auto callAct = CallFuncN::create(CC_CALLBACK_1(NebulaGame::callUnPause, this));

                if (PublicVariables::Instance()->getAssistant())
                    callAct = CallFuncN::create(CC_CALLBACK_1(NebulaGame::callAssistant, this));

                auto seq = Sequence::create(anim, DelayTime::create(0.5f), callAct, nullptr);
                _enemy->runAction(seq);
            } else {
                _enemy->runAction(anim);
            }
        }
    }
}

// Call assistant by couple of enemies (rush)
void NebulaGame::callAssistant(Ref *pSender) {
    showMyAssistant(mdialogs[PublicVariables::Instance()->tutorDialogCount()], ATTACK, false);
}

// Call resume game by assistant layer
void NebulaGame::callUnPause(Ref *pSender) {
    resumeGame();
}

// Activate shield
void NebulaGame::startShield() {
    if(!player->getShield()) {
        // Play sound
        if (PublicVariables::Instance()->getSFX())
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ShieldOn.mp3");

        if (player->getEnergy() < 100) {
            this->unschedule(schedule_selector(NebulaGame::upShield));
        }

        this->schedule(schedule_selector(NebulaGame::downShield), PublicVariables::Instance()->getDownShieldTimer());
        player->setShield(true);
    }
}

// Decrement energy and deactivate shield
void NebulaGame::downShield(float dt) {
    if (player->getEnergy() > 0) {
        player->setEnergy(player->getEnergy() - 1);
    } else {
        player->setEnergy(0);
        player->setShield(false);
        this->unschedule(schedule_selector(NebulaGame::downShield));
        this->schedule(schedule_selector(NebulaGame::upShield), PublicVariables::Instance()->getUpShieldTimer());

        // Play sound
        if (PublicVariables::Instance()->getSFX())
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ShieldOff.mp3");
    }
    headLayer->UpdateEnergy(player->getEnergy());
}

// Get energy
void NebulaGame::upShield(float dt) {
    if (player->getEnergy() < 100) {
        player->setEnergy(player->getEnergy() + 1);
    } else {
        player->setEnergy(100);
        this->unschedule(schedule_selector(NebulaGame::upShield));
    }
    headLayer->UpdateEnergy(player->getEnergy());
}

// Overheat decrement
void NebulaGame::downOverheat(float dt) {
    if (player->getOverheat() > 0) {
        player->setOverheat(player->getOverheat() - 1);
    } else {
        player->setOverheat(0);
        this->unschedule(schedule_selector(NebulaGame::callFireRocket));
    }

    headLayer->UpdateOverheat(player->getOverheat());
}

// Overheat increment
void NebulaGame::upOverheat(float dt) {
    if (player->getOverheat() < 100) {
        player->setOverheat(player->getOverheat() + 1);
    } else {
        player->setOverheat(100);
        this->unschedule(schedule_selector(NebulaGame::upOverheat));
    }

    headLayer->UpdateOverheat(player->getOverheat());
}


// Game over layer
void NebulaGame::ShowGameOver(float dt) {    
    this->unschedule(schedule_selector(NebulaGame::ShowGameOver));
    this->unschedule(schedule_selector(NebulaGame::callAdWare));
    HideMyHUD();
    gameOverHUD = new GameOver(score);
    this->addChild(gameOverHUD, 50);
}

// auto close assistant
void NebulaGame::callCloseAssistant(float dt) {
    this->unschedule(schedule_selector(NebulaGame::callCloseAssistant));
    hideAssistant();
}

// Delete asteroid after explosion
void NebulaGame::DeleteAsteroid(Asteroid *_aster) {
    auto *target = _aster;
    asteroids.eraseObject(target);
    this->removeChild(target, true);
}

// Dead player
void NebulaGame::DeadPlayer() {
    unscheduleFireRocket();
    unschedulePlasmaRocket();
    this->unschedule(schedule_selector(NebulaGame::upOverheat));
    this->unschedule(schedule_selector(NebulaGame::downOverheat));
    this->unschedule(schedule_selector(NebulaGame::upShield));
    this->unschedule(schedule_selector(NebulaGame::downShield));

    createExplosion(player->getPosition(), SPACESHIP);

    // Hide player spaceship
    player->setVisible(false);
    player->setHealth(0.0f);

    // Game Over layer
    this->schedule(schedule_selector(NebulaGame::ShowGameOver), 1.0f);
}

// Monitoring asteroids
void NebulaGame::AsteroidMonitorning(float dt) {
    // Damage reduction
    float dD = 1.0f;

    // Check shield
    if (player->getShield())
        dD = PublicVariables::Instance()->getShieldProtection();

    // Asteroid collision
    if (asteroids.size() > 0) {
        for (auto &nextAsteroid : asteroids) {
            // Update asteroid
            nextAsteroid->update(dt);

            // Collision asteroid with player
            if (!player->getDead() && !nextAsteroid->getDead() && checkCollision(nextAsteroid, player)) {
                // Show explode animation
                nextAsteroid->showExplode();

                // Calculation of damage relative to size
                if(nextAsteroid->getMyWeight() < 0.8f)
                    player->setHealth(player->getHealth() - (50*nextAsteroid->getMyWeight()/dD));
                else
                    player->setHealth(player->getHealth() - (100/dD));

                // Update health indicator
                headLayer->UpdateHealth(player->getHealth());

                // Check player health
                if(player->getDead())
                    DeadPlayer();
            }

            // Check screen border
            if (!nextAsteroid->getDead())
                if (nextAsteroid->getPositionY() + nextAsteroid->getContentSize().height < 0)
                    DeleteAsteroid(nextAsteroid);
        }
    }
}

// Monitoring enemies
void NebulaGame::EnemydMonitorning(float dt) {
    // Damage reduction
    float dD = 1.0f;

    // Check shield
    if (player->getShield())
        dD = PublicVariables::Instance()->getShieldProtection();

    // Enemy collision
    if (enemies.size() > 0) {
        for (auto &nextEnemy : enemies) {
            // Update enemy spaceship
            nextEnemy->update(dt);

            if (!player->getDead() && checkCollision(nextEnemy, player)) {
                // Move to recycle bin
                recyclebinEnemies.pushBack(nextEnemy);

                // Show explosion
                createExplosion(nextEnemy->getPosition(), SPACESHIP);

                // Calculation of damage
                player->setHealth(player->getHealth() - (100/dD));

                // Update health indicator
                headLayer->UpdateHealth(player->getHealth());

                // Check player health
                if(player->getDead())
                    DeadPlayer();
            }

            // Check screen border
            if (nextEnemy->getPositionY() + nextEnemy->getContentSize().height < 0)
                recyclebinEnemies.pushBack(nextEnemy);            
        }
    }

    // Empty recycle bin
    for (auto &nextEnemy : recyclebinEnemies) {
        auto *target = nextEnemy;
        enemies.eraseObject(target);
        recyclebinEnemies.eraseObject(target);
        this->removeChild(target, true);
    }
}

// Monitoriung storage boxes
void NebulaGame::StorageBoxMonitoring(float dt) {
    // Collision of the player with the box
    if (storageboxes.size() > 0) {
        for (auto &nextSBox : storageboxes) {
            // Update storage box
            nextSBox->update(dt);

            // Check collision with player
            if (!player->getDead() && checkCollision(nextSBox, player)) {
                // Move to recycle bin
                recyclebinStorageBoxes.pushBack(nextSBox);

                if (PublicVariables::Instance()->getSFX())
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Pickup.mp3");

                // Bonus switch
                switch (nextSBox->getBoxType()) {
                    // Repair spaceship
                    case 0:
                        {
                            if (100 < player->getHealth() + 25)
                                player->setHealth(100);
                            else
                                player->setHealth(player->getHealth() + 25);

                            headLayer->UpdateHealth(player->getHealth());
                        };
                        break;

                    // Get plasma missiles
                    case 1:
                        {
                            player->setPlasmaRocket(player->getPlasmaRocket() + PublicVariables::Instance()->getPlasmaPercent());
                            headLayer->UpdatePlasma(player->getPlasmaRocket());
                        }
                        break;
                }
            }

            // Check screen border
            if (nextSBox->getPositionY() + nextSBox->getContentSize().height < 0)
                recyclebinStorageBoxes.pushBack(nextSBox);
        }
    }

    // Empty recycle bin
    for (auto &nextBox : recyclebinStorageBoxes) {
        auto target = nextBox;
        storageboxes.eraseObject(target);
        recyclebinStorageBoxes.eraseObject(target);
        this->removeChild(target, true);
    }
}

// Monitoring enemy missiles
void NebulaGame::EnemyMissileMonitoring(float dt) {
    // Shield -> damage / defend
    float dD = 1.0f;

    // Check shield
    if (player->getShield())
        dD = PublicVariables::Instance()->getShieldProtection();

    // Enemy missiles collision
    if(enemyWeapons.size() > 0) {
        for (auto &nextEnemyMissile : enemyWeapons) {
            // Update enemy missile
            nextEnemyMissile->update(dt);

            // Check collision with player
            if (!player->getDead() && checkCollision(nextEnemyMissile, player) && !nextEnemyMissile->getIsDead()) {
                // isDead
                nextEnemyMissile->setIsDead();

                // Move to recycle bin
                recyclebinEnemyMissiles.pushBack(nextEnemyMissile);

                // Show explosion
                createExplosion(nextEnemyMissile->getPosition(), FMISSILE);

                // Update player health
                player->setHealth(player->getHealth() - int(PublicVariables::Instance()->getEnemyMissileDamage()/dD));

                // Check health player
                if(player->getDead())
                    DeadPlayer();

                // Update health indicator
                headLayer->UpdateHealth(player->getHealth());
            }

            // Check screen border
            if (nextEnemyMissile->getPositionY() < 0)
                recyclebinEnemyMissiles.pushBack(nextEnemyMissile);
        }
    }

    // Empty recycle bin
    for (auto &nextMissile : recyclebinEnemyMissiles) {
        auto *target = nextMissile;
        enemyWeapons.eraseObject(target);
        recyclebinEnemyMissiles.eraseObject(target);
        this->removeChild(target, true);
    }
}

// Monitoring player missiles
void NebulaGame::PlayerMissileMonitoring(float dt) {
    // Player missiles collision
    if(playerWeapons.size() > 0 ) {
        for (auto &nextPlayerMissile : playerWeapons) {
            // Update player missile
            nextPlayerMissile->update(dt);

            // Collision with enemy spaceships
            for (auto &nextEnemy : enemies) {
                if (!nextPlayerMissile->getIsDead() && checkCollision(nextPlayerMissile, nextEnemy)) {
                    // isDead
                    nextPlayerMissile->setIsDead();

                    // Swith damage
                    if (FMISSILE == nextPlayerMissile->getType()) {
                        // Show missile explosion
                        createExplosion(nextPlayerMissile->getPosition(), FMISSILE);

                        nextEnemy->setHealth(nextEnemy->getHealth() - 20);
                    } else {
                        // Show missile explosion
                        createExplosion(nextPlayerMissile->getPosition(), PMISSILE);

                        nextEnemy->setHealth(nextEnemy->getHealth() - 100);
                    }

                    // Move missile to recycle bin
                    recyclebinPlayerMissiles.pushBack(nextPlayerMissile);

                    // Check enemy health
                    if (nextEnemy->getDead()) {
                        // Show enemy spaceship explosion
                        createExplosion(nextEnemy->getPosition(), SPACESHIP);

                        // Move enemy to recycle bin
                        recyclebinEnemies.pushBack(nextEnemy);

                        // Storage box from enemy spaceship
                        // Random number from 0 to 9 (1:10 instead 1:2)
                        if (7 == (rand() % 10))
                            createStorageBox(int(rand() % 2), Vec2(nextEnemy->getPositionX(), nextEnemy->getPositionY()));

                        // Update score
                        score++;

                        // Update score indicator
                        headLayer->SetScore(score);

                        // check high score
                        if(score > PublicVariables::Instance()->getHighScore() && !isHiScore) {
                            isHiScore = true;

                            // Play sound
                            if (PublicVariables::Instance()->getSFX())
                                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("highscore.mp3");

                            // Show assistant message
                            if (PublicVariables::Instance()->getAssistant())
                                showMyAssistant(mdialogs[PublicVariables::Instance()->tutorDialogCount()+1], SCORE, true);
                        }

                        // Enemy rush hour
                        if (0 == (score % PublicVariables::Instance()->getStartRush()) && score != 0)
                            rush = true;                        
                    }
                }
            }

            // Collision with foreground asteroids
            for (auto &nextAsteroid : asteroids) {
                if (!nextAsteroid->getDead() && checkCollision(nextPlayerMissile, nextAsteroid) && !nextPlayerMissile->getIsDead()) {
                    // isDead
                    nextPlayerMissile->setIsDead();

                    // Show missile explosion
                    if (FMISSILE ==  nextPlayerMissile->getType())
                        createExplosion(nextAsteroid->getPosition(), FMISSILE);
                    else
                        createExplosion(nextAsteroid->getPosition(), PMISSILE);

                    // Move missile to recycle bin
                    recyclebinPlayerMissiles.pushBack(nextPlayerMissile);

                    // Show asteroid explosion
                    nextAsteroid->showExplode();
                }
            }

            // Check screen border
            if(nextPlayerMissile->getPositionY() > visibleSize.height)                
                recyclebinPlayerMissiles.pushBack(nextPlayerMissile);
        }
    }

    // Empty recycle bin
    for (auto &nextMissile : recyclebinPlayerMissiles) {
        auto target = nextMissile;
        playerWeapons.eraseObject(target);
        recyclebinPlayerMissiles.eraseObject(target);
        this->removeChild(target, true);
    }
}

// Check collision by bounding box
bool NebulaGame::checkCollision(Sprite *boxA, Sprite *boxB) {
    Rect rectA = boxA->getBoundingBox();
    Rect rectB = boxB->getBoundingBox();

    if(rectA.intersectsRect(rectB))
        return true;
    else
        return false;
}

// Random enemy spawn
void NebulaGame::randomEnemySpawn(float dt) {
    int rc = rand() % 2;
    if(rc == 0)
        createEnemy();
    else
        createAsteroid();
}

// Create foreground asteroid
void NebulaGame::createAsteroid() {
    Asteroid *_asteroid;
    string name[] = { "AsteroidFG1.png", "AsteroidFG2.png", "AsteroidFG3.png" };
    int rc = rand() % int(sizeof(name)/sizeof(*name));

    _asteroid = Asteroid::create(name[rc], PublicVariables::Instance()->getAsteroidSpeed());

    this->addChild(_asteroid, 2);
    asteroids.pushBack(_asteroid);
}

// Create enemy spaceship
void NebulaGame::createEnemy() {
    Enemy *_enemy;
    string name[] = { "EnemyShip01.png", "EnemyShip02.png", "EnemyShip03.png" };
    int rc = rand() % int(sizeof(name)/sizeof(*name));

    _enemy = Enemy::create(name[rc]);

    this->addChild(_enemy, 2);
    enemies.pushBack(_enemy);
}

// Create enemy missiles
void NebulaGame::createEnemyRocket(float dt) {
    for (auto &nextEnemy : enemies) {        
        float dx = 0.0f;
        float dy = nextEnemy->getPosition().y - nextEnemy->getContentSize().height/2 - 10.0f;

        if(nextEnemy->getSide()) {
            nextEnemy->setSide(false);
            dx = nextEnemy->getPosition().x + nextEnemy->getContentSize().width/2 - 10;
        } else {
            nextEnemy->setSide(true);
            dx = nextEnemy->getPosition().x - nextEnemy->getContentSize().width/2 + 10;
        }

        Weapon *newMissile = Weapon::create("RocketEasy.png", true, FIREROCKET);
        newMissile->setStartPosition(Vec2(dx, dy));
        this->addChild(newMissile, 1);
        enemyWeapons.pushBack(newMissile);
    }
}

// Create player missiles
void NebulaGame::createPlayerRocket(int _rtype) {
    if (!player->getDead()) {
        string fname[] = { "RocketEasy.png", "RocketHard.png" };
        float dx;
        float dy = player->getPosition().y + player->getContentSize().height/2 + 20.0f;

        if(player->getSide()) {
            player->setSide(false);
            dx = player->getPosition().x + player->getContentSize().width/2 - 10;
        } else {
            player->setSide(true);
            dx = player->getPosition().x - player->getContentSize().width/2 + 10;
        }

        auto missile = Weapon::create(fname[_rtype], false, _rtype);
        missile->setStartPosition(Vec2(dx, dy));
        this->addChild(missile, 1);
        playerWeapons.pushBack(missile);

        if(PublicVariables::Instance()->getSFX()) {
            auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
            audio->playEffect("Missile.mp3");
        }
    }
}

// schedule fire missiles
void NebulaGame::scheduleFireRocket() {
    if(!player->getDead() && player->getOverheat() > 0) {

        createPlayerRocket(FIREROCKET);

        player->setOverheat(player->getOverheat() - 50 * PublicVariables::Instance()->getDownOverheatTimer());
        headLayer->UpdateOverheat(player->getOverheat());

        if (!isFireRocketSchedule) {
            this->schedule(schedule_selector(NebulaGame::callFireRocket), PublicVariables::Instance()->getPlayerMissileTimer());
            isFireRocketSchedule = true;
        }

        if (isUpOverheatSchedule) {
            this->unschedule(schedule_selector(NebulaGame::upOverheat));
            isUpOverheatSchedule = false;
        }

        if (!isDownOverheatSchedule) {
            this->schedule(schedule_selector(NebulaGame::downOverheat), PublicVariables::Instance()->getDownOverheatTimer());
            isDownOverheatSchedule = true;
        }
    }
}

// schedule plasma missiles
void NebulaGame::schedulePlasmaRocket() {
    if(!player->getDead() && player->getPlasmaRocket() > 0) {

        createPlayerRocket(PLASMAROCKET);

        player->setPlasmaRocket(player->getPlasmaRocket() - PublicVariables::Instance()->getPlasmaPercent());
        headLayer->UpdatePlasma(player->getPlasmaRocket());

        this->schedule(schedule_selector(NebulaGame::callPlasmaRocket), PublicVariables::Instance()->getPlayerMissileTimer());
    }
}

// unschedule fire missiles
void NebulaGame::unscheduleFireRocket() {
    if (isFireRocketSchedule) {
        this->unschedule(schedule_selector(NebulaGame::callFireRocket));
        isFireRocketSchedule = false;
    }

    if (isDownOverheatSchedule) {
        this->unschedule(schedule_selector(NebulaGame::downOverheat));
        isDownOverheatSchedule = false;
    }

    if (!isUpOverheatSchedule) {
        this->schedule(schedule_selector(NebulaGame::upOverheat), PublicVariables::Instance()->getUpOverheatTimer());
        isUpOverheatSchedule = true;
    }
}

// unschedule plasma missiles
void NebulaGame::unschedulePlasmaRocket() {    
    this->unschedule(schedule_selector(NebulaGame::callPlasmaRocket));    
}

// Call fire missiles after pressed fire button
void NebulaGame::callFireRocket(float dt) {
    createPlayerRocket(FIREROCKET);    
}

// Call plasma missiles after pressed plasma button
void NebulaGame::callPlasmaRocket(float dt) {
    if (player->getPlasmaRocket() <= 0) {
        this->unschedule(schedule_selector(NebulaGame::callPlasmaRocket));
    } else {
        createPlayerRocket(PLASMAROCKET);

        player->setPlasmaRocket(player->getPlasmaRocket() - PublicVariables::Instance()->getPlasmaPercent());
        headLayer->UpdatePlasma(player->getPlasmaRocket());
    }
}

// Create storage box
void NebulaGame::createStorageBox(int boxType, Vec2 _pos) {
    StorageBox *_storagebox;
    string name[] = { "HealthBox.png", "RocketBox.png" };

    _storagebox = StorageBox::create(name[boxType], boxType, _pos);

    this->addChild(_storagebox, 2);
    storageboxes.pushBack(_storagebox);
}

// Create explosion
void NebulaGame::createExplosion(Vec2 _pos, explosionType et) {
    if(PublicVariables::Instance()->getSFX()) {
        if (et == SPACESHIP)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ShipExplosion.mp3");
        else
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("MissileExplosion.mp3");
    }

    auto theExplosion = Explosions::create(et, _pos);
    this->addChild(theExplosion);
}

// game pause
void NebulaGame::pauseGame() {
    pauseMode = true;
    this->unschedule(schedule_selector(NebulaGame::randomEnemySpawn));
    this->unschedule(schedule_selector(NebulaGame::createEnemyRocket));
    this->unschedule(schedule_selector(NebulaGame::downOverheat));
    this->unschedule(schedule_selector(NebulaGame::upOverheat));

    if (isFireRocketSchedule)
        this->unschedule(schedule_selector(NebulaGame::callFireRocket));

    if (player->getShield()) {
        this->unschedule(schedule_selector(NebulaGame::downShield));
        player->pauseAnimationShield();
    }

    if (!player->getShield() && player->getEnergy() < 100) {
        this->unschedule(schedule_selector(NebulaGame::upShield));
        player->pauseAnimationShield();
    }

    backgroundLayer->pauseAsteroid();
    player->pauseParticle();

    if (enemies.size() >= 0 ) {
        for (auto &nextEnemy : enemies) {
            Enemy *enemy = (Enemy*) nextEnemy;
            enemy->pauseParticle();
        }
    }

    if(enemyWeapons.size() >= 0 ) {
        for (auto &nextWeapon : enemyWeapons) {
            Weapon* weapon = (Weapon*) nextWeapon;
            weapon->pauseParticle();
        }
    }

    if(playerWeapons.size() >= 0 ) {
        for (auto &nextWeapon : playerWeapons) {
            Weapon* weapon = (Weapon*) nextWeapon;
            weapon->pauseParticle();
        }
    }

    // Check Network
    this->unschedule(schedule_selector(NebulaGame::callAdWare));
}

// game resume
void NebulaGame::resumeGame() {
    pauseMode = false;    
    this->schedule(schedule_selector(NebulaGame::randomEnemySpawn), PublicVariables::Instance()->getEnemyTimer());
    this->schedule(schedule_selector(NebulaGame::createEnemyRocket), PublicVariables::Instance()->getEnemyMissileTimer());

    if (isFireRocketSchedule)
        this->schedule(schedule_selector(NebulaGame::callFireRocket), PublicVariables::Instance()->getPlayerMissileTimer());

    if (isDownOverheatSchedule)
        this->schedule(schedule_selector(NebulaGame::downOverheat), PublicVariables::Instance()->getDownOverheatTimer());

    if (isUpOverheatSchedule)
        this->schedule(schedule_selector(NebulaGame::upOverheat), PublicVariables::Instance()->getUpOverheatTimer());

    if (player->getShield()) {
        this->schedule(schedule_selector(NebulaGame::downShield), PublicVariables::Instance()->getDownShieldTimer());
        player->resumeAnimationShield();
    }

    if (!player->getShield() && player->getEnergy() < 100) {
        this->schedule(schedule_selector(NebulaGame::upShield), PublicVariables::Instance()->getUpShieldTimer());
        player->resumeAnimationShield();
    }

    backgroundLayer->resumeAsteroid();
    player->resumeParticle();

    if (enemies.size() >= 0 ) {
        for (auto &nextEnemy : enemies) {
            Enemy *enemy = (Enemy*) nextEnemy;
            enemy->resumeParticle();
        }
    }

    if(enemyWeapons.size() >= 0 ) {
        for (auto &nextWeapon : enemyWeapons) {
            Weapon* weapon = (Weapon*) nextWeapon;
            weapon->resumeParticle();
        }
    }

    if(playerWeapons.size() >= 0 ) {
        for (auto &nextWeapon : playerWeapons) {
            Weapon* weapon = (Weapon*) nextWeapon;
            weapon->resumeParticle();
        }
    }

    // Check Network
    this->schedule(schedule_selector(NebulaGame::callAdWare), PublicVariables::Instance()->getAdTime());
}

// Update music switch after settings
void NebulaGame::updateMusic() {
    if(PublicVariables::Instance()->getMusic())
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("theme.mp3", true);
    else
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

// Stop all sounds by Game Over layer
void NebulaGame::stopAllSounds() {
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

// Call pause layer after pressed menu button
void NebulaGame::callMenuGame() {
    if (!pauseMode && !player->getDead()) {
        pauseGame();
        HideMyHUD();
        PauseLayer *pauseLayer = new PauseLayer();
        this->addChild(pauseLayer, 50);
    }
}

// Call AdWare
void NebulaGame::callAdWare(float dt) {
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    this->unschedule(schedule_selector(NebulaGame::callAdWare));
    if (!CheckNet::getNetCon()) {
        showMyAssistant("Check your network connection", ADWARE, false);
    }
    #endif
}


