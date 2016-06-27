#ifndef _VFILE_H
#define _VFILE_H

/* stores a virtual file in memory */
typedef struct
{
    char *name;
    char *data;
    long len;
} V_File;

/* need this for subdirectories to work right */
typedef struct vdir* V_Dirptr;

/* stores a directory containing pointers to files and subdirectories */
typedef struct vdir
{
    char *name;
    V_File **files;
    int nfiles;
    V_Dirptr *dirs;
    int ndirs;
} V_Dir;

/* stores a virtual file system in memory */
typedef struct
{
    V_Dir *root;
} V_System;

/* Open a normal file and place the data in a virtual file */
extern V_File *V_OpenFile(const char *path, const char *name);

/* Write a virtual file to a normal file */
extern int V_SaveFile(V_File *vfile, const char *path);

/* Make a new empty file */
extern V_File *V_NewFile(const char *name);

/* Make a new empty directory */
extern V_Dir *V_NewDir(const char *name);

/* Make a new virtual filesystem */
extern V_System *V_NewSystem();

/* Get a file by its path in a filesystem */
extern V_File *V_GetFileByPath(V_System *sys, const char *path);

/* Get a directory using its path in a filesystem */
extern V_Dir *V_GetDirByPath(V_System *sys, const char *path);

/* Get a file from a directory */
extern V_File *V_GetFileByDir(V_Dir *dir, const char *name);

/* Get a directory from another directory */
extern V_Dir *V_GetDirByDir(V_Dir *dir, const char *name);

extern void V_AddFileToDir(V_Dir *parent, V_File *add);

/* Add a subdirectory to a directory */
extern void V_AddDirToDir(V_Dir *parent, V_Dir *add);

/* Free a virtual file */
extern void V_FreeVFile(V_File *file);

/* Free a virtual directory */
extern void V_FreeVDir(V_Dir *dir);

/* Free a virtual filesystem */
extern void V_FreeVSystem(V_System *sys);

#endif
