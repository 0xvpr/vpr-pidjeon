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

typedef struct _InjectData
{
    int process_id;
    char target_process[256];
    char dll_rel_path[256];
} InjectData;

typedef struct _Injector
{
    int      status;
    int      operation;
    unsigned delay_ms;
    int      remote:1;
    int      silent:1;
    int      stealth:1;
    int      verbosity:2;
    char     output_file[256];
} Injector;

#endif /* _DEFINITIONS_H */
