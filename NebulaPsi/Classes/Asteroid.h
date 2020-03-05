#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class Asteroid : public Sprite {
    public:
        static Asteroid *create(string _name, float _speed);        
        void update(float dt);
        float getMyWeight();
        void showExplode();
        bool getDead();
      
    private:
        bool initialize(string _name, float _speed);  
        void callDead(Ref* pSender);
        Animate *explode;
        float speed;  
        float mySize;  
        bool isDead;  
};

#endif
