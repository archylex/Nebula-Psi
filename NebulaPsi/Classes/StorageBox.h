#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class StorageBox: public Sprite {
    public:
        static StorageBox *create(string _name, int _boxType, Vec2 _pos);        
        void update(float dt);
        int getBoxType();
        void setDead(bool _isDead);
        bool getDead();
      
    private:
        bool initialize(string _name, int _boxType, Vec2 _pos);    
        int boxType;    
        bool isDead;
        
};
