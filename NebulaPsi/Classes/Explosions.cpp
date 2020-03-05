#include "Explosions.h"
#include "AnimationTools.h"

// Create sprite
Explosions *Explosions::create(int _type, Vec2 _pos) {
    Explosions *_expl = new Explosions();

    if(_expl && _expl->initialize(_type, _pos)) {
        _expl->autorelease();
        return _expl;
    }

    CC_SAFE_DELETE(_expl);

    return nullptr;
}

// Initialize
bool Explosions::initialize(int _type, Vec2 _pos) {

    string filename;
    int num = 0;
    float iscale = 1.0f;
    switch (_type) {
        case 0:
            filename = "FireMissile/fexplosion%04d.png";
            num = 10;
            iscale = 2.0f;
            break;
        case 1:
            filename = "PlasmaMissile/pexplosion%04d.png";
            num = 10;
            iscale = 3.0f;
            break;
        case 2:
            filename = "Spaceship/explosion%04d.png";
            num = 14;
            break;
    }

    auto frames = AnimationTools::getAnimation(filename.c_str(), 1, num);

    auto animation = Animation::createWithSpriteFrames(frames, 0.5f/15);
    auto explode = Animate::create(animation);
    explode->retain();

    this->setSpriteFrame(frames.front());
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->setPosition(_pos);
    this->setScale(iscale);

    FiniteTimeAction *callAct = CallFuncN::create(CC_CALLBACK_1(Explosions::deleteExplosion, this));

    auto seq = Sequence::create(explode, callAct, nullptr);
    this->runAction(seq);

    return true;
}

void Explosions::deleteExplosion(Ref *_sender) {
    this->removeFromParentAndCleanup(true);
}
