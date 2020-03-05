#include "Asteroid.h"
#include "NebulaGame.h"
#include "AnimationTools.h"

// Create sprite
Asteroid *Asteroid::create(string _name, float _speed) {
    Asteroid *_asteroid = new Asteroid();

    if(_asteroid && _asteroid->initialize(_name, _speed)) {
        _asteroid->autorelease();
        return _asteroid;
    }

    CC_SAFE_DELETE(_asteroid);

    return nullptr;
}

// Initialize asteroid
bool Asteroid::initialize(string _name, float _speed) {
    // Screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Random scale
    mySize = float(rand() % 9 + 1) / 10.0f;

    // Asteroid velocity
    speed = _speed;

    // Asteroid life
    isDead = false;

    // Get cache instance
    auto spritecache = SpriteFrameCache::getInstance();

    // Set asteroid sprite
    this->setSpriteFrame(spritecache->getSpriteFrameByName(_name));

    // Set anchor point
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Set scale
    this->setScale(mySize);

    // Set random position
    this->setPosition(Vec2(rand()%int(visibleSize.width-(2*this->getContentSize().width*mySize))+int(this->getContentSize().width*mySize),
                           visibleSize.height + this->getContentSize().height/2));

    // Set random rotation
    this->setRotation(rand() % 360);

    // explode animation
    auto frames = AnimationTools::getAnimation("e%04d.png", 1, 10);
    auto animation = Animation::createWithSpriteFrames(frames, 0.5f/10);
    explode = Animate::create(animation);
    explode->retain();

    return true;
}

bool Asteroid::getDead() {
    return isDead;
}

float Asteroid::getMyWeight() {
    return mySize;
}

void Asteroid::update(float dt) {
    // Get position
    Vec2 pos = this->getPosition();

    // Set new position
    this->setPosition(Vec2(pos.x, pos.y - speed * dt));

    // Set new angle
    this->setRotation(this->getRotation() + speed/7 * dt);
}

void Asteroid::showExplode() {
    isDead = true;
    FiniteTimeAction *callAct = CallFuncN::create(CC_CALLBACK_1(Asteroid::callDead, this));
    auto seq = Sequence::create(explode, callAct, nullptr);
    this->runAction(seq);    
}

void Asteroid::callDead(Ref* pSender) {
    NebulaGame *game = (NebulaGame*) this->getParent();
    Asteroid *target = (Asteroid*)this;
    game->DeleteAsteroid(target);
}

