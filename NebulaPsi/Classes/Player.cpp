#include "Player.h"
#include "PublicVariables.h"
#include "AnimationTools.h"
#include <math.h>

Player *Player::create(string _name) {
    Player *_player = new Player();

    if(_player && _player->initialize(_name)) {
        _player->autorelease();
        return _player;
    }

    CC_SAFE_DELETE(_player);

    return nullptr;
}


bool Player::initialize(string _name) {    
    // left or right gun
    side = false;

    // shield activity
    shield = false;

    // pressed fire
    isFire = false;

    energy = 100;
    overheat = 100;
    health = 100;
    plasma = 100;

    // x-velocity
    vX = 0;

    // y-velocity
    vY = 0;

    visibleSize = Director::getInstance()->getVisibleSize();

    auto spritecache = SpriteFrameCache::getInstance();

    this->setSpriteFrame(spritecache->getSpriteFrameByName(_name));

    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4));

    // sprite engines
    string engineName = "PlayerEngine.png";
    firstEngine = Sprite::createWithSpriteFrameName(engineName);
    firstEngine->setAnchorPoint(Vec2(0.5f, 1.0f));
    firstEngine->setPosition(Vec2(this->getContentSize().width*3/8, this->getContentSize().height/5));
    this->addChild(firstEngine, 1);

    secondEngine = Sprite::createWithSpriteFrameName(engineName);
    secondEngine->setAnchorPoint(Vec2(0.5f, 1.0f));
    secondEngine->setPosition(Vec2(this->getContentSize().width*5/8, this->getContentSize().height/5));
    this->addChild(secondEngine, 1);

    // sprite emmiters
    string emitterName = "PlayerEmitter.png";
    firstEmitter = Sprite::createWithSpriteFrameName(emitterName);
    firstEmitter->setAnchorPoint(Vec2(0.5f, 1.0f));
    firstEmitter->setPosition(Vec2(firstEngine->getContentSize().width/2, 0));
    firstEngine->addChild(firstEmitter,0);

    // animation
    auto fbig = ScaleTo::create(0.2f, 1);
    auto flittle = ScaleTo::create(0.2f, 1, 0.8f);
    auto fanim = Sequence::create(fbig, flittle , NULL);
    auto frep = RepeatForever::create(fanim);
    firstEmitter->runAction(frep);

    secondEmitter = Sprite::createWithSpriteFrameName(emitterName);
    secondEmitter->setAnchorPoint(Vec2(0.5f, 1.0f));
    secondEmitter->setPosition(Vec2(secondEngine->getContentSize().width/2, 0));
    secondEngine->addChild(secondEmitter, 0);

    // animation
    auto sbig = ScaleTo::create(0.2f, 1);
    auto slittle = ScaleTo::create(0.2f, 1, 0.8f);
    auto sanim = Sequence::create(sbig, slittle , NULL);
    auto srep = RepeatForever::create(sanim);
    secondEmitter->runAction(srep);

    auto cloneShip = Sprite::createWithSpriteFrameName(_name);
    cloneShip->setAnchorPoint(Vec2(0.0f, 0.0f));
    this->addChild(cloneShip, 3);

    // Fade in animation (shield)
    //auto frames = getAnimation("%04d.png", 1, 12);
    auto frames = AnimationTools::getAnimation("%04d.png", 1, 12);
    auto animation = Animation::createWithSpriteFrames(frames, 0.5f/12);
    activateShield = Animate::create(animation);
    activateShield->retain();

    // Create shield sprite
    defendSprite = Sprite::createWithSpriteFrame(frames.front());
    defendSprite->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(defendSprite, 5);

    // Shield animation
    //auto frames2 = getAnimation("%04d.png", 13, 48);
    auto frames2 = AnimationTools::getAnimation("%04d.png", 13, 48);
    auto animation2 = Animation::createWithSpriteFrames(frames2, 2.0f/36);
    defended = RepeatForever::create(Animate::create(animation2));
    defended->retain();

    // Fade out animation (shield)
    //auto frames3 = getAnimation("%04d.png", -12, -1);
    auto frames3 = AnimationTools::getAnimation("%04d.png", -12, -1);
    auto animation3 = Animation::createWithSpriteFrames(frames3, 0.5f/12);
    deactivateShield = Animate::create(animation3);
    deactivateShield->retain();

    return true;
}

void Player::afterStartShield(Ref* _sender) {
    defendSprite->stopAllActions();
    defendSprite->runAction(defended);
}

void Player::startShield() {
    defendSprite->stopAllActions();

    FiniteTimeAction *callAct = CallFuncN::create(CC_CALLBACK_1(Player::afterStartShield, this));

    auto seq = Sequence::create(activateShield, callAct, nullptr);
    defendSprite->runAction(seq);
}

void Player::stopShield() {
    defendSprite->stopAllActions();
    defendSprite->runAction(deactivateShield);
}

void Player::pauseAnimationShield() {
    defendSprite->pause();
}

void Player::resumeAnimationShield() {
    defendSprite->resume();
}

void Player::setShield(bool _sw) {
    if (shield && !_sw) {
        stopShield();
        shield = _sw;
    }
    if (!shield && _sw) {
        startShield();
        shield = _sw;
    }
}

bool Player::getShield() {
    return shield;
}

void Player::setEnergy(int _energy) {
    energy = _energy;
}

int Player::getEnergy() {
    return energy;
}

void Player::setOverheat(int _overheat) {
    overheat = _overheat;
}

int Player::getOverheat() {
    return overheat;
}

void Player::setIsFire(bool _isfire) {
    isFire = _isfire;
}

bool Player::getIsFire() {
    return  isFire;
}

// Get the number of Plasma missiles
float Player::getPlasmaRocket() {
    return plasma;
}

// Set the number of Plasma missiles
void Player::setPlasmaRocket(float _plasma) {
    plasma = _plasma;
}

// Calculate angle
float Player::getAngle(float x, float y) {
    float angle = 0.0f;
    if (y > 0) {
        if (x > 0)
            angle = - atan(y / x) * 180.0f / 3.14f + 90.0f;
        if (x < 0)
            angle = atan(x / y) * 180.0f / 3.14f;
        if (x == 0.0f)
            angle = 0;
    }

    if (y < 0) {
        if (x > 0)
            angle = atan(x / y) * 180.0f / 3.14f + 180.0f;
        if (x < 0)
            angle = atan(x / y) * 180.0f / 3.14f + 180.0f;
        if (x == 0.0f)
            angle = 180.0f;
    }

    if (y == 0.0f) {
        if (x > 0)
            angle = 90.0f;
        if (x < 0)
            angle = -90.0f;
    }

    return angle;
}

// Emitter animation
void Player::setEmitterAnimation() {
    // first emitter
    firstEmitter->stopAllActions();
    auto fbig = ScaleTo::create(0.2f, 1 + MAX(abs(vY), abs(vX)));
    auto flittle = ScaleTo::create(0.2f, 1, 0.8f + MAX(abs(vY), abs(vX)));
    auto fanim = Sequence::create(fbig, flittle , NULL);
    auto frep = RepeatForever::create(fanim);
    firstEmitter->runAction(frep);

    // second emitter
    secondEmitter->stopAllActions();
    auto sbig = ScaleTo::create(0.2f, 1 + MAX(abs(vY), abs(vX)));
    auto slittle = ScaleTo::create(0.2f, 1, 0.8f + MAX(abs(vY), abs(vX)));
    auto sanim = Sequence::create(sbig, slittle , NULL);
    auto srep = RepeatForever::create(sanim);
    secondEmitter->runAction(srep);
}

// Set X-velocity
void Player::setVX(float _x) {
    vX = _x;

    float angle = getAngle(vX, vY);

    if (!getDead()) {
        firstEngine->setRotation(angle);
        secondEngine->setRotation(angle);

        setEmitterAnimation();
    }
}

// Set Y-velocity
void Player::setVY(float _y) {
    vY = _y;

    float angle = getAngle(vX, vY);

    if (!getDead()) {
        firstEngine->setRotation(angle);
        secondEngine->setRotation(angle);

        setEmitterAnimation();
    }
}

float Player::getVX() {
    return vX;
}

float Player::getVY() {
    return vY;
}

void Player::setHealth(float _health) {
    health = _health;
}

float Player::getHealth() {
    return health;
}

void Player::setSide(bool _side) {
    side = _side;
}

bool Player::getSide() {
    return side;
}

bool Player::getDead() {
    if (health <= 0)
        return true;
    return false;
}

void Player::update(float dt) {
    // Set new position
    this->setPosition(Vec2(this->getPosition().x + vX * PublicVariables::Instance()->getPlayerSpeed() * dt,
                           this->getPosition().y + vY * PublicVariables::Instance()->getPlayerSpeed() * dt));

    // Right border
    if(this->getPosition().x > visibleSize.width - this->getContentSize().width/2)
       this->setPosition(Vec2(visibleSize.width - this->getContentSize().width/2, this->getPosition().y));

    // Left border
    if(this->getPosition().x < this->getContentSize().width/2)
        this->setPosition(Vec2(this->getContentSize().width/2, this->getPosition().y));

    // Top border
    if(this->getPosition().y > visibleSize.height - this->getContentSize().height/2)
        this->setPosition(Vec2(this->getPosition().x, visibleSize.height - this->getContentSize().height/2));

    // Bottom border
    if(this->getPosition().y < this->getContentSize().height/2)
        this->setPosition(Vec2(this->getPosition().x, this->getContentSize().height/2));
}

// pause emitters
void Player::pauseParticle() {
    firstEmitter->pause();
    secondEmitter->pause();
}

// pause emitters
void Player::resumeParticle() {
    firstEmitter->resume();
    secondEmitter->resume();
}
