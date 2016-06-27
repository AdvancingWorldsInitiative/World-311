#include "vfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

V_File *V_OpenFile(const char *path, const char *name)
{
    int i, len;
    V_File *out;
    /* open the file */
    FILE *file = fopen(path, "rb");

    if(file == NULL)
        return NULL;

    /* move to the end to see how long it is */
    if(fseek(file, 0L, SEEK_END))
    {
        fclose(file);
        return NULL;
    }

    len = ftell(file);
    rewind(file);
    
    /* create a new vfile */
    out = V_NewFile(name);

    /* allocate enough data to hold the file */
    out->data = (char*)malloc(len * sizeof(char));

    /* read in the whole file */
    out->len = fread(out->data, sizeof(char), len, file);

    fclose(file);
    return out;
}

int V_SaveFile(V_File *vfile, const char *path)
{
    /* open a file */
    FILE *file = fopen(path, "wb");

    if(file == NULL)
        return -1;

    /* write the vfile's data to the file */
    if(fwrite(vfile->data, sizeof(char), vfile->len, file) != vfile->len)
        return -1;

    fclose(file);
    return 0;
}

V_File *V_NewFile(const char *name)
{
    /* allocate enough memory for the structure */
    V_File *out = (V_File*)malloc(sizeof(V_File));
    memset(out, 0, sizeof(V_File));

    /* copy the name to a newly allocated character array */
    out->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    memset(out->name, 0, (strlen(name) + 1) * sizeof(char));
    memcpy(out->name, name, strlen(name) * sizeof(char));

    return out;
}

V_Dir *V_NewDir(const char *name)
{
    /* allocate enough memory for the structure */
    V_Dir *out = (V_Dir*)malloc(sizeof(V_Dir));
    memset(out, 0, sizeof(V_Dir));

    /* copy the name to a newly allocated character array */
    out->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    memset(out->name, 0, (strlen(name) + 1) * sizeof(char));
    memcpy(out->name, name, strlen(name) * sizeof(char));

    return out;
}

V_System *V_NewSystem()
{
    /* allocate enough memory for the structure */
    V_System *out = (V_System*)malloc(sizeof(V_System));
    memset(out, 0, sizeof(V_System));

    /* create a root directory */
    out->root = V_NewDir("");

    return out;
}

V_File *V_GetFileByPath(V_System *sys, const char *path)
{
    int dirlen;
    char *dirpath;
    V_Dir *dir;

    /* find the last slash in the path so we can see how long the directory name is */
    for(dirlen = strlen(path); path[dirlen] != '/' && dirlen > 0; dirlen--);

    if(dirlen != 0)
    {
        /* copy just the directory name to a character array */
        dirpath = (char*)malloc((dirlen + 1) * sizeof(char));
        memset(dirpath, 0, (dirlen + 1) * sizeof(char));
        memcpy(dirpath, path, dirlen);

        /* look up the directory */
        dir = V_GetDirByPath(sys, dirpath);

        free(dirpath);
        return V_GetFileByDir(dir, path + dirlen + 1);
    }

    /* if there was no directory just find it in root */
    return V_GetFileByDir(sys->root, path);
}

V_Dir *V_GetDirByPath(V_System *sys, const char *path)
{
    int i;
    int pos = 0;
    char *temp;
    V_Dir *curdir = sys->root;

    for(i = 0; i < strlen(path) + 1; i++)
    {
        if((path[i] == '/' && i != pos) || path[i] == '\0')
        {
            /* get the name of the next directory */
            temp = (char*)malloc((i - pos + 1) * sizeof(char));
            memset(temp, 0, (i - pos + 1) * sizeof(char));
            memcpy(temp, path + pos, i - pos);

            /* look up that directory in the current directory */
            curdir = V_GetDirByDir(curdir, temp);
            free(temp);

            /* check if we're at the end yet */
            if(path[i] == '\0')
                break;

            pos = i + 1;
        }
    }

    /* return the last directory we found */
    return curdir;
}

V_File *V_GetFileByDir(V_Dir *dir, const char *name)
{
    int i;

    /* make sure the directory has files */
    if(dir->files == NULL)
        return NULL;

    /* search through it to see if the filename matches with anything */
    for(i = 0; i < dir->nfiles; i++)
        if(strcmp(dir->files[i]->name, name) == 0)
            return dir->files[i];

    /* if nothing matches */
    return NULL;
}

V_Dir *V_GetDirByDir(V_Dir *dir, const char *name)
{
    int i;

    /* make sure the directory has subdirectories */
    if(dir->dirs == NULL)
        return NULL;

    /* search through the subdirctories to see if anything matches */
    for(i = 0; i < dir->ndirs; i++)
        if(strcmp(dir->dirs[i]->name, name) == 0)
            return dir->dirs[i];

    /* if nothing matched */
    return NULL;
}

void V_AddFileToDir(V_Dir *parent, V_File *add)
{
    /* allocate a place to put files if there is none */
    /* or increase the array size if there already is one */
    if(parent->files == NULL || parent->nfiles == 0)
    {
        parent->files = (V_File**)malloc(sizeof(V_File*));
        parent->nfiles = 1;
    }
    else
        parent->files = (V_File**)realloc(parent->files, ++parent->nfiles * sizeof(V_File*));

    /* add the file to the newly allocated place in the array */
    parent->files[parent->nfiles - 1] = add;
}

void V_AddDirToDir(V_Dir *parent, V_Dir *add)
{
    /* allocate a place to put subdirectories if there is none */
    /* or increase the array size if there already is one */
    if(parent->dirs == NULL || parent->ndirs == 0)
    {
        parent->dirs = (V_Dir**)malloc(sizeof(V_Dir*));
        parent->ndirs = 1;
    }
    else
        parent->dirs = (V_Dir**)realloc(parent->dirs, ++parent->ndirs * sizeof(V_Dir*));

    /* add the directory to the newly allocated place in the array */
    parent->dirs[parent->ndirs - 1] = add;
}

void V_FreeVFile(V_File *file)
{
    /* free all the dynamically allocated arrays */
    free(file->name);
    free(file->data);
    /* free itself */
    free(file);
}

void V_FreeVDir(V_Dir *dir)
{
    int i;

    /* free the name character array */
    free(dir->name);

    /* free each of the files */
    for(i = 0; i < dir->nfiles; i++)
        V_FreeVFile(dir->files[i]);

    /* free the file array */
    free(dir->files);
    /* free itself */
    free(dir);
}

void V_FreeVSystem(V_System *sys)
{
    /* free the root directory */
    V_FreeVDir(sys->root);
    /* free itself */
    free(sys);
}
