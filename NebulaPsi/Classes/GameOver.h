#include "cocos2d.h"

#pragma once

using namespace std;
using namespace cocos2d;

class GameOver : public Layer {
    public:
        GameOver(int _score);

    private: 
        Sprite *gameOverTitle;
        Sprite *scoreFrame;
        Label *scoreLabel;    
        Menu *menu;
        Size visibleSize;
        void ShowMe();
        void HideMe(CallFunc *_callfunc);
        void RestartGame(Ref* pSender);
        void QuitGame(Ref* pSender);

};
