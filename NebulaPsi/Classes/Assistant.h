#include "cocos2d.h"

#pragma once

using namespace cocos2d;
using namespace std;

class Assistant : public Layer {
    public:
        Assistant();
        void setText(string _text);
        void setMode(int _mode);
        int getMode();
        
    private:
        Label *label;
        int mode;

};
