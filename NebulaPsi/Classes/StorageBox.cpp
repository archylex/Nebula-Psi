#include "StorageBox.h"
#include "PublicVariables.h"

StorageBox *StorageBox::create(string _name, int _boxType, Vec2 _pos) {
    StorageBox *_storageBox = new StorageBox();

    if(_storageBox && _storageBox->initialize(_name, _boxType, _pos)) {
        _storageBox->autorelease();
        return _storageBox;
    }

    CC_SAFE_DELETE(_storageBox);

    return nullptr;
}


bool StorageBox::initialize(string _name, int _boxType, Vec2 _pos) {
    auto spritecache = SpriteFrameCache::getInstance();

    boxType = _boxType;

    this->setSpriteFrame(spritecache->getSpriteFrameByName(_name));
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->setPosition(_pos);

    return true;
}

int StorageBox::getBoxType() {
    return boxType;
}

void StorageBox::setDead(bool _isDead) {
    isDead = _isDead;
}

bool StorageBox::getDead() {
    return isDead;
}

void StorageBox::update(float dt) {
    Vec2 pos = this->getPosition();
    float scale = 0.9f + 0.1f * sin(pos.y / PublicVariables::Instance()->getStorageBoxSpeed());
    this->setPosition(Vec2(pos.x, pos.y - PublicVariables::Instance()->getStorageBoxSpeed() * dt));
    this->setScale(scale);
}


