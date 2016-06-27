#ifndef _DRAW_H
#define _DRAW_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* Render an obj file */
extern int Draw_RenderObj(const char *obj);

/* Swap the draw buffers so that everything just drawn can be shown */
extern void Draw_Swap();

/* Clear the buffer */
extern void Draw_Clear();

/* Move the camera somewhere and set its angle */
extern void Draw_SetCam(float x, float y, float z, float rotx, float roty, float rotz);

#endif
