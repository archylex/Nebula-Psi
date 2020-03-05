#include "ControlLayer.h"
#include "NebulaGame.h"
#include "PauseLayer.h"

ControlLayer::ControlLayer() {
    visibleSize = Director::getInstance()->getVisibleSize();

    // Control background
    control = Sprite::createWithSpriteFrameName("ControlPanel.png");
    control->setAnchorPoint(Vec2(0.5f, 0.0f));
    control->setPosition(Vec2(visibleSize.width/2, 0));
    this->addChild(control);

    // Joystick
    crossJoystick = Sprite::createWithSpriteFrameName("Cross.png");
    centerJoystick = Vec2(control->getContentSize().width * 0.15833f, control->getContentSize().height * 0.45833f);
    crossJoystick->setAnchorPoint(Vec2(0.5f, 0.5f));
    crossJoystick->setPosition(centerJoystick);
    control->addChild(crossJoystick);

    // Pause button
    pauseButton = Button::create("MenuButton.png", "MenuButton.png", "MenuButton.png", Widget::TextureResType::PLIST);
    pauseButton->setAnchorPoint(Vec2(0.5f,0.5f));
    pauseButton->setPosition(Vec2(control->getContentSize().width * 0.50417f, control->getContentSize().height * 0.35f));
    pauseButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                NebulaGame *game = (NebulaGame*)this->getParent();
                game->callMenuGame();
                break;
        }
    });
    control->addChild(pauseButton);

    // Fire missile button
    rocketButton = Button::create("RocketStormButton.png", "RocketStormButton.png", "RocketStormButton.png", Widget::TextureResType::PLIST);
    rocketButton->setAnchorPoint(Vec2(0.5f,0.5f));
    rocketButton->setPosition(Vec2(control->getContentSize().width * 0.9125f, control->getContentSize().height * 0.69444f));
    rocketButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN: {
                    NebulaGame *game = (NebulaGame*)this->getParent();
                    if (!game->pauseMode)
                        game->scheduleFireRocket();
                }
                break;
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED: {
                    NebulaGame *game = (NebulaGame*)this->getParent();                    
                    if (!game->pauseMode)
                        game->unscheduleFireRocket();
                }
                break;
        }
    });
    control->addChild(rocketButton);

    // Plasma missile button
    plasmaButton = Button::create("RocketPlasmaButton.png", "RocketPlasmaButton.png", "RocketPlasmaButton.png", Widget::TextureResType::PLIST);
    plasmaButton->setAnchorPoint(Vec2(0.5f,0.5f));
    plasmaButton->setPosition(Vec2(control->getContentSize().width * 0.775f, control->getContentSize().height/2));
    plasmaButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN: {
                    NebulaGame *game = (NebulaGame*)this->getParent();
                    if (!game->pauseMode)
                        game->schedulePlasmaRocket();
                }
                break;
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED: {
                    NebulaGame *game = (NebulaGame*)this->getParent();
                    if (!game->pauseMode)
                        game->unschedulePlasmaRocket();
                }
                break;
        }
    });
    control->addChild(plasmaButton);

    // Shield button
    shieldButton = Button::create("ShieldButton.png", "ShieldButton.png", "ShieldButton.png", Widget::TextureResType::PLIST);
    shieldButton->setAnchorPoint(Vec2(0.5f,0.5f));
    shieldButton->setPosition(Vec2(control->getContentSize().width * 0.9125f, control->getContentSize().height * 0.23611f));
    shieldButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                NebulaGame *game = (NebulaGame*)this->getParent();
                if (!game->pauseMode)
                    game->startShield();
                break;
        }
    });
    control->addChild(shieldButton);

}

void ControlLayer::SetPosCross(Vec2 _pos) {
    Vec2 rv = centerJoystick - _pos;
    float touchRadius = sqrt(pow(rv.x, 2.0f) + pow(rv.y, 2.0f));
    float crossRadius = control->getContentSize().height * 0.40278f;

    if(touchRadius > crossRadius) {
        float scale = touchRadius / crossRadius;
        crossJoystick->setPosition(Vec2(centerJoystick.x - rv.x/scale, centerJoystick.y - rv.y/scale));
    } else {
        crossJoystick->setPosition(_pos);
    }
}

Vec2 ControlLayer::GetPosCross() {
    return (crossJoystick->getPosition() - centerJoystick) / (control->getContentSize().height * 0.40278f);
}

void ControlLayer::ResetPosCross() {
    crossJoystick->setPosition(centerJoystick);
}

bool ControlLayer::CrossZone(Vec2 _pos) {
    Vec2 rv = centerJoystick - _pos;

    if(sqrt(pow(rv.x, 2.0f) + pow(rv.y, 2.0f)) <= control->getContentSize().height * 0.40278f) {
        crossJoystick->setPosition(_pos);
        return true;
    }

    return false;
}

void ControlLayer::lightFireRocket(bool _is) {
    if (_is)
        rocketButton->setColor(Color3B::RED);
    else
        rocketButton->setColor(Color3B::WHITE);
}

void ControlLayer::lightPlasmaRocket(bool _is) {
    if (_is)
        plasmaButton->setColor(Color3B::RED);
    else
        plasmaButton->setColor(Color3B::WHITE);
}

void ControlLayer::lightMenu(bool _is) {
    if (_is)
        pauseButton->setColor(Color3B::RED);
    else
        pauseButton->setColor(Color3B::WHITE);
}

void ControlLayer::lightShield(bool _is) {
    if (_is)
        shieldButton->setColor(Color3B::RED);
    else
        shieldButton->setColor(Color3B::WHITE);
}

void ControlLayer::lightCross(bool _is) {
    if (_is)
        crossJoystick->setColor(Color3B::RED);
    else
        crossJoystick->setColor(Color3B::WHITE);
}

void ControlLayer::HideMe() {
    auto cMoveTo = MoveTo::create(0.2f, Vec2(0, -control->getContentSize().height));
    this->runAction(cMoveTo);
}

void ControlLayer::ShowMe() {
    auto cMoveTo = MoveTo::create(0.2f, Vec2(0, 0));
    this->runAction(cMoveTo);
}

void ControlLayer::ResetMe() {
    this->setPosition(Vec2(0, -control->getContentSize().height));
}
