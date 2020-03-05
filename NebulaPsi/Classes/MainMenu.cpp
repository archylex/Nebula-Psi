#include "MainMenu.h"
#include "NebulaGame.h"
#include "Settings.h"
#include "SimpleAudioEngine.h"
#include "PublicVariables.h"
#include "Utils.h"
#include <unistd.h>

// Create scene
Scene* MainMenu::scene() {
    return MainMenu::create();
}

// Initialize scene
bool MainMenu::init() {
    // Get sprite cache
    auto spritecache = SpriteFrameCache::getInstance();

    // Get screen size
    visibleSize = Director::getInstance()->getVisibleSize();

    // Set tag for orientation in settings layer
    this->setTag(SCENE::MAINMENU);

    // Set first tutorial
    if (PublicVariables::Instance()->getAssistant())
        PublicVariables::Instance()->setFirstTutorial(true);

    // Fade background
    fadeBGNode = DrawNode::create();
    Vec2 fadeBG[] = { Vec2(0, 0), Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height), Vec2(visibleSize.width, 0) };
    Color4F aBlack(0.0f, 0.0f, 0.0f, 0.5f);
    fadeBGNode->drawPolygon(fadeBG, 4, aBlack, 0, aBlack);
    this->addChild(fadeBGNode, 7);

    // Background picture
    auto bg = Sprite::createWithSpriteFrameName("NebulaBackground.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg, 1);

    // Header with high score
    header =  Sprite::createWithSpriteFrameName("header.png");
    header->setAnchorPoint(Vec2(0.5f, 1.0f));
    header->setPosition(Vec2(visibleSize.width/2, visibleSize.height + header->getContentSize().height));
    this->addChild(header, 8);

    // Footer with logo
    footer =  Sprite::createWithSpriteFrameName("footer.png");
    footer->setAnchorPoint(Vec2(0.5f, 0.0f));
    footer->setPosition(Vec2(visibleSize.width/2,  -footer->getContentSize().height));
    this->addChild(footer, 8);

    // Score label from header
    scoreLabel = Label::createWithBMFont("font.fnt", to_string(PublicVariables::Instance()->getHighScore()));
    scoreLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    scoreLabel->setPosition(Vec2(header->getContentSize().width/2, header->getContentSize().height/2.0f));
    scoreLabel->setScale(0.5f);
    Color3B gb(0, 245, 206);
    scoreLabel->setColor(gb);
    header->addChild(scoreLabel, 10);

    // Game title
    logo =  Sprite::createWithSpriteFrameName("Logo.png");
    logo->setPosition(Vec2(-logo->getContentSize().width, visibleSize.height * 0.735f));
    this->addChild(logo, 10);

    // Create menu items
    Vector<MenuItem*> MenuItems;

    // Play
    auto pPlayItem = MenuItemImage::create();
    pPlayItem->setNormalSpriteFrame(spritecache->getSpriteFrameByName("StartButton.png"));
    pPlayItem->setSelectedSpriteFrame(spritecache->getSpriteFrameByName("StartButtonL.png"));
    pPlayItem->setCallback(CC_CALLBACK_1(MainMenu::playGame, this));
    pPlayItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.4925f));
    MenuItems.pushBack(pPlayItem);

    // Settings
    auto pOptionsItem = MenuItemImage::create();
    pOptionsItem->setCallback(CC_CALLBACK_1(MainMenu::settingsLayer, this));
    pOptionsItem->setNormalSpriteFrame(spritecache->getSpriteFrameByName("SettingsButton.png"));
    pOptionsItem->setSelectedSpriteFrame(spritecache->getSpriteFrameByName("SettingsButtonL.png"));
    pOptionsItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.36f));
    MenuItems.pushBack(pOptionsItem);

    // Quit
    auto pQuitItem = MenuItemImage::create();
    pQuitItem->setCallback(CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    pQuitItem->setNormalSpriteFrame(spritecache->getSpriteFrameByName("QuitButton.png"));
    pQuitItem->setSelectedSpriteFrame(spritecache->getSpriteFrameByName("QuitButtonL.png"));
    pQuitItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.2275f));
    MenuItems.pushBack(pQuitItem);

    // Create menu
    pMenu = Menu::createWithArray(MenuItems);
    pMenu->setPosition(Vec2(0.0f, 0.0f));
    this->addChild(pMenu, 10);
    pMenu->setOpacity(0);

    // Update scene
    this->scheduleUpdate();

    // Add asteroids to background
    addAsteroids();

    // Show scene elements
    ShowMe();

    return true;
}

// Hide scene elements
void MainMenu::HideMe(CallFunc *_callfunc) {
    // Footer animation
    auto footerMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2,  -footer->getContentSize().height));
    footer->runAction(footerMoveTo);

    // Logo animation
    auto logoMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width + logo->getContentSize().width, visibleSize.height * 0.735f));
    logo->runAction(logoMoveTo);

    // Menu animation
    auto fadeOut = FadeOut::create(0.2f);
    pMenu->runAction(fadeOut);

    // Callback
    auto headerMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height + header->getContentSize().height));
    auto seq = Sequence::create(headerMoveTo, _callfunc, nullptr);
    header->runAction(seq);
}

// Add asteroids
void MainMenu::addAsteroids() {
    // Add first asteroid
    CreateAsteroid(1.0);

    // Create an asteroid every 3.5 minutes.
    this->schedule(schedule_selector(MainMenu::CreateAsteroid), 3.5f);
}

// Play callback
void MainMenu::playGame(Ref* pSender) {
    // Sound
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    // Go to game scene
    auto cf = CallFunc::create([](){
        //sleep(0.5f);
        Scene *mScene = NebulaGame::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, mScene, Color3B(0, 0, 0)));
    });

    // Hide menu scene elements
    HideMe(cf);
}

// Settings callback
void MainMenu::settingsLayer(Ref* pSender) {
    // Sound
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    // Show settings layer
    auto cf = CallFuncN::create(CC_CALLBACK_1(MainMenu::callShowSettings, this));

    // Hide menu scene elements
    HideMe(cf);
}

// Show settings layer (callback from animation)
void MainMenu::callShowSettings(Ref *pSender) {
    // Disable menu fade
    fadeBGNode->setVisible(false);

    // Disable menu
    pMenu->setVisible(false);

    // Create settings layer
    Settings *settings = new Settings();
    this->addChild(settings, 11);
}

// Quit callback
void MainMenu::menuCloseCallback(Ref* pSender) {
    // Sound
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    // Callback function
    auto cf = CallFunc::create([](){
        // Close application
        Director::getInstance()->end();

        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
        #endif
    });

    // Hide menu scene elements
    HideMe(cf);
}

// Show menu scene elements
void MainMenu::ShowMe() {
    // Enable fade
    fadeBGNode->setVisible(true);

    // Enable menu
    pMenu->setVisible(true);

    // Header animation
    auto headerMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height));
    header->runAction(headerMoveTo);

    // Footer animation
    auto footerMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, 0.0f));
    footer->runAction(footerMoveTo);

    // Title animation
    auto logoMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height * 0.735f));
    logo->runAction(logoMoveTo);

    // Menu animation
    auto fadeIn = FadeIn::create(0.2f);
    pMenu->runAction(fadeIn);
}

// Update scene
void MainMenu::update(float dt) {
    // Asteroid velocity
    int speed = 2;

    // Asteroids
    if (asteroids.size() > 0) {
        for (auto &nextAsteroid : asteroids) {
            // Get link
            auto _aster = nextAsteroid;

            // Get position
            Vec2 pos = _aster->getPosition();

            // Set new position
            _aster->setPosition(Vec2(pos.x, pos.y - speed/2));

            // Set new rotation
            _aster->setRotation(_aster->getRotation() + speed/2);

            // Check screen border
            if (_aster->getPosition().y < -_aster->getContentSize().height) {
                asteroids.eraseObject(_aster);
                this->removeChild(_aster, true);
            }
        }
    }

    // Circles
    if (circles.size() > 0) {
        for (auto &nextCircle : circles) {
            // Get link
            auto _circle = nextCircle;

            // Get position
            Vec2 pos = _circle->getPosition();

            // Set new position
            _circle->setPosition(Vec2(pos.x, pos.y - speed/2));

            // Set new rotation
            _circle->setRotation(_circle->getRotation() + speed);

            // Check screen bornextCircleder
            if (_circle->getPosition().y < -_circle->getContentSize().height) {
                circles.eraseObject(_circle);
                this->removeChild(_circle, true);
            }
        }
    }

    // Arcs
    if (arcs.size() > 0) {
        for (auto &nextArc : arcs) {
            // Get link
            auto _arc = nextArc;

            // Get position
            Vec2 pos = _arc->getPosition();

            // Set new position
            _arc->setPosition(Vec2(pos.x, pos.y - speed/2));

            // Set new rotation
            _arc->setRotation(_arc->getRotation() - speed);

            // Check screen border
            if (_arc->getPosition().y < -_arc->getContentSize().height) {
                arcs.eraseObject(_arc);
                this->removeChild(_arc, true);
            }
        }
    }
}

// Create asteroid
void MainMenu::CreateAsteroid(float dt) {
    // Asteroid scale randomize
    float scale = float(rand() % 9 + 1) / 10.0f;

    // asteroid types names
    string name[] = { "AsteroidFG1.png", "AsteroidFG2.png", "AsteroidFG3.png" };

    // Get random number of asteroid type
    int rn = rand() % int(sizeof(name)/sizeof(*name));

    // Create asteroid sprite
    auto ast = Sprite::createWithSpriteFrameName(name[rn]);

    // Set anchor point of asteroid
    ast->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Set scale of asteroid
    ast->setScale(scale);

    // Get random position
    float x = rand() % int(visibleSize.width-(2*ast->getContentSize().width*scale)) + int(ast->getContentSize().width*scale);
    float y = visibleSize.height + ast->getContentSize().height/2;

    // Set asteroid position
    ast->setPosition(Vec2(x, y));

    // Set asteroid rotation
    ast->setRotation(rand() % 360);

    // Add asteroid to scene
    this->addChild(ast, 5);

    // Add asteroid to vector
    asteroids.pushBack(ast);

    // Create arc sprite
    auto arc = Sprite::createWithSpriteFrameName("ct2.png");

    // Set anchor point of arc
    arc->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Set scale of arc
    arc->setScale(scale * 0.8f);

    // Set arc position
    arc->setPosition(Vec2(x, y));

    // Add arc to scene
    this->addChild(arc, 6);

    // Add arc to vector
    arcs.pushBack(arc);

    // Create circle sprite
    auto cir = Sprite::createWithSpriteFrameName("ct1.png");

    // Set anchor point of circle
    cir->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Set scale of circle
    cir->setScale(scale);

    // Set circle position
    cir->setPosition(Vec2(x, y));

    // Add circle to scene
    this->addChild(cir, 6);

    // Add circle to vector
    circles.pushBack(cir);
}
