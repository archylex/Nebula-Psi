#ifndef __GYROSCOPE__
#define __GYROSCOPE__

#include "cocos2d.h"

using namespace cocos2d;

class Gyroscope {
    public:
        static bool startProcess();
        static void stopProcess();
        static Vec2 getCoordinates();
};

#endif
