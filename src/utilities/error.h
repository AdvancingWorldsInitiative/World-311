#ifndef _ERROR_H
#define _ERROR_H

#define ERROR_FILENAME "error.txt"

#define ERROR_OPT_STDOUT 0
#define ERROR_OPT_STDERR 1
#define ERROR_OPT_FILE 2

extern int Error_Init();

extern void Error_Print(const char *fmt, ...);

extern void Error_SetOptions(int options);
extern int Error_GetOptions();

#endif /* _ERROR_H */
