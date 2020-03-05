#include "Enemy.h"
#include "PublicVariables.h"

Enemy *Enemy::create(string _name) {
    Enemy *_enemy = new Enemy();

    if(_enemy && _enemy->initialize(_name)) {
        _enemy->autorelease();
        return _enemy;
    }

    CC_SAFE_DELETE(_enemy);

    return nullptr;
}

bool Enemy::initialize(string _name) {
    health = 100;

    visibleSize = Director::getInstance()->getVisibleSize();

    auto spritecache = SpriteFrameCache::getInstance();

    this->setSpriteFrame(spritecache->getSpriteFrameByName(_name));
    this->setAnchorPoint(Vec2(0.5f, 0.5f));    
    int rx = rand() % int(visibleSize.width - this->getContentSize().width - 60) + int(this->getContentSize().width + 60);
    this->setPosition(Vec2(rx, visibleSize.height + this->getContentSize().height/2));



    // sprite emmiters
    string emitterName = "EnemyEmitter.png";
    firstEmitter = Sprite::createWithSpriteFrameName(emitterName);
    firstEmitter->setAnchorPoint(Vec2(0.5f, 0.0f));
    firstEmitter->setPosition(Vec2(this->getContentSize().width*3/10, this->getContentSize().height));
    this->addChild(firstEmitter,0);

    // animation
    auto fbig = ScaleTo::create(0.2f, 1);
    auto flittle = ScaleTo::create(0.2f, 1, 0.8f);
    auto fanim = Sequence::create(fbig, flittle , NULL);
    auto frep = RepeatForever::create(fanim);
    firstEmitter->runAction(frep);

    secondEmitter = Sprite::createWithSpriteFrameName(emitterName);
    secondEmitter->setAnchorPoint(Vec2(0.5f, 0.0f));
    secondEmitter->setPosition(Vec2(this->getContentSize().width*7/10, this->getContentSize().height));
    this->addChild(secondEmitter,0);

    // animation
    auto sbig = ScaleTo::create(0.2f, 1);
    auto slittle = ScaleTo::create(0.2f, 1, 0.8f);
    auto sanim = Sequence::create(sbig, slittle , NULL);
    auto srep = RepeatForever::create(sanim);
    secondEmitter->runAction(srep);

    return true;
}

void Enemy::update(float dt) {    
    Vec2 enemy_pos = this->getPosition();    
    this->setPosition(Vec2(enemy_pos.x, enemy_pos.y - PublicVariables::Instance()->getEnemySpeed() * dt));
}

bool Enemy::getSide() {
    return side;
}

void Enemy::setSide(bool _side) {
    side = _side;
}

int Enemy::getHealth() {
    return health;
}

void Enemy::setHealth(int _health) {
    health = _health;
}

bool Enemy::getDead() {
    if (health <= 0) return true;
    return false;
}

void Enemy::pauseParticle() {
    firstEmitter->pause();
    secondEmitter->pause();
}

void Enemy::resumeParticle() {
    firstEmitter->resume();
    secondEmitter->resume();
}

