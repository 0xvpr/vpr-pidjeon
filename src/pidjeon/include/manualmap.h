#ifndef MANUAL_MAP_HEADER
#define MANUAL_MAP_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif /* VC_EXTRA_LEAN */

typedef FARPROC(__stdcall * pGetProcAddress)(HMODULE, LPCSTR);
typedef INT(__stdcall * dllmain)(HMODULE, DWORD, LPVOID);
typedef HMODULE(__stdcall * pLoadLibraryA)(LPCSTR);

typedef struct _loaderdata
{
	LPVOID ImageBase;
	PIMAGE_NT_HEADERS NtHeaders;
	PIMAGE_BASE_RELOCATION BaseReloc;
	PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;
	pLoadLibraryA fnLoadLibraryA;
	pGetProcAddress fnGetProcAddress;
} loaderdata;

DWORD __stdcall LibraryLoader(LPVOID memory);

DWORD __stdcall stub(void);

unsigned inject_ManualMap(DWORD process_id, const char* restrict dll_path);

#endif /* MANUAL_MAP_HEADER */
