#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#define INCORRECT_PARAMETERS  0xFFFFFFFF
#define PROCESS_NOT_RUNNING   0x00000001
#define DLL_DOES_NOT_EXIST    0x00000002
#define INJECTION_FAILED      0x00000004

#define NO_OPERATION          0x00000000
#define INJECT_LOAD_LIBRARY_A 0x00000001
#define INJECT_LOAD_LIBRARY_W 0x00000002
#define INJECT_MANUAL_MAP     0x00000004
#define INJECT_CRT            0x00000008
#define INJECT_ADD_DELAY      0x000000FF

#ifndef MAX_PATH
#define MAX_PATH              0x000000FF
#endif

typedef struct _Resource
{
    int process_id;
    char target_process[MAX_PATH];
    char dll_rel_path[MAX_PATH];
} Resource;

typedef struct _Injector
{
    int      status:8;
    int      operation;
    unsigned delay_ms;
    int      remote:1;
    int      silent:1;
    int      stealth:1;
    int      verbosity:2;
    char     output_file[MAX_PATH];
} Injector;

#endif /* _DEFINITIONS_H */
