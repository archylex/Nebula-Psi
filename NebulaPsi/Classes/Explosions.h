#ifndef EXPLOSIONS_H
#define EXPLOSIONS_H

#include "cocos2d.h"

using namespace cocos2d;

class Explosions : public Sprite {
    public:
        static Explosions *create(int _type, Vec2 _pos);

    private:
        bool initialize(int _type, Vec2 _pos);
        void deleteExplosion(Ref *_sender);

};

#endif // EXPLOSIONS_H
