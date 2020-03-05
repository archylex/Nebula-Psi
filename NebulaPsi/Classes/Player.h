#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class Player: public Sprite {
    public:
        static Player *create(string _name);        
        virtual void update(float dt);         
        void setVX(float _x);
        void setVY(float _y);
        float getVX();
        float getVY();
        float getHealth();
        void setHealth(float _health);
        void setSide(bool _side);
        bool getSide();
        void setShield(bool _sw);
        bool getShield();
        void setEnergy(int _energy);
        int getEnergy();
        void setOverheat(int _overheat);
        int getOverheat();
        Vector<SpriteFrame*> getAnimation(const char *format, int start, int count);
        void pauseParticle();
        void resumeParticle();
        void setPlasmaRocket(float _plasma);
        float getPlasmaRocket();
        bool getDead();
        void setIsFire(bool _isfire);
        bool getIsFire();
        void pauseAnimationShield();
        void resumeAnimationShield();

    private:
        bool initialize(string _name); 
        Size visibleSize;
        bool shield;
        int energy;
        int overheat;
        float vX, vY; 
        float health;
        float plasma;
        bool isFire;        
        Sprite *firstEmitter;
        Sprite *secondEmitter;
        Sprite *firstEngine;
        Sprite *secondEngine;
        ParticleSystemQuad *emitter1, *emitter2;
        void createEmitter(ParticleSystemQuad *_psq, Vec2 _pos);
        bool side;
        Sprite *defendSprite;
        void afterStartShield(Ref* _sender);
        Action* defended;
        Animate *activateShield;
        Animate *deactivateShield;
        void stopShield();
        void startShield();
        float getAngle(float x, float y);
        void setEmitterAnimation();
};

#endif
