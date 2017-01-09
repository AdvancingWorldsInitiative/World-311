#include "draw.h"
#include "../utilities/error.h"

int Draw_Init(int width, int height, int fullscreen)
{
    float ratio;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    ratio = (float)width / (float)height;

    if(SDL_SetVideoMode(width, height, 24,
        (fullscreen) ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL) == 0)
    {
        Error_Print("Unable to initialize video: %s\n", SDL_GetError());
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 1.0, 1024.0);
}

int Draw_RenderObj(const char *obj, long len, Draw_Texture texture,
    Draw_ObjCache *cachein, Draw_ObjCache *cacheout)
{
    long curpos = 0;

    float *vlist[3];
    int vsize[3];
    int vindex = 0;

    char *fpos;
    int fvert;
    int ftvert;
    int fnvert;

    char *line;
    int linelen;

    glMatrixMode(GL_MODELVIEW);

    if(texture != 0)
        glBindTexture(GL_TEXTURE_2D, texture);

    if(cachein != NULL && cachein->used)
    {
        memcpy(vlist, cachein->vlist, sizeof(float*) * 3);
        memcpy(vsize, cachein->vsize, sizeof(int) * 3);
    }
    else
    {
        memset(vlist, 0, sizeof(float*) * 3);
        memset(vsize, 0, sizeof(int) * 3);
    }

    while(curpos < len)
    {
        while(obj[curpos] == ' ' || obj[curpos] == '\t' && curpos < len)
            curpos++;
        if(curpos >= len)
            break;

        for(linelen = 0;
            obj[curpos + linelen] != '\n' && curpos + linelen < len;
            linelen++);
        line = (char*)malloc((linelen + 1) * sizeof(char));
        strncpy(line, obj + curpos, linelen);
        line[linelen] = '\0';

        switch(obj[curpos])
        {
            case '#':
            case '\n':
                break;

            case 'v':
                if(cachein != NULL && cachein->used)
                    break;

                switch(obj[curpos + 1])
                {
                    case ' ':
                        vindex = 0;
                        break;
                    case 't':
                        vindex = 1;
                        break;
                    case 'n':
                        vindex = 2;
                        break;
                    default:
                        vindex = -1;
                        break;
                }

                if(vindex == -1)
                    break;

                if(vlist[vindex] == NULL)
                    vlist[vindex] = (float*)malloc(((vindex == 1) ? 2 : 3)
                        * sizeof(float));
                else
                    vlist[vindex] = (float*)realloc(vlist[vindex],
                        (vsize[vindex] + ((vindex == 1) ? 2 : 3)) * sizeof(float));

                memset(vlist[vindex] + vsize[vindex], 0,
                    ((vindex == 1) ? 2 : 3) * sizeof(float));

                if(vindex == 1)
                    sscanf(line + 3 * sizeof(char), "%f %f",
                        &vlist[1][vsize[1]], &vlist[1][vsize[1] + 1]);
                else
                    sscanf(line + ((vindex) ? 3 : 2) * sizeof(char),
                        "%f %f %f",
                        &vlist[vindex][vsize[vindex]],
                        &vlist[vindex][vsize[vindex] + 1],
                        &vlist[vindex][vsize[vindex] + 2]);

                if(vindex == 1)
                    vsize[1] += 2;
                else
                    vsize[vindex] += 3;

                break;

            case 'f':
                if(obj[curpos + 1] != ' ')
                    break;

                fpos = line + 2 * sizeof(char);

                fvert = -1;
                ftvert = -1;
                fnvert = -1;

                glBegin(GL_POLYGON);

                while(fpos - line < linelen * sizeof(char))
                {
                    sscanf(fpos, "%d", &fvert);

                    if((strchr(fpos, '/') < strchr(fpos, ' ') ||
                        strchr(fpos, ' ') == NULL) &&
                        strchr(fpos, '/') != NULL)
                    {
                        fpos = strchr(fpos, '/');
                        fpos += sizeof(char);

                        sscanf(fpos, "%d", &ftvert);

                        if((strchr(fpos, '/') < strchr(fpos, ' ') ||
                            strchr(fpos, ' ') == NULL) &&
                            strchr(fpos, '/') != NULL)
                        {
                            fpos = strchr(fpos, '/');
                            fpos += sizeof(char);

                            sscanf(fpos, "%d", &fnvert);

                            fpos = strchr(fpos, ' ');
                        }
                        else
                            fpos = strchr(fpos, ' ');
                    }
                    else
                        fpos = strchr(fpos, ' ');

                    if(fpos == NULL)
                        fpos = line + linelen * sizeof(char);
                    else
                        fpos += sizeof(char);

                    if(fnvert >= 0 && fnvert <= vsize[2] / 3)
                        glNormal3f(vlist[2][(fnvert - 1) * 3],
                            vlist[2][(fnvert - 1) * 3 + 1],
                            vlist[2][(fnvert - 1) * 3 + 2]);

                    if(ftvert >= 0 && ftvert <= vsize[1] / 2)
                        glTexCoord2f(1 - vlist[1][(ftvert - 1) * 2],
                            1 - vlist[1][(ftvert - 1) * 2 + 1]);

                    if(fvert >= 0 && fvert <= vsize[0] / 3)
                        glVertex3f(vlist[0][(fvert - 1) * 3],
                            vlist[0][(fvert - 1) * 3 + 1],
                            vlist[0][(fvert - 1) * 3 + 2]);
                }

                glEnd();

                break;
        }

        free(line);
        curpos += linelen + 1;
    }

    if(cacheout != NULL)
    {
        if(!cacheout->used ||
            (memcmp(cacheout->vlist, vlist, sizeof(float*) * 3) != 0 &&
            memcmp(cacheout->vsize, vsize, sizeof(int) * 3) != 0))
        {
            cacheout->used = 1;

            if(cacheout->vlist[0] != NULL)
                free(cacheout->vlist[0]);
            if(cacheout->vlist[1] != NULL)
                free(cacheout->vlist[1]);
            if(cacheout->vlist[2] != NULL)
                free(cacheout->vlist[2]);

            memcpy(cacheout->vlist, vlist, sizeof(float*) * 3);
            memcpy(cacheout->vsize, vsize, sizeof(int) * 3);
        }
    }
    else
    {
        free(vlist[0]);
        free(vlist[1]);
        free(vlist[2]);
    }

    return 0;
}

Draw_ObjCache *Draw_NewObjCache()
{
    Draw_ObjCache *out;

    out = (Draw_ObjCache*)malloc(sizeof(Draw_ObjCache));

    if(out == NULL)
        return out;

    memset(out, 0, sizeof(Draw_ObjCache));

    return out;
}

void Draw_FreeObjCache(Draw_ObjCache *cache)
{
    if(cache == NULL)
        return;

    free(cache->vlist[0]);
    free(cache->vlist[1]);
    free(cache->vlist[2]);

    free(cache);
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
