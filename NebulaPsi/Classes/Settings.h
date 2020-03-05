#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;
using namespace std;

class Settings : public Layer {
    public:
        Settings();
        void soundClicked(Ref* pSender);
        void musicClicked(Ref* pSender);
        void controlClicked(Ref* pSender);
        void assistantClicked(Ref* pSender);
        void animateFromMainMenu();
        void animateFromPauseMenu();
    		
    private:         
        MenuItemImage *musicItem;
        MenuItemImage *soundItem;
        MenuItemImage *controlItem;
        MenuItemImage *assistantItem;
        Sprite *settingsTitle;
        Sprite *settingsMenu;
        Button *backButton;
        Menu *pMenu;
        Size visibleSize;
        bool isSFX;
        bool isMusic;
        bool isAssistant;
        bool isGyro;
        bool isMainMenu;
        int b2i(bool i);
        void ShowMe();
        void HideMe(CallFunc *_callfunc);
        void callBackGame(Ref *pSender);
        void callBackMenu(Ref *pSender);
};

#endif 
