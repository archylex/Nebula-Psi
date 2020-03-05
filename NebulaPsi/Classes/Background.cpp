#include "Background.h"
#include "PublicVariables.h"

Background *Background::create(string _name) {
    Background *background = new Background();
    if(background && background->initialize(_name)) {
        background->autorelease();
        return background;
    }

    CC_SAFE_DELETE(background);
    return nullptr;
}


bool Background::initialize(string _name) {
    name = _name;

    first_background = Sprite::createWithSpriteFrameName(_name);
    first_background->setAnchorPoint(Vec2(0.0f, 0.0f));
    first_background-> setPosition(Vec2(0, 0));
    addChild(first_background);

    second_background = Sprite::createWithSpriteFrameName(_name);
    second_background->setAnchorPoint(Vec2(0.0f, 0.0f));
    second_background->setPosition(Vec2(0, first_background->getContentSize().height - 1));
    addChild(second_background);

    return true;
}


void Background::update(float dt) {
    respawn(first_background);
    respawn(second_background);

    move(first_background, dt);
    move(second_background, dt);
}


void Background::respawn(Sprite *_background) {
    if(_background->getPosition().y <= - _background->getContentSize().height)
        _background->setPosition(Vec2(0.0f, _background->getContentSize().height - 2));
}


void Background::move(Sprite *_background, float dt) {
    Point bg_pos = _background->getPosition();
    _background->setPosition(Vec2(bg_pos.x, bg_pos.y - PublicVariables::Instance()->getBackgroundSpeed() * dt));
}
