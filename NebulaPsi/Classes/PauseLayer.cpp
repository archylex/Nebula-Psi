#include "PauseLayer.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"
#include "NebulaGame.h"
#include "Settings.h"
#include "PublicVariables.h"

PauseLayer::PauseLayer() {
    // Screen size
    visibleSize = Director::getInstance()->getVisibleSize();

    // Fade background
    auto fadeBGNode = DrawNode::create();
    Vec2 fadeBG[] = { Vec2(0, 0), Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height), Vec2(visibleSize.width, 0) };
    Color4F aBlack(0.0f, 0.0f, 0.0f, 0.5f);
    fadeBGNode->drawPolygon(fadeBG, 4, aBlack, 0, aBlack);
    this->addChild(fadeBGNode, 48);

    // Title
    pauseTitle = Sprite::createWithSpriteFrameName("PauseTitle.png");
    pauseTitle->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.8f));
    this->addChild(pauseTitle, 50);

    // Resume item
    auto resumeItem = MenuItemImage::create();
    resumeItem->setNormalImage(Sprite::createWithSpriteFrameName("ResumeButton.png"));
    resumeItem->setSelectedImage(Sprite::createWithSpriteFrameName("ResumeButtonL.png"));
    resumeItem->setCallback(CC_CALLBACK_1(PauseLayer::ResumeGame, this));
    resumeItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.5325f));

    // Settings item
    auto settingsItem = MenuItemImage::create();
    settingsItem->setNormalImage(Sprite::createWithSpriteFrameName("SettingsButton.png"));
    settingsItem->setSelectedImage(Sprite::createWithSpriteFrameName("SettingsButtonL.png"));
    settingsItem->setCallback(CC_CALLBACK_1(PauseLayer::SettingsGame, this));
    settingsItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.4f));

    // Quit item
    auto quitItem = MenuItemImage::create();
    quitItem->setNormalImage(Sprite::createWithSpriteFrameName("QuitButton.png"));
    quitItem->setSelectedImage(Sprite::createWithSpriteFrameName("QuitButtonL.png"));
    quitItem->setCallback(CC_CALLBACK_1(PauseLayer::QuitGame, this));
    quitItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.2675f));

    // Create menu
    menu = Menu::create(resumeItem, settingsItem, quitItem, NULL);
    menu->setPosition(Vec2(0.0f, 0.0f));
    this->addChild(menu, 50);

    // Show pause layer elements
    ShowMe();
}

void PauseLayer::ShowMe() {
    // reset position of elements
    pauseTitle->setPosition(Vec2(- pauseTitle->getContentSize().width, visibleSize.height * 0.8f));
    menu->setOpacity(0);

    // Title animation
    auto titleMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height * 0.8f));
    pauseTitle->runAction(titleMoveTo);

    // Menu animation        
    auto fadeIn = FadeIn::create(0.3f);
    menu->runAction(fadeIn);
}

void PauseLayer::HideMe(CallFunc* _callfunc) {
    // Title animation
    auto titleMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width + pauseTitle->getContentSize().width, visibleSize.height * 0.8f));
    auto seq = Sequence::create(titleMoveTo, _callfunc, nullptr);
    pauseTitle->runAction(seq);

    // Menu animation
    auto fadeOut = FadeOut::create(0.2f);
    menu->runAction(fadeOut);
}

void PauseLayer::callResume(Ref* pSender) {
    NebulaGame *game = (NebulaGame*)this->getParent();
    game->resumeGame();
    game->ShowMyHUD();
    this->removeFromParentAndCleanup(true);
}

void PauseLayer::callQuit(Ref* pSender) {
    NebulaGame *game = (NebulaGame*)this->getParent();
    game->stopAllSounds();

    // start main menu
    Scene *mScene = MainMenu::scene();
    Director::getInstance()->replaceScene(mScene);
}


void PauseLayer::callSettings(Ref* pSender) {
    Settings *settings = new Settings();
    NebulaGame *game = (NebulaGame*)this->getParent();
    game->addChild(settings, 50);
    this->removeFromParentAndCleanup(true);
}

void PauseLayer::ResumeGame(Ref* pSender) {
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    auto callbackResume = CallFuncN::create(CC_CALLBACK_1(PauseLayer::callResume, this));
    HideMe(callbackResume);
}

void PauseLayer::SettingsGame(Ref* pSender) {
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    auto callbackSettings = CallFuncN::create(CC_CALLBACK_1(PauseLayer::callSettings, this));
    HideMe(callbackSettings);
}

void PauseLayer::QuitGame(Ref* pSender) {
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    auto callbackResume = CallFuncN::create(CC_CALLBACK_1(PauseLayer::callQuit, this));
    HideMe(callbackResume);
}
