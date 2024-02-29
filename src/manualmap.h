#ifndef MANUAL_MAP_HEADER
#define MANUAL_MAP_HEADER

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

DWORD __stdcall LibraryLoader(LPVOID memory);

DWORD __stdcall stub(void);

unsigned inject_ManualMap(DWORD process_id, const char* restrict dll_path);

#endif /* MANUAL_MAP_HEADER */
