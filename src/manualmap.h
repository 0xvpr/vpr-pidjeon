#ifndef MANUAL_MAP_HEADER
#define MANUAL_MAP_HEADER

#include "definitions.h"

#include <windows.h>

typedef FARPROC(__stdcall * pGetProcAddress)(HMODULE, LPCSTR);
typedef INT(__stdcall * dllmain)(HMODULE, DWORD, LPVOID);
typedef HMODULE(__stdcall * pLoadLibraryA)(LPCSTR);

typedef struct _LoaderData
{
	LPVOID ImageBase;
	PIMAGE_NT_HEADERS NtHeaders;
	PIMAGE_BASE_RELOCATION BaseReloc;
	PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;
	pLoadLibraryA fnLoadLibraryA;
	pGetProcAddress fnGetProcAddress;
} LoaderData;

unsigned inject_manual_map(const Resource * const restrict, const Injector * const restrict injector);

#endif /* MANUAL_MAP_HEADER */
