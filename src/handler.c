#include "handler.h"
#include <stdlib.h>
#include "network.h"

/* holds the current state of the packet handler */
typedef struct
{
    int *quit;
    Net_Sock serv;
    Net_Port port;
} Handler_State;

static void *Handler_Entry(void *data);

static int Handler_Init(Handler_State *state);

static int Handler_Loop(Handler_State *state);

static void Handler_Cleanup(Handler_State *state);

int Handler_Start(Handler_Proc *proc, Net_Port port)
{
    Handler_State *state = (Handler_State*)malloc(sizeof(Handler_State));

    if(proc == NULL)
        proc = Handler_NewProc();

    if(proc == NULL)
        return -1;

    proc->quit = 0;
    state->quit = &proc->quit;
    state->port = port;

    if(pthread_create(&proc->thread, NULL, Handler_Entry, state))
        return -1;

    return 0;
}

void Handler_End(Handler_Proc *proc)
{
    proc->quit = 1;
}

void Handler_Kill(Handler_Proc *proc, int sig)
{
    if(pthread_kill(proc->thread, sig))
        Handler_End(proc);
}

Handler_Proc *Handler_NewProc()
{
    return (Handler_Proc*)malloc(sizeof(Handler_Proc*));
}

void Handler_FreeProc(Handler_Proc *proc)
{
    free(proc);
}

void *Handler_Entry(void *data)
{
    Handler_State *state = (Handler_State*)data;

    if(!Handler_Init(state))
        while(!Handler_Loop(state));

    Handler_Cleanup(state);
}

int Handler_Init(Handler_State *state)
{
    if(state == NULL)
        return -1;

    state->serv = Net_NewSock(NET_UDP);
    Net_StartServer(state->serv, state->port, NET_UDP);

    return 0;
}

int Handler_Loop(Handler_State *state)
{
    if(*state->quit)
        return 1;

    return 0;
}

void Handler_Cleanup(Handler_State *state)
{
    if(state == NULL)
        return;

    free(state);
}
