#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class Loading : public Scene {
    public:
        virtual bool init();  
    	static Scene *scene();
    	
    private:
        Label *percLabel;
        void loadSources(float dt);
        	
    CREATE_FUNC(Loading)
};

#endif 
