#ifndef _HANDLER_H
#define _HANDLER_H

#include <pthread.h>
#include "../connection/network.h"

/* lets you communicate with a running handler */
typedef struct
{
    int quit;
    pthread_t thread;
} Handler_Proc;

/* start up a handler */
extern int Handler_Start(Handler_Proc *proc, Net_Port port);

extern void Handler_End(Handler_Proc *proc);

extern void Handler_Kill(Handler_Proc *proc, int sig);

extern Handler_Proc *Handler_NewProc();

extern void Handler_FreeProc(Handler_Proc *proc);

#endif /* _HANDLER_H */
