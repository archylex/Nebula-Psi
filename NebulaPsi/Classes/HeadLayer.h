#include "cocos2d.h"
#include <string>  

#pragma once

using namespace cocos2d;
using namespace std;

class HeadLayer : public Layer {
    public:
        HeadLayer();
        void SetScore(int _score);
        void UpdateHealth(float _health);
        void UpdateEnergy(float _energy);
        void UpdateOverheat(float _overheat);
        void UpdatePlasma(float _plasma);
        void HideMe();
        void ShowMe();
        void ResetMe();
        void lightHealth();
        void lightEnergy();
        void lightOverheat();
        void lightPlasma();
        void lightScore();
        void unlightScore();
        
    private:
        Sprite *head;
        Size visibleSize;
        Label *scoreLabel;
        void CreateLines(DrawNode *pNode, float Ax, float Ay, float Dx, float Dy);
        void UpdateLine(DrawNode *uNode, float Ax, float Ay, float Dx, float Dy, float _progress, bool _help);
        DrawNode *healthLine;
        DrawNode *energyLine;
        DrawNode *overheatLine;
        DrawNode *plasmaLine;

};
