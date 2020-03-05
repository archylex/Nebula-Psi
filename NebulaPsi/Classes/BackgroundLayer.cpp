#include "BackgroundLayer.h"
#include "PublicVariables.h"

// Create background layer
BackgroundLayer::BackgroundLayer() {
    nebula_bg = Background::create("NebulaBackground.png");
    this->addChild(nebula_bg);

    // Asteroid schedule
    this->schedule(schedule_selector(BackgroundLayer::createAsteroid), PublicVariables::Instance()->getAsteroidBGTimer());
}

// Update background
void BackgroundLayer::update(float dt) {
    // Update background picture
    nebula_bg->update(dt);

    // Update asteroids
    if (asteroids.size() > 0) {
        for (auto &nextAsteroid : asteroids) {
            auto _asteroid = nextAsteroid;

            _asteroid->update(dt);

            // Check screen border
            if (_asteroid->getPosition().y < -_asteroid->getContentSize().height) {
                // Delete from vector
                asteroids.eraseObject(_asteroid);

                // Delete from layer
                this->removeChild(_asteroid, true);
            }
        }
    }
}

// Create asteroid
void BackgroundLayer::createAsteroid(float dt) {
    // Asteroid sprite names
    string name[] = { "AsteroidBG1.png", "AsteroidBG2.png", "AsteroidBG3.png" };

    // Random asteroid sprite name
    int rn = rand() % int(sizeof(name)/sizeof(*name));

    // Create asteroid sprite
    Asteroid *_asteroid = Asteroid::create(name[rn], PublicVariables::Instance()->getAsteroidBGSpeed());

    // Add to layer
    this->addChild(_asteroid);

    // Add to vector
    asteroids.pushBack(_asteroid);
}

void BackgroundLayer::pauseAsteroid() {
    this->unschedule(schedule_selector(BackgroundLayer::createAsteroid));
}

void BackgroundLayer::resumeAsteroid() {
    this->schedule(schedule_selector(BackgroundLayer::createAsteroid), 2.0f);
}




