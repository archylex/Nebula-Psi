#include "GameOver.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"
#include "NebulaGame.h"
#include "PublicVariables.h"
#include "Utils.h"
#include <unistd.h>
#include "AddMob.h"

GameOver::GameOver(int _score) {
    // AdMob Initialize
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AddMob::showInterstitial();
    #endif

    // Screen size
    visibleSize = Director::getInstance()->getVisibleSize();

    // Fade background
    auto fadeBGNode = DrawNode::create();
    Vec2 fadeBG[] = { Vec2(0, 0), Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height), Vec2(visibleSize.width, 0) };
    Color4F aBlack(0.0f, 0.0f, 0.0f, 0.5f);
    fadeBGNode->drawPolygon(fadeBG, 4, aBlack, 0, aBlack);
    this->addChild(fadeBGNode, 48);

    // Title
    gameOverTitle = Sprite::createWithSpriteFrameName("GameOver.png");
    gameOverTitle->setPosition(Vec2(visibleSize.width/2, visibleSize.height + gameOverTitle->getContentSize().height));
    this->addChild(gameOverTitle, 50);

    // Player score frame
    scoreFrame = Sprite::createWithSpriteFrameName("Score.png");
    scoreFrame->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.6f));
    this->addChild(scoreFrame, 49);
    scoreFrame->setOpacity(0);

    // Score label    
    scoreLabel = Label::createWithBMFont("font.fnt", to_string(_score));
    scoreLabel->setPosition(Vec2(scoreFrame->getContentSize().width/2, scoreFrame->getContentSize().height * 0.45f));
    scoreLabel->setScale(0.6f);
    Color3B gb(0, 245, 206);
    scoreLabel->setColor(gb);
    scoreFrame->addChild(scoreLabel, 50);
    scoreLabel->setOpacity(0);

    // Restart item
    auto restartItem = MenuItemImage::create();
    restartItem->setNormalImage(Sprite::createWithSpriteFrameName("RestartButton.png"));
    restartItem->setSelectedImage(Sprite::createWithSpriteFrameName("RestartButtonL.png"));
    restartItem->setCallback(CC_CALLBACK_1(GameOver::RestartGame, this));
    restartItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.45f));

    // Quit item
    auto *quitItem = MenuItemImage::create();
    quitItem->setNormalImage(Sprite::createWithSpriteFrameName("QuitButton.png"));
    quitItem->setSelectedImage(Sprite::createWithSpriteFrameName("QuitButtonL.png"));
    quitItem->setCallback(CC_CALLBACK_1(GameOver::QuitGame, this));
    quitItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.3f));

    // Create menu
    menu = Menu::create(restartItem, quitItem, NULL);
    menu->setPosition(Vec2(0.0f, -visibleSize.height));
    this->addChild(menu, 50);

    // Show elements with animation
    ShowMe();

    // save hiscore
    if (PublicVariables::Instance()->getHighScore() < _score) {
        // Set public variable
        PublicVariables::Instance()->setHighScore(_score);

        // Save to shared
        UserDefault::getInstance()->setStringForKey("SCORES", Utils::encryptScore(_score));
    }
}

void GameOver::ShowMe() {
    // Title animation
    auto titleMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height * 0.8f));
    gameOverTitle->runAction(titleMoveTo);

    // Score frame animation
    auto fFadeIn = FadeIn::create(0.3f);
    scoreFrame->runAction(fFadeIn);

    auto lFadeIn = FadeIn::create(0.3f);
    scoreLabel->runAction(lFadeIn);

    // Menu animation
    auto menuMoveTo = MoveTo::create(0.2f, Vec2(0.0f, 0.0f));
    menu->runAction(menuMoveTo);
}

void GameOver::HideMe(CallFunc *_callfunc) {
    // Title animation
    auto titleMoveTo = MoveTo::create(0.2f, Vec2(visibleSize.width/2, visibleSize.height + gameOverTitle->getContentSize().height));
    gameOverTitle->runAction(titleMoveTo);

    // Score frame animation
    auto fFadeOut = FadeOut::create(0.2f);
    scoreFrame->runAction(fFadeOut);

    auto lFadeOut = FadeOut::create(0.2f);
    scoreLabel->runAction(lFadeOut);

    // Menu animation
    auto menuMoveTo = MoveTo::create(0.2f, Vec2(0.0f, -visibleSize.height));
    auto seq = Sequence::create(menuMoveTo, _callfunc, nullptr);
    menu->runAction(seq);
}

void GameOver::RestartGame(Ref* pSender) {
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    NebulaGame *game = (NebulaGame*)this->getParent();
    game->stopAllSounds();
    auto cb = CallFunc::create([](){
        auto restartScene = NebulaGame::createScene();
        Director::getInstance()->replaceScene(restartScene);
    });
    HideMe(cb);
}

void GameOver::QuitGame(Ref* pSender) {
    if(PublicVariables::Instance()->getSFX()) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("click.mp3");
    }

    NebulaGame *game = (NebulaGame*)this->getParent();
    game->stopAllSounds();
    auto cb = CallFunc::create([](){        
        // start main menu
        Scene *mScene = MainMenu::scene();
        Director::getInstance()->replaceScene(mScene);
    });
    HideMe(cb);
}
