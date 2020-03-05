#include "Settings.h"
#include "MainMenu.h"
#include "NebulaGame.h"
#include "PauseLayer.h"
#include "SimpleAudioEngine.h"
#include "PublicVariables.h"

Settings::Settings() {
    // Screen size
    visibleSize = Director::getInstance()->getVisibleSize();

    // Fade
    auto fadeBGNode = DrawNode::create();
    Vec2 fadeBG[] = { Vec2(0, 0), Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height), Vec2(visibleSize.width, 0) };
    Color4F aBlack(0.0f, 0.0f, 0.0f, 0.5f);
    fadeBGNode->drawPolygon(fadeBG, 4, aBlack, 0, aBlack);
    this->addChild(fadeBGNode, 7);

    // Background settings menu
    settingsMenu = Sprite::createWithSpriteFrameName("SettingsMenu.png");
    settingsMenu->setAnchorPoint(Vec2(0.5f, 0.5f));
    settingsMenu->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(settingsMenu,10);
    settingsMenu->setOpacity(0);

    // Get SFX option
    isSFX = PublicVariables::Instance()->getSFX();

    // Get Music option
    isMusic = PublicVariables::Instance()->getMusic();

    // Get assistant option
    isAssistant = PublicVariables::Instance()->getAssistant();

    // Get gyroscope option
    isGyro = PublicVariables::Instance()->getGyroscope();

    // Title
    settingsTitle = Sprite::createWithSpriteFrameName("SettingsTitle.png");
    settingsTitle->setAnchorPoint(Vec2(0.5f, 0.5f));
    settingsTitle->setPosition(Vec2(visibleSize.width/2, visibleSize.height + settingsTitle->getContentSize().height));
    this->addChild(settingsTitle,10);

    // Image button names
    string selectedName[] = { "UnselectButton.png", "SelectButton.png" };

    // Sound item
    soundItem = MenuItemImage::create();
    soundItem->setNormalImage(Sprite::createWithSpriteFrameName(selectedName[b2i(isSFX)]));
    soundItem->setCallback(CC_CALLBACK_1(Settings::soundClicked, this));
    soundItem->setPosition(Vec2(settingsMenu->getContentSize().width * 0.74f, settingsMenu->getContentSize().height * 0.72535f));

    // Music item
    musicItem = MenuItemImage::create();
    musicItem->setNormalImage(Sprite::createWithSpriteFrameName(selectedName[b2i(isMusic)]));
    musicItem->setCallback(CC_CALLBACK_1(Settings::musicClicked, this));
    musicItem->setPosition(Vec2(settingsMenu->getContentSize().width * 0.74f, settingsMenu->getContentSize().height * 0.58099f));

    // Gyroscope item
    controlItem = MenuItemImage::create();
    controlItem->setNormalImage(Sprite::createWithSpriteFrameName(selectedName[b2i(isGyro)]));
    controlItem->setCallback(CC_CALLBACK_1(Settings::controlClicked, this));
    controlItem->setPosition(Vec2(settingsMenu->getContentSize().width * 0.74f, settingsMenu->getContentSize().height * 0.43662f));

    // Assistant item
    assistantItem = MenuItemImage::create();
    assistantItem->setNormalImage(Sprite::createWithSpriteFrameName(selectedName[b2i(isAssistant)]));
    assistantItem->setCallback(CC_CALLBACK_1(Settings::assistantClicked, this));
    assistantItem->setPosition(Vec2(settingsMenu->getContentSize().width * 0.74f, settingsMenu->getContentSize().height * 0.29577f));

    // Create menu
    pMenu = Menu::create(soundItem, musicItem, controlItem, assistantItem, NULL);
    pMenu->setPosition(Vec2(0.0f, 0.0f));
    settingsMenu->addChild(pMenu, 10);
    pMenu->setOpacity(0);

    // back button
    backButton = Button::create("BackButton.png", "BackButtonL.png", "BackButton.png", Widget::TextureResType::PLIST);
    backButton->setAnchorPoint(Vec2(0.5f,0.5f));
    backButton->setPosition(Vec2(visibleSize.width/2, -backButton->getContentSize().height));
    backButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                // Set public variables
                PublicVariables::Instance()->setSFX(isSFX);
                PublicVariables::Instance()->setMusic(isMusic);
                PublicVariables::Instance()->setAssistant(isAssistant);
                PublicVariables::Instance()->setGyroscope(isGyro);

                // Save to shared
                UserDefault::getInstance()->setBoolForKey("SFX", isSFX);
                UserDefault::getInstance()->setBoolForKey("MUSIC", isMusic);
                UserDefault::getInstance()->setBoolForKey("GYROSCOPE", isGyro);
                UserDefault::getInstance()->setBoolForKey("ASSISTANT", isAssistant);
                UserDefault::getInstance()->flush();

                // Play click sound
                if(PublicVariables::Instance()->getSFX()) {
                    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
                    audio->playEffect("click.mp3");
                }

                // Get parent layer
                auto layer = (Layer*) this->getParent();

                // Remove settings layer from parent scene
                if(SCENE::MAINMENU == layer->getTag()) {
                    auto cb = CallFuncN::create(CC_CALLBACK_1(Settings::callBackMenu, this));
                    HideMe(cb);
                } else {
                    auto cb = CallFuncN::create(CC_CALLBACK_1(Settings::callBackGame, this));
                    HideMe(cb);
                }
                break;            
        }
    });
    this->addChild(backButton,10);

    // Show settings elements
    ShowMe();
}

// Show settings layer elements
void Settings::ShowMe() {
    // Title animation
    settingsTitle->setPosition(Vec2(visibleSize.width/2, visibleSize.height +settingsTitle->getContentSize().height/2));
    auto titleMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height * 0.8f));
    settingsTitle->runAction(titleMoveTo);

    // Back button animation
    backButton->setPosition(Vec2(visibleSize.width/2, - backButton->getContentSize().height/2));
    auto backMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height * 0.2f));
    backButton->runAction(backMoveTo);

    // Background menu animation
    auto sfadeIn = FadeIn::create(0.2f);
    settingsMenu->runAction(sfadeIn);

    // Menu animation
    auto mfadeIn = FadeIn::create(0.2f);
    pMenu->runAction(mfadeIn);
}

// Hide settings elements
void Settings::HideMe(CallFunc *_callfunc) {
    // Title animation
    auto titleMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height + settingsTitle->getContentSize().height/2));
    settingsTitle->runAction(titleMoveTo);

    // Back button animation
    auto backMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, -backButton->getContentSize().height));
    auto seq = Sequence::create(backMoveTo, _callfunc, nullptr);
    backButton->runAction(seq);

    // Background menu animation
    auto sfadeOut = FadeOut::create(0.2f);
    settingsMenu->runAction(sfadeOut);

    // Menu animation
    auto mfadeOut = FadeOut::create(0.2f);
    pMenu->runAction(mfadeOut);
}

// Go to main menu
void Settings::callBackMenu(Ref *pSender) {
    MainMenu *mainMenu = (MainMenu*)this->getParent();
    mainMenu->ShowMe();
    this->removeFromParentAndCleanup(true);
}

// Go to pause menu (game)
void Settings::callBackGame(Ref *pSender) {
    // Get parent scene
    NebulaGame *game = (NebulaGame*)this->getParent();

    // Create pause layer
    PauseLayer *pauseLayer = new PauseLayer();
    game->addChild(pauseLayer, 90);

    // Update parameters (music)
    game->updateMusic();

    // Delete this
    this->removeFromParentAndCleanup(true);
}

// Sound option
void Settings::soundClicked(Ref* pSender) {
    if(isSFX) {
        soundItem->setNormalImage(Sprite::createWithSpriteFrameName("UnselectButton.png"));
        isSFX = false;        
    } else {
        soundItem->setNormalImage(Sprite::createWithSpriteFrameName("SelectButton.png"));
        isSFX = true;
    }
}

// Music option
void Settings::musicClicked(Ref* pSender) {
    if(isMusic) {
        musicItem->setNormalImage(Sprite::createWithSpriteFrameName("UnselectButton.png"));
        isMusic = false;
    } else {
        musicItem->setNormalImage(Sprite::createWithSpriteFrameName("SelectButton.png"));
        isMusic = true;
    }
}

// Gyroscope option
void Settings::controlClicked(Ref* pSender) {
    if(isGyro) {
        controlItem->setNormalImage(Sprite::createWithSpriteFrameName("UnselectButton.png"));
        isGyro = false;
    } else {
        controlItem->setNormalImage(Sprite::createWithSpriteFrameName("SelectButton.png"));
        isGyro = true;
    }
}

// Assistant option
void Settings::assistantClicked(Ref* pSender) {
    if(isAssistant) {
        assistantItem->setNormalImage(Sprite::createWithSpriteFrameName("UnselectButton.png"));
        isAssistant = false;
    } else {
        assistantItem->setNormalImage(Sprite::createWithSpriteFrameName("SelectButton.png"));
        isAssistant = true;
    }
}

// Bool to int
int Settings::b2i(bool i) {
    if (i) return 1;
    return 0;
}

