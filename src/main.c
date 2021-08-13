#include <stdlib.h>
#include <stdio.h>

#include "app.h"

int _newlib_heap_size_user = 192 * 1024 * 1024;

int main(int argc, char*argv[]){
    t_app* pApp = AppNew("Tic Tac Toe - SDL2", 960, 544);
    AppRun(pApp);
    pApp=AppDel(pApp);
    return EXIT_SUCCESS;
}
