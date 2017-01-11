#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "draw.h"
#include "vfile.h"
#include "error.h"

GLuint LoadTexture(const char *path);

int main(int argc, char *argv[])
{
    /* This is mostly just for testing use right now */
    V_File *file;
    SDL_Event event;
    int quitting = 0;
    float depth = 0;
    Draw_ObjCache *cache = Draw_NewObjCache();
    GLuint tex = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Error_Print("Unable to initialize SDL.\n");
        return -1;
    }

    if(argc < 2)
        return -1;

    Draw_Init(920, 540, 0);

    if(argc > 2)
        tex = LoadTexture(argv[2]);

    file = V_OpenFile(argv[1], "foo");

    while(!quitting)
    {
        while(SDL_PollEvent(&event))
            if(event.type == SDL_QUIT)
                quitting = 1;
        Draw_Clear();
        glTranslatef(0, 0, depth);
        glRotatef(depth * 50, 0, 1, 0);
        Draw_RenderObj(file->data, file->len, tex, cache, cache);
        depth -= 0.1;
        Draw_Swap();
    }

    Draw_FreeObjCache(cache);
    SDL_Quit();
}

GLuint LoadTexture(const char *path)
{
  int i, j;
  GLuint texture;
  int check = 1;
  int bpp;
  SDL_Surface *surface;
  Uint8 *pixels;
  surface = SDL_LoadBMP(path);
  if(surface == NULL)
  {
    printf("Unable to open texture file: %s\n", SDL_GetError());
    return 0;
  }
  while(check < surface->w)
    check <<= 1;
  if(check != surface->w)
  {
    printf("Texture width is not a power of two.\n");
    return 0;
  }
  check = 1;
  while(check < surface->h)
    check <<= 1;
  if(check != surface->h)
  {
    printf("Texture height is not a power of two.\n");
    return 0;
  }
  bpp = surface->format->BytesPerPixel;
  pixels = (Uint8*)malloc(surface->w * surface->h * bpp);
  SDL_LockSurface(surface);
  for(i = 0; i < surface->w * surface->h * bpp; i++)
    pixels[i] = ((Uint8*)surface->pixels)[(i - i % (surface->w * bpp)) + ((surface->w - 1) * bpp - i % (surface->w * bpp)) + bpp - 1];
  SDL_UnlockSurface(surface);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0,
    GL_RGB, GL_UNSIGNED_BYTE, pixels);
  SDL_FreeSurface(surface);
  free(pixels);
  return texture;
}
