#ifndef   UTIL_HEADER
#define   UTIL_HEADER

#include  "definitions.h"

void usage(const char* error_message, struct parsed_args_t* parsed_args);

size_t get_process_ids_by_process_name(const char* const process_name, DWORD* processes, size_t processes_size);

int get_process_name_by_process_id(DWORD process_id, char* buffer, size_t buffer_size);

enum machine_t get_machine_type(const HANDLE process_handle);

int file_exists(const char* filename);

void init_parsed_args(struct parsed_args_t* parsed_args, const char* argv0);

enum errcode_t validate_arguments(struct parsed_args_t* parsed_args);

#endif /* UTIL_HEADER */
