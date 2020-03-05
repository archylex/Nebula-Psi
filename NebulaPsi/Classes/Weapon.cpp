#include "Weapon.h"

// Create missile sprite
Weapon *Weapon::create(string _name, bool _isEnemy, int _type) {
    Weapon *_weapon = new Weapon();

    if(_weapon && _weapon->initialize(_name, _isEnemy, _type)) {
        _weapon->autorelease();
        return _weapon;
    }

    CC_SAFE_DELETE(_weapon);

    return nullptr;
}

// Property initialization
bool Weapon::initialize(string _name, bool _isEnemy, int _type) {
    // Missile type (fire or plasma)
    typeWeapon = _type;

    // isDead
    isDead = false;

    // Speed from public variables (config)
    speed = PublicVariables::Instance()->getMissileSpeed();

    // Set image
    this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_name));

    // Set anchor point
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Create emitter
    string emitterName[] = { "MissileEmitter.png", "PlasmaEmitter.png" };
    emitter = Sprite::createWithSpriteFrameName(emitterName[typeWeapon]);
    emitter->setAnchorPoint(Vec2(0.5f, 1.0f));
    emitter->setPosition(this->getContentSize().width/2, 0.0f);
    this->addChild(emitter);

    // Emitter animation
    auto little = ScaleTo::create(0.2f, 1.5f);
    auto big = ScaleTo::create(0.2f, 1.0f, 2.7f);
    auto seq = Sequence::create(little, big, NULL);
    auto anim = RepeatForever::create(seq);
    emitter->runAction(anim);

    // Whose missile
    if (_isEnemy) {
        // Rotate missile
        this->setRotation(180);

        // Inverse path
        speed *= -1;
    }

    return true;
}

// Update missile
void Weapon::update(float dt) {
    this->setPosition(Vec2(this->getPosition().x, this->getPosition().y + speed * dt));
}

// Set missile position
void Weapon::setStartPosition(Vec2 _pos) {
    this->setPosition(_pos);
}

// Get missile y-position
float Weapon::getYPosition() {
    return this->getPosition().y;
}

// Get missile type (fire or plasma)
int Weapon::getType() {
    return typeWeapon;
}

// Set isDead
void Weapon::setIsDead() {
    isDead = true;
}

bool Weapon::getIsDead() {
    return isDead;
}

// Pause particle system
void Weapon::pauseParticle() {
    emitter->pause();
}

// Resume particle system
void Weapon::resumeParticle() {
    emitter->resume();
}




