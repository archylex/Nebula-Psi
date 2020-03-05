#ifndef ANIMATIONTOOLS_H
#define ANIMATIONTOOLS_H

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class AnimationTools {
    public:
        static Vector<SpriteFrame*> getAnimation(const char *format, int start, int count);
};

#endif // ANIMATION_H
