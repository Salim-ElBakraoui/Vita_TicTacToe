#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define S_CELL_SIZE                 128
#define S_CELL_SPACING              12
#define S_BG_COLOR                  0x150022FF
#define S_FONTPATH                  "app0://Text/Square.ttf"

#define S_ROW_NUMBER                3
#define S_COL_NUMBER                3

#define S_TILE_NUMBER               (S_ROW_NUMBER*S_COL_NUMBER)

#define S_TILEPATH_N                "app0://Tiles/Sprites/TileN.png"
#define S_TILEPATH_O                "app0://Tiles/Sprites/TileO.png"
#define S_TILEPATH_X                "app0://Tiles/Sprites/TileX.png"

typedef struct s_scene t_scene;

t_scene* SceneNew(SDL_Window* pWindow, SDL_Renderer* pRenderer, int iWidth, int iHeight);
t_scene* SceneDel(t_scene* pScene);

void SceneDraw(t_scene* pScene);
void SceneTransferInput(t_scene* pScene, int iX, int iY);
void SceneCheckGameover(t_scene* pScene);
int SceneGetGameStatus(t_scene* pScene);
