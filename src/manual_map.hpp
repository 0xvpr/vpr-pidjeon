#ifndef   MANUAL_MAP_HEADER
#define   MANUAL_MAP_HEADER

#include  "definitions.hpp"

typedef FARPROC(__stdcall * pGetProcAddress)(HMODULE, LPCSTR);
typedef INT(__stdcall * dllmain)(HMODULE, DWORD, LPVOID);
typedef HMODULE(__stdcall * pLoadLibraryA)(LPCSTR);

typedef struct _LoaderData {
	LPVOID ImageBase;
	PIMAGE_NT_HEADERS NtHeaders;
	PIMAGE_BASE_RELOCATION BaseReloc;
	PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;
	pLoadLibraryA fnLoadLibraryA;
	pGetProcAddress fnGetProcAddress;
} LoaderData;

std::int32_t inject_manual_map(const types::parsed_args_t&);

#endif // MANUAL_MAP_HEADER
