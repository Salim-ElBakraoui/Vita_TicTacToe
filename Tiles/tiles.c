#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tiles.h"
#include <defines.h>

typedef enum e_tileState{
    TILE_N       = 0x1,
    TILE_X       = 0x2,
    TILE_O       = 0x4,
}t_tileState;

struct s_tiles{
    SDL_Rect   rDrawRect;
    t_tileState uTileState;    
};

struct {
    SDL_Renderer* pRenderer;
    SDL_Texture*  pTileImgN;
    SDL_Texture*  pTileImgO;
    SDL_Texture*  pTileImgX;
}graph;

int TileInitGraph(SDL_Renderer* pRenderer, char* TileImgNPath, char* TileImgXPath, char* TileImgOPath){
    graph.pRenderer = pRenderer;
    graph.pTileImgN = IMG_LoadTexture(graph.pRenderer, TileImgNPath);
    graph.pTileImgO = IMG_LoadTexture(graph.pRenderer, TileImgOPath);
    graph.pTileImgX = IMG_LoadTexture(graph.pRenderer, TileImgXPath);
}

void TileReleaseGraph(){
    SDL_DestroyTexture(graph.pTileImgX);
    SDL_DestroyTexture(graph.pTileImgO);
    SDL_DestroyTexture(graph.pTileImgN);
    graph.pRenderer = NULL;
}

t_tiles* TileNew(SDL_Rect* pDrawRect){
    t_tiles* pTile = (t_tiles*)malloc(sizeof(t_tiles));
    pTile->uTileState=0;
    mBitsSet(pTile->uTileState, TILE_N);
    pTile->rDrawRect=*pDrawRect;
    return pTile;
}

t_tiles* TileDel(t_tiles* pTile){
    free(pTile);
    pTile = NULL;
}

int TileDraw(t_tiles* pTile){
    if(mIsBitsSet(pTile->uTileState, TILE_O)){
        SDL_RenderCopy(graph.pRenderer, graph.pTileImgO, NULL, &pTile->rDrawRect);
    }
    else if(mIsBitsSet(pTile->uTileState, TILE_X)){
        SDL_RenderCopy(graph.pRenderer, graph.pTileImgX, NULL, &pTile->rDrawRect);
    }
    else if(mIsBitsSet(pTile->uTileState, TILE_N)){
        SDL_RenderCopy(graph.pRenderer, graph.pTileImgN, NULL, &pTile->rDrawRect);
    }
}
/*
1 for X
0 for O
*/
int TileSetValue(t_tiles* pTile, SDL_Rect *pValueTable){
    if( (!mIsBitsSet(pTile->uTileState, TILE_O)) && (!mIsBitsSet(pTile->uTileState, TILE_X)) && (pValueTable->x>pTile->rDrawRect.x) && (pValueTable->x<pTile->rDrawRect.x+pTile->rDrawRect.w) && (pValueTable->y>pTile->rDrawRect.y) && (pValueTable->y<pTile->rDrawRect.y+pTile->rDrawRect.h)){  
        if(pValueTable->w)
            mBitsSet(pTile->uTileState, TILE_X);
        else
            mBitsSet(pTile->uTileState, TILE_O);
        return 0;
    }
    else
        return 1;
}

/*
Returns 0 if the tile is neutral (not played yet)
Returns 1 if the tile is an X
Returns 2 if the tile is an O
*/
int TileGetValue(t_tiles* pTile){
    if(mIsBitsSet(pTile->uTileState, TILE_X))
        return 1;
    else if(mIsBitsSet(pTile->uTileState, TILE_O))
        return 2;
    else
        return 0;
}