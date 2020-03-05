#include <iostream>
#include "cocos2d.h"
#include "PublicVariables.h"

using namespace std;
using namespace cocos2d;

class Weapon: public Sprite {
    public:
        static Weapon *create(string _name, bool _isEnemy, int _type);
        void update(float dt);
        void setStartPosition(Vec2 _pos);
        float getYPosition();
        int getType();
        bool getIsDead();
        void setIsDead();
        void resumeParticle();
        void pauseParticle();
      
    private:
        bool initialize(string _name, bool _isEnemy, int _type);
        float speed;
        int typeWeapon;   
        bool isDead;
        Sprite *emitter;
};


