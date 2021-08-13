#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct s_tiles t_tiles;

int TileInitGraph(SDL_Renderer* pRenderer, char* TileImgNPath, char* TileImgXPath, char* TileImgOPath);
void TileReleaseGraph();

t_tiles* TileNew(SDL_Rect* pDrawRect);
t_tiles* TileDel(t_tiles* pTile);
int TileDraw(t_tiles* pTile);
int TileSetValue(t_tiles* pTile, SDL_Rect* pValueTable);
int TileGetValue(t_tiles* pTile);