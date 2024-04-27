#ifndef DEFINITIONS_HEADER
#define DEFINITIONS_HEADER

#include <cstdint>

namespace inject {

enum error : uint32_t {
    incorrect_parameters  = 100,
    process_not_running   = 101,
    dll_does_not_exist    = 102,
    injection_failed      = 103,
    method_not_found      = 104
};

enum operation : uint32_t {
    no_operation          = 0,
    load_library_a        = 1,
    load_library_w        = 2,
    manual_map            = 3,
    crt                   = 4,
}; // operation

} // namespace inject

namespace types {

enum machine_type : int32_t {
    machine_type_err      = -1,
    machine_unknown       =  0,
    machine_x86           =  1,
    machine_x64           =  2
};

typedef void* exec_mem_t;

typedef struct _resource {
    unsigned    process_id;
    const char* target_process;
    const char* relative_payload_path;
} resource;

typedef struct _injector {
    unsigned          status;
    inject::operation operation;
    unsigned          delay_ms;
    int               silent;
    int               stealth;
    int               verbosity;
    int               (* logger)(const _injector& injector, const char* event, uint32_t shiftwidth);
    char              output_file[260];
} injector;

} // namespace types

#endif // DEFINITIONS_HEADER
