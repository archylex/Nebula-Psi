#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class Enemy: public Sprite {
    public:
        static Enemy *create(string _name);        
        void update(float dt);
        bool getSide();
        void setSide(bool _side);
        void setHealth(int _health);
        int getHealth();
        void resumeParticle();
        void pauseParticle();
        bool getDead();
      
    private:
        bool initialize(string _name);        
        Size visibleSize;
        bool side;
        int health;
        Sprite *firstEmitter;
        Sprite *secondEmitter;
};

#endif
