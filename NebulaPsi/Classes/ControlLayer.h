#include "cocos2d.h"
#include "ui/CocosGUI.h"

#pragma once

using namespace cocos2d;
using namespace ui;

class ControlLayer : public Layer {
    public:
        ControlLayer();
        void SetPosCross(Vec2 _pos);
        Vec2 GetPosCross();
        void ResetPosCross();
        bool CrossZone(Vec2 _pos);
        void ShowMe();
        void HideMe();
        void ResetMe();
        void lightFireRocket(bool _is);
        void lightPlasmaRocket(bool _is);
        void lightShield(bool _is);
        void lightCross(bool _is);
        void lightMenu(bool _is);
        
    private:
        Sprite *control;
        Sprite *crossJoystick;
        Vec2 centerJoystick;
        Button *rocketButton;
        Button *pauseButton;
        Button *plasmaButton;
        Button *shieldButton;
        Size visibleSize;

};
