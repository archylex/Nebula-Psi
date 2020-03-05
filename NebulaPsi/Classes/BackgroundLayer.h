#ifndef __BACKGROUND_LAYER_H__
#define __BACKGROUND_LAYER_H__

#include <iostream>
#include "Background.h"
#include "Asteroid.h"

using namespace std;
using namespace cocos2d;

class BackgroundLayer: public Layer {
    public:
        BackgroundLayer();
        void update(float dt);
        void pauseAsteroid();
        void resumeAsteroid();

    private:    
        Background *nebula_bg;
        Background *asteroids_bg;
        Asteroid *asteroid;
        void createAsteroid(float dt);
        Vector<Asteroid*> asteroids;
};

#endif 
