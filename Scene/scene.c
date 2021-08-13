#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <container.h>
#include <defines.h>
#include <text.h>
#include <tiles.h>

#include "scene.h"

typedef enum e_gameStatus{
    S_GAME_PLAYING = 	0b0000001,
    S_GAME_OVER_P1 =    0b0000010,
    S_GAME_OVER_P2 =    0b0000100,
    S_GAME_OVER_DRAW =  0b0001000,
}t_gameStatus;

struct s_scene{
    SDL_Window*             pWindow;
    SDL_Renderer*           pRenderer;
    TTF_Font*               pFont;
    int                     iWidth,
                            iHeight,
                            iCheckDraw;
    t_container*            pTexts;
    t_container*            pTiles;
    t_gameStatus            uGameStatus;
    int                     iTableTileValues[S_TILE_NUMBER];
    bool                    bGameState;

};
t_scene* SceneNew(SDL_Window* pWindow, SDL_Renderer* pRenderer, int iWidth, int iHeight){
    t_scene* pScene = (t_scene*)malloc(sizeof(t_scene));

    pScene->pWindow = pWindow;
    pScene->pRenderer = pRenderer;
    pScene->iWidth = iWidth;
    pScene->iHeight = iHeight;
    pScene->iCheckDraw=0;

    TTF_Init();
    pScene->pFont = TTF_OpenFont(S_FONTPATH, 16);
    TextInitGraph(pScene->pFont, (SDL_Color){255,255,255,255}, pScene->pRenderer);
    TileInitGraph(pScene->pRenderer, S_TILEPATH_N, S_TILEPATH_X, S_TILEPATH_O);
    
    pScene->pTexts=ContainerNew((t_ptfV)TextDel);
    pScene->pTiles=ContainerNew((t_ptfV)TileDel);

    int iWCenterScene = pScene->iWidth/2 - (S_CELL_SIZE*S_COL_NUMBER + S_CELL_SPACING*S_COL_NUMBER)/2;
    int iHCenterScene = pScene->iHeight/2 - (S_CELL_SIZE*S_ROW_NUMBER + S_CELL_SPACING*S_ROW_NUMBER)/2;
    for(int k=0; k<S_ROW_NUMBER; k++){
        for(int m=0; m<S_COL_NUMBER; m++){
            ContainerPushback(pScene->pTiles, TileNew(                
                &(SDL_Rect){iWCenterScene+S_CELL_SIZE*m+S_CELL_SPACING*m,
                iHCenterScene+S_CELL_SIZE*k+S_CELL_SPACING*k,
                S_CELL_SIZE,
                S_CELL_SIZE})
            );
        }
    }

    for(int k=0;k<S_TILE_NUMBER;k++)
        pScene->iTableTileValues[k]=0;

    ContainerPushback(pScene->pTexts, TextNew(&(SDL_Point){24,24},"Player1 Turn"));
    ContainerPushback(pScene->pTexts, TextNew(&(SDL_Point){24,24}, "Player1 Wins"));
    ContainerPushback(pScene->pTexts, TextNew(&(SDL_Point){24,24}, "Player2 Wins"));
    ContainerPushback(pScene->pTexts, TextNew(&(SDL_Point){24,24}, "It's a draw"));
    ContainerPushback(pScene->pTexts, TextNew(&(SDL_Point){24,24},"Player2 Turn"));
    TextCenterX((t_text*)ContainerGetfront(pScene->pTexts), pScene->iWidth);
    TextCenterX((t_text*)ContainerGetAt(pScene->pTexts, 1), pScene->iWidth);
    TextCenterX((t_text*)ContainerGetAt(pScene->pTexts, 2), pScene->iWidth);
    TextCenterX((t_text*)ContainerGetAt(pScene->pTexts, 3), pScene->iWidth);
    TextCenterX((t_text*)ContainerGetback(pScene->pTexts), pScene->iWidth);

    pScene->uGameStatus = 0;
    mBitsSet(pScene->uGameStatus, S_GAME_PLAYING);
    pScene->bGameState = false;
    return pScene;
}

t_scene* SceneDel(t_scene* pScene){
    ContainerDel(pScene->pTexts);
    ContainerDel(pScene->pTiles);
    TileReleaseGraph();
    TextReleaseGraph();
    TTF_CloseFont(pScene->pFont);
    free(pScene);
    pScene=NULL;
}

void SceneDraw(t_scene* pScene){
    SDL_Color cBgCol;
    mToColor(cBgCol, S_BG_COLOR);
    SDL_SetRenderDrawColor(pScene->pRenderer, cBgCol.r, cBgCol.g, cBgCol.b, cBgCol.a);
    SDL_RenderClear(pScene->pRenderer);

    for(int k=0; k<S_TILE_NUMBER; k++)
        TileDraw((t_tiles*)ContainerGetAt(pScene->pTiles, k));
    
    if(mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_P1))
        TextDraw(ContainerGetAt(pScene->pTexts, 1));
    else if(mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_P2))
        TextDraw(ContainerGetAt(pScene->pTexts, 2));
    else if(mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_DRAW))
        TextDraw(ContainerGetAt(pScene->pTexts, 3));
    else if(pScene->bGameState)
        TextDraw(ContainerGetback(pScene->pTexts));
    else if(!pScene->bGameState)
        TextDraw(ContainerGetfront(pScene->pTexts));
    SDL_RenderPresent(pScene->pRenderer);
}

void SceneTransferInput(t_scene* pScene, int iX, int iY){
    if(mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_P1) || mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_P2) || mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_DRAW)){
        printf("It's game over, you don't have the right to play anymore\n");
        return;
    }

    if(!pScene->bGameState){ 
        for(int k=0; k<S_TILE_NUMBER; k++){
            if(!TileSetValue((t_tiles*)ContainerGetAt(pScene->pTiles, k), &(SDL_Rect){iX, iY, 0, 0})){
                pScene->bGameState = !pScene->bGameState;
                return;
            }
        }
    }   
      
    else{
        for(int k=0; k<S_TILE_NUMBER; k++){
            if(!TileSetValue((t_tiles*)ContainerGetAt(pScene->pTiles, k), &(SDL_Rect){iX, iY, 1, 0})){
                pScene->bGameState = !pScene->bGameState;
                return;
            }
        }
    }   

}

void SceneCheckGameover(t_scene* pScene){
    
    int iWinnerCheck=0, iCheckDraw=0, iEmptyTiles=0;
    for(int k=0;k<S_TILE_NUMBER;k++){
        pScene->iTableTileValues[k]=TileGetValue((t_tiles*)ContainerGetAt(pScene->pTiles, k));
    }
    
    if(
      (pScene->iTableTileValues[2] && (iWinnerCheck=pScene->iTableTileValues[2])  && pScene->iTableTileValues[2]==pScene->iTableTileValues[5] && pScene->iTableTileValues[5]==pScene->iTableTileValues[8])
    ||(pScene->iTableTileValues[0] && (iWinnerCheck=pScene->iTableTileValues[0])  && pScene->iTableTileValues[0]==pScene->iTableTileValues[3] && pScene->iTableTileValues[3]==pScene->iTableTileValues[6])
    ||(pScene->iTableTileValues[1] && (iWinnerCheck=pScene->iTableTileValues[1])  && pScene->iTableTileValues[1]==pScene->iTableTileValues[4] && pScene->iTableTileValues[4]==pScene->iTableTileValues[7])
    ||(pScene->iTableTileValues[0] && (iWinnerCheck=pScene->iTableTileValues[0])  && pScene->iTableTileValues[0]==pScene->iTableTileValues[1] && pScene->iTableTileValues[1]==pScene->iTableTileValues[2])
    ||(pScene->iTableTileValues[3] && (iWinnerCheck=pScene->iTableTileValues[3])  && pScene->iTableTileValues[3]==pScene->iTableTileValues[4] && pScene->iTableTileValues[4]==pScene->iTableTileValues[5])
    ||(pScene->iTableTileValues[6] && (iWinnerCheck=pScene->iTableTileValues[6])  && pScene->iTableTileValues[6]==pScene->iTableTileValues[7] && pScene->iTableTileValues[7]==pScene->iTableTileValues[8])
    ||(pScene->iTableTileValues[0] && (iWinnerCheck=pScene->iTableTileValues[0])  && pScene->iTableTileValues[0]==pScene->iTableTileValues[4] && pScene->iTableTileValues[4]==pScene->iTableTileValues[8])
    ||(pScene->iTableTileValues[2] && (iWinnerCheck=pScene->iTableTileValues[2])  && pScene->iTableTileValues[2]==pScene->iTableTileValues[4] && pScene->iTableTileValues[4]==pScene->iTableTileValues[6])
    ){
        if(iWinnerCheck==2)mBitsSet(pScene->uGameStatus, S_GAME_OVER_P1);
        if(iWinnerCheck==1)mBitsSet(pScene->uGameStatus, S_GAME_OVER_P2);

        SceneDraw(pScene);
    }
    else{
        for(int k=0;k<S_TILE_NUMBER;k++){
            iCheckDraw+=pScene->iTableTileValues[k];
            if(!pScene->iTableTileValues[k])
                iEmptyTiles++;
        }
        if(!iEmptyTiles)
            mBitsSet(pScene->uGameStatus, S_GAME_OVER_DRAW); 
            SceneDraw(pScene);     
    }
}


int SceneGetGameStatus(t_scene* pScene){
    
    if(mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_P1) || mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_P2) || mIsBitsSet(pScene->uGameStatus, S_GAME_OVER_DRAW))
        return 1;
    else
        return 0;
}
