#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class Background: public Node {
    public:
        static Background *create(string _name);        
        void update(float dt);

    private:
        bool initialize(string _name);        
        void respawn(Sprite *_background);
        void move(Sprite *_background, float dt);
        Sprite *first_background, *second_background;
        string name;
};

#endif
