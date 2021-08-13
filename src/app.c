#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include <defines.h>
#include <scene.h>

#include "app.h"

struct s_app{
    int                     iWidth,
                            iHeight;
    
    SDL_Window*             pWindow;
    SDL_Renderer*           pRenderer;
    t_scene*                pScene;
};

t_app* AppNew(char* sTitle, int iWidth, int iHeight){
    t_app* pApp = (t_app*)malloc(sizeof(t_app));
    
    pApp->iWidth = iWidth;
    pApp->iHeight = iHeight;

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER);

    pApp->pWindow = SDL_CreateWindow(
        sTitle,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        iWidth,
        iHeight,
        0
    );
    
    pApp->pRenderer = SDL_CreateRenderer(
        pApp->pWindow,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_GameControllerOpen(0);
    pApp->pScene = SceneNew(pApp->pWindow, pApp->pRenderer, pApp->iWidth, pApp->iHeight);

    return pApp;
}

t_app* AppDel(t_app* pApp){
    SceneDel(pApp->pScene);
    SDL_DestroyRenderer(pApp->pRenderer);
    SDL_DestroyWindow(pApp->pWindow);
    SDL_Quit();
    free(pApp);
    return pApp=NULL;
}

int AppRun(t_app* pApp){
    char buf[32] = "";
    int quit = 0;
    SDL_Event event;
    SceneDraw(pApp->pScene);
    while(!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    quit=1;
                    break;
                case SDLK_SPACE:
                	if(SceneGetGameStatus(pApp->pScene)){
                        pApp->pScene = SceneDel(pApp->pScene);
                        pApp->pScene = SceneNew(pApp->pWindow, pApp->pRenderer, pApp->iWidth, pApp->iHeight);
                        SceneDraw(pApp->pScene);
    			    }
                    break;
                default:
                    break;
                }
            break;
            case SDL_CONTROLLERBUTTONDOWN:
                switch(event.cbutton.button){
                case SDL_CONTROLLER_BUTTON_START:
                   if(SceneGetGameStatus(pApp->pScene)){
                        pApp->pScene = SceneDel(pApp->pScene);
                        pApp->pScene = SceneNew(pApp->pWindow, pApp->pRenderer, pApp->iWidth, pApp->iHeight);
                        SceneDraw(pApp->pScene);
    			    }
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                sprintf(buf, "Tic Tac Toe (SDL2) - X:%d Y:%d", event.motion.x, event.motion.y);
                SDL_SetWindowTitle(pApp->pWindow,buf);
                SceneTransferInput(pApp->pScene, event.motion.x, event.motion.y);
                SceneDraw(pApp->pScene);
                SceneCheckGameover(pApp->pScene);
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
