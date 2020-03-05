#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "Settings.h"

using namespace cocos2d;

class MainMenu : public Scene {
    public:
        virtual bool init();  
    	static Scene *scene();
    	void playGame(Ref* pSender);
        void settingsLayer(Ref* pSender);
        void menuCloseCallback(Ref* pSender);
        void ShowMe();
        Sprite *logo;
        Sprite *footer;
        Sprite *header;

    private:
        Menu *pMenu;
        Size visibleSize;
        Label *scoreLabel;
        Vector<Sprite*> asteroids;
        Vector<Sprite*> circles;
        Vector<Sprite*> arcs;
        DrawNode *fadeBGNode;
        void CreateAsteroid(float dt);
        void update(float dt);
        void callShowSettings(Ref *pSender);
        void HideMe(CallFunc *_callfunc);
        void addAsteroids();
	
    CREATE_FUNC(MainMenu)
};

#endif 
