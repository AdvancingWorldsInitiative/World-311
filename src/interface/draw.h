#ifndef _DRAW_H
#define _DRAW_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

typedef GLuint Draw_Texture;

typedef struct
{
    int used;
    float *vlist[3];
    int vsize[3];
} Draw_ObjCache;

extern int Draw_Init(int width, int height, int fullscreen);

/* Render an obj file */
extern int Draw_RenderObj(const char *obj, long len, Draw_Texture texture,
    Draw_ObjCache *cachein, Draw_ObjCache *cacheout);

extern Draw_ObjCache *Draw_NewObjCache();
extern void Draw_FreeObjCache(Draw_ObjCache *cache);

/* Swap the draw buffers so that everything just drawn can be shown */
extern void Draw_Swap();

/* Clear the buffer */
extern void Draw_Clear();

/* Move the camera somewhere and set its angle */
extern void Draw_SetCam(float x, float y, float z,
    float rotx, float roty, float rotz);

#endif
