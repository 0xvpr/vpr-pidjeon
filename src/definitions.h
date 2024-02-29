#ifndef DEFINITIONS_HEADER
#define DEFINITIONS_HEADER

#define INCORRECT_PARAMETERS  0xFFFFFFFF
#define PROCESS_NOT_RUNNING   0x00000001
#define DLL_DOES_NOT_EXIST    0x00000002
#define INJECTION_FAILED      0x00000004
#define METHOD_NOT_FOUND      0x00000008

#define NO_OPERATION          0x00000000
#define INJECT_LOAD_LIBRARY_A 0x00000001
#define INJECT_LOAD_LIBRARY_W 0x00000002
#define INJECT_MANUAL_MAP     0x00000004
#define INJECT_CRT            0x00000008
#define INJECT_ADD_DELAY      0x000000FF

#ifndef MAX_PATH
#define MAX_PATH              0x00000104
#endif /* MAX_PATH */

#include <stdint.h>

typedef struct _Resource
{
    unsigned    process_id;
    char        target_process[MAX_PATH];
    char        dll_rel_path[MAX_PATH];
} Resource;

typedef struct _Injector
{
    unsigned    status;
    int         operation;
    unsigned    delay_ms;
    int         remote;
    int         silent;
    int         stealth;
    int         verbosity;
    int (* logger)(const struct _Injector* injector, const char* restrict event, uint32_t shiftwidth);
    char        output_file[MAX_PATH];
} Injector;


#endif /* DEFINITIONS_HEADER */
