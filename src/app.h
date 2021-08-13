#include <stdlib.h>
#include <stdio.h>

typedef struct s_app t_app;

t_app* AppNew(char* sTitle, int iWidth, int iHeight);
t_app* AppDel(t_app* pApp);
int AppRun(t_app* pApp);