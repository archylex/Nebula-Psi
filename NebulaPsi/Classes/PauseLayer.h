#include "cocos2d.h"

#pragma once

using namespace cocos2d;
using namespace std;

class PauseLayer : public Layer {
    public:
        PauseLayer();

    private:
        Sprite *title;
        Size visibleSize;
        Menu *menu;
        Sprite *pauseTitle;
        void ResumeGame(Ref* pSender);
        void SettingsGame(Ref* pSender);
        void QuitGame(Ref* pSender);
        void callResume(Ref* pSender);
        void callSettings(Ref* pSender);
        void callQuit(Ref* pSender);
        void ShowMe();
        void HideMe(CallFunc* _callfunc);
};
