#include "draw.h"
#include "error.h"

int Draw_RenderObj(const char *obj)
{
}

void Draw_Swap()
{
    SDL_GL_SwapBuffers();
    glLoadIdentity();
}

void Draw_Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw_SetCam(float x, float y, float z, float rotx, float roty, float rotz)
{
    glTranslatef(0 - x, 0 - y, 0 - z);
    glRotatef(rotx, 1, 0, 0);
    glRotatef(roty, 0, 1, 0);
    glRotatef(rotz, 0, 0, 1);
}
