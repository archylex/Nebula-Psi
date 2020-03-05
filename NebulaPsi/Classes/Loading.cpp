#include "Loading.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "PublicVariables.h"
#include "Utils.h"

// Create Loading scene
Scene* Loading::scene() {
    return Loading::create();
}

// Initialize scene
bool Loading::init() {
    // Screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // First circle sprite
    auto cOne = Sprite::create("ct1.png");
    cOne->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(cOne);
    auto animOne = RotateBy::create(2.0f, Vec3(0.0f, 0.0f, -360.0f));
    cOne->runAction(RepeatForever::create(animOne));

    // Second cicrle sprite
    auto cTwo = Sprite::create("ct2.png");
    cTwo->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    cTwo->setScale(0.75f);
    this->addChild(cTwo);
    auto animTwo = RotateBy::create(1.0f, Vec3(0.0f, 0.0f, 360.0f));
    cTwo->runAction(RepeatForever::create(animTwo));

    // Third circle sprite
    auto cThree = Sprite::create("ct1.png");
    cThree->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    cThree->setScale(0.5f);
    this->addChild(cThree);
    auto animThree = RotateBy::create(5.0f, Vec3(0.0f, 0.0f, -360.0f));
    cThree->runAction(RepeatForever::create(animThree));

    // Percent label
    percLabel = Label::createWithBMFont("font.fnt", "0");
    percLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    percLabel->setScale(0.3f);
    this->addChild(percLabel);

    // Get load
    this->schedule(schedule_selector(Loading::loadSources), 0.5f);

    return true;
}

void Loading::loadSources(float dt) {
    // Unschedule
    this->unschedule(schedule_selector(Loading::loadSources));

    // Loading sprites
    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("spritesheet.plist");
    spritecache->addSpriteFramesWithFile("spritesheet-0.plist");    
    spritecache->addSpriteFramesWithFile("spritesheet-1.plist");
    spritecache->addSpriteFramesWithFile("spritesheet-2.plist");
    spritecache->addSpriteFramesWithFile("spritesheet-3.plist");
    percLabel->setString("20");

    // Get audio engine instance
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

    // Loading music
    audioEngine->preloadBackgroundMusic("theme.mp3");

    // Loading missile sound
    audioEngine->preloadEffect("Missile.mp3");

    // Loading explosion sound
    audioEngine->preloadEffect("MissileExplosion.mp3");
    audioEngine->preloadEffect("ShipExplosion.mp3");

    // Loading pick up sound
    audioEngine->preloadEffect("Pickup.mp3");

    // Shield ON
    audioEngine->preloadEffect("ShieldOn.mp3");

    // Shield OFF
    audioEngine->preloadEffect("ShieldOff.mp3");

    // Loading click sound
    audioEngine->preloadEffect("click.mp3");

    // Loading high score sound
    audioEngine->preloadEffect("highscore.mp3");
    percLabel->setString("55");

    // Loading high score and set to public variable
    int highScore = Utils::decryptScore(UserDefault::getInstance()->getStringForKey("SCORES", Utils::getSaltKey()));
    PublicVariables::Instance()->setHighScore(highScore);

    // Loading sound option and set to public variable
    PublicVariables::Instance()->setSFX(UserDefault::getInstance()->getBoolForKey("SFX", true));

    // Loading music option and set to public variable
    PublicVariables::Instance()->setMusic(UserDefault::getInstance()->getBoolForKey("MUSIC", true));

    // Loading assistant option and set to public variable
    PublicVariables::Instance()->setAssistant(UserDefault::getInstance()->getBoolForKey("ASSISTANT", true));

    // Loading gyroscope option and set to public variable
    PublicVariables::Instance()->setGyroscope(UserDefault::getInstance()->getBoolForKey("GYROSCOPE", false));
    percLabel->setString("100");

    // Go to main menu scene
    Scene *myScene = MainMenu::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, myScene, Color3B(0, 0, 0)));
}

