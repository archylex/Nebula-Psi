#include "HeadLayer.h"

HeadLayer::HeadLayer() {
    // Screen size
    visibleSize = Director::getInstance()->getVisibleSize();

    // head background
    head = Sprite::createWithSpriteFrameName("Head.png");
    head->setAnchorPoint(Vec2(0.5f, 1.0f));
    head->setPosition(Vec2(visibleSize.width/2, visibleSize.height));
    this->addChild(head);

    // score label
    scoreLabel = Label::createWithBMFont("font.fnt", "0");    
    scoreLabel->setPosition(Vec2(head->getContentSize().width/2, head->getContentSize().height * 0.65f));
    scoreLabel->setScale(0.7f);
    Color3B gb(0, 245, 206);
    scoreLabel->setColor(gb);
    head->addChild(scoreLabel, 10);

    // Health Level
    healthLine = DrawNode::create();
    head->addChild(healthLine);

    // Energy level
    energyLine = DrawNode::create();
    head->addChild(energyLine, 50);

    // Overheat Level
    overheatLine = DrawNode::create();
    head->addChild(overheatLine);

    // Plasma level
    plasmaLine = DrawNode::create();
    head->addChild(plasmaLine);
}

void HeadLayer::SetScore(int _score) {
    scoreLabel->setString(to_string(_score));
}

void HeadLayer::UpdateLine(DrawNode *uNode, float Ax, float Ay, float Dx, float Dy, float _progress, bool _help) {
    if (_progress >= 0 && _progress <= 100) {
        float progress = (head->getContentSize().width * Dx - head->getContentSize().width * Ax) * _progress / 100;

        uNode->clear();

        Vec2 pLine[] = {
            Vec2(head->getContentSize().width * Ax, head->getContentSize().height * Ay),
            Vec2(head->getContentSize().width * Ax + progress, head->getContentSize().height * Ay),
            Vec2(head->getContentSize().width * Ax + progress, head->getContentSize().height * Dy),
            Vec2(head->getContentSize().width * Ax, head->getContentSize().height * Dy)
        };

        if(_help) {
            uNode->drawPolygon(pLine, 4, Color4F::RED, 0, Color4F::RED);
        } else {
            Color4F emerald(0.0f, 0.96f, 0.8f, 1.0f);
            uNode->drawPolygon(pLine, 4, emerald, 0, emerald);
        }
    }
}

void HeadLayer::lightHealth() {
    UpdateLine(healthLine, 0.07917f, 0.7551f, 0.29167f, 0.89796f, 100, true);
}

void HeadLayer::lightEnergy() {
    UpdateLine(energyLine, 0.07917f, 0.4898f, 0.29167f, 0.63265f, 100, true);
}

void HeadLayer::lightOverheat() {
    UpdateLine(overheatLine, 0.70833f, 0.7551f, 0.92083f, 0.89796f, 100, true);
}

void HeadLayer::lightPlasma() {
    UpdateLine(plasmaLine, 0.70833f, 0.4898f, 0.92083f, 0.63265f, 100, true);
}

void HeadLayer::lightScore() {
    scoreLabel->setColor(Color3B::RED);
}

void HeadLayer::unlightScore() {
    Color3B gb(0, 245, 206);
    scoreLabel->setColor(gb);
}

void HeadLayer::UpdateHealth(float _health) {
    if (_health < 0) _health = 0;
    if (_health > 100) _health = 100;
    UpdateLine(healthLine, 0.07917f, 0.7551f, 0.29167f, 0.89796f, _health, false);
}

void HeadLayer::UpdateEnergy(float _energy) {
    if (_energy < 0) _energy = 0;
    if (_energy > 100) _energy = 100;
    UpdateLine(energyLine, 0.07917f, 0.4898f, 0.29167f, 0.63265f, _energy, false);
}

void HeadLayer::UpdateOverheat(float _overheat) {
    if (_overheat < 0) _overheat = 0;
    if (_overheat > 100) _overheat = 100;
    UpdateLine(overheatLine, 0.70833f, 0.7551f, 0.92083f, 0.89796f, _overheat, false);
}

void HeadLayer::UpdatePlasma(float _plasma) {
    if (_plasma < 0) _plasma = 0;
    if (_plasma > 100) _plasma = 100;
    UpdateLine(plasmaLine, 0.70833f, 0.4898f, 0.92083f, 0.63265f, _plasma, false);
}

void HeadLayer::HideMe() {
    auto headMoveTo = MoveTo::create(0.2f, Vec2(0, head->getContentSize().height));
    this->runAction(headMoveTo);
}

void HeadLayer::ShowMe() {
    auto headMoveTo = MoveTo::create(0.2f, Vec2(0, 0));
    this->runAction(headMoveTo);
}

void HeadLayer::ResetMe() {
    this->setPosition(Vec2(0, head->getContentSize().height));
}


