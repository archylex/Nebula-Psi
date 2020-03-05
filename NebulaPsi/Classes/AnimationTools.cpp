#include "AnimationTools.h"

// Get frames for animation
Vector<SpriteFrame*> AnimationTools::getAnimation(const char *format, int start, int count) {
    auto spritecache = SpriteFrameCache::getInstance();

    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = start; i <= count; i++)
    {
        sprintf(str, format, abs(i));
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}
