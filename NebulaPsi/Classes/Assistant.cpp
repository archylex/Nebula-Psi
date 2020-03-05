#include "Assistant.h"
#include "NebulaGame.h"

Assistant::Assistant() {

    auto visibleSize = Director::getInstance()->getVisibleSize();

    float border = visibleSize.width / 48;

    auto aAvatar = Sprite::createWithSpriteFrameName("Kasia.png");
    aAvatar->setAnchorPoint(Vec2(1.0f, 1.0f));
    aAvatar->setPosition(Vec2(visibleSize.width - border, visibleSize.height/2 + aAvatar->getContentSize().height/2));
    this->addChild(aAvatar);

    auto aDialog = Sprite::createWithSpriteFrameName("dialog.png");
    aDialog->setAnchorPoint(Vec2(0.0f, 1.0f));
    aDialog->setPosition(Vec2(border, visibleSize.height/2 + aDialog->getContentSize().height/2));
    this->addChild(aDialog);


    label = Label::createWithBMFont("font.fnt", "");
    label->setAnchorPoint(Vec2(0.0f, 1.0f));
    label->setPosition(Vec2(2*border, aDialog->getContentSize().height - 2*border));
    label->setScale(0.27f);
    aDialog->addChild(label, 3);
    label->setDimensions(1050, 0);

    auto helpLabel = Label::createWithBMFont("font.fnt", "TOUCH TO CONTINUE");
    helpLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
    helpLabel->setPosition(Vec2(aDialog->getContentSize().width - border, -border));
    aDialog->addChild(helpLabel, 3);
    helpLabel->setScale(0.15f);
}

void Assistant::setText(string _text) {
    label->setString(_text);
}

void Assistant::setMode(int _mode) {
    mode = _mode;
}

int Assistant::getMode() {
    return mode;
}

