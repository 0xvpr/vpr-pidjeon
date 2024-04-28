#ifndef DEFINITIONS_HEADER
#define DEFINITIONS_HEADER

#include <cstdint>
#include <string>

namespace inject {

enum error : std::uint32_t {
    incorrect_parameters      = 100,
    process_not_running       = 101,
    dll_does_not_exist        = 102,
    injection_failed          = 103,
    method_not_found          = 104
};

enum operation : std::uint32_t {
    no_operation              = 0,
    load_library_a            = 1,
    load_library_w            = 2,
    manual_map                = 3,
    crt                       = 4,
};

} // namespace inject

namespace types {

enum machine_type : std::int32_t {
    machine_type_err          = -1,
    machine_unknown           =  0,
    machine_x86               =  1,
    machine_x64               =  2
};

typedef struct _resource {
    unsigned                  process_id;
    std::string_view          target_process;
    std::string_view          relative_payload_path;
} resource;

typedef struct _injector {
    std::uint32_t             status;
    inject::operation         operation;
    std::chrono::milliseconds delay_ms;
    bool                      silent;
    std::int32_t              stealth;
    std::int32_t              verbosity;
    std::int32_t              (* logger)(const _injector& injector, const char* event, std::uint32_t shiftwidth);
    std::string               output_file;
} injector;

typedef void* exec_mem_t;

} // namespace types

#endif // DEFINITIONS_HEADER
