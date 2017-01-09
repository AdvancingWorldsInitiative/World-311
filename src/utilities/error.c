#include "error.h"
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static int Error_options;
static pthread_mutex_t Error_options_m;

int Error_Init()
{
    if(pthread_mutex_init(&Error_options_m, NULL))
        return -1;

    Error_SetOptions(ERROR_OPT_STDOUT);

    return 0;
}

void Error_Print(const char *fmt, ...)
{
    FILE *file;
    va_list ap;

    pthread_mutex_lock(&Error_options_m);
    va_start(ap, fmt);

    switch(Error_options)
    {
        case ERROR_OPT_STDOUT:
          vprintf(fmt, ap);
          break;

        case ERROR_OPT_STDERR:
          vfprintf(stderr, fmt, ap);
          break;
        case ERROR_OPT_FILE:
          file = fopen(ERROR_FILENAME, "a");
          vfprintf(file, fmt, ap);
          fclose(file);
          break;
    }

    va_end(ap);
    pthread_mutex_unlock(&Error_options_m);
}

void Error_SetOptions(int options)
{
    pthread_mutex_lock(&Error_options_m);
    Error_options = options;
    pthread_mutex_unlock(&Error_options_m);
}

int Error_GetOptions()
{
    return Error_options;
}
