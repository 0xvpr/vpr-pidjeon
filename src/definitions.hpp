#ifndef   DEFINITIONS_HEADER
#define   DEFINITIONS_HEADER

#include  <windows.h>

#include  <filesystem>
#include  <string>
#include  <chrono>

#include  <cstdint>

namespace inject {

enum status : std::int32_t {
    success                   = 0,
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

enum machine : std::int32_t {
    type_err                  = -1,
    unknown                   =  0,
    x86                       =  1,
    x64                       =  2
};

struct parsed_args_t {
    std::string               program_name;
    std::string               process_name;
    DWORD                     process_id;
    std::filesystem::path     payload_path;
    std::string               relative_payload_path;
    inject::operation         operation;
    std::chrono::milliseconds delay;
    std::int32_t              verbosity;
    std::int32_t              stealth;
    std::filesystem::path     log_path;
    std::int32_t              (* logger)(const parsed_args_t& args, const char* event, std::uint32_t shiftwidth);
};

typedef int (* logger_t)(const parsed_args_t&, const char*, std::uint32_t);
typedef void* exec_mem_t;

} // namespace types

#endif // DEFINITIONS_HEADER
