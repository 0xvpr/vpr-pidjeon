#ifndef DEFINITIONS_HEADER
#define DEFINITIONS_HEADER

#include <cstdint>

namespace inject {

enum error : uint32_t {
    incorrect_parameters  = 0x01,
    process_not_running   = 0x02,
    dll_does_not_exist    = 0x03,
    injection_failed      = 0x04,
    method_not_found      = 0x05
};

enum operation : uint32_t {
    no_operation          = 0x01,
    load_library_a        = 0x02,
    load_library_w        = 0x03,
    manual_map            = 0x04,
    crt                   = 0x05,
    add_delay             = 0x06
}; // operation

} // namespace inject

enum machine_type : int32_t {
    machine_type_err =  -1,
    machine_unknown =    0,
    machine_x86 =        1,
    machine_x64 =        2
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

#endif // DEFINITIONS_HEADER
