#ifndef   UTIL_HEADER
#define   UTIL_HEADER

#include  "definitions.hpp"

#include  <vector>
#include  <string>

namespace util {

std::vector<DWORD> get_process_ids_by_process_name(const char* process_name);
std::string get_process_name_by_process_id(DWORD process_id);
types::machine get_machine_type(const HANDLE process_handle);

} // namespace util

#endif // UTIL_HEADER
