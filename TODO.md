# TODO
- [ ] Add some sort of Jira for project management
- [ ] Improve DLL injection statuses
- [ ] Improve DLL injection verification
- [ ] SWITCH: --cave -> (enhanced remote thread routine) code cave start address
- [ ] SWITCH: -s --silent -> disable logging
- [ ] SWITCH: -S --stealth -> specify stealth level

# TODONE
- [x] Use cmake build system inside of pidjeon

- [x] [ 21%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon.dir/src/injector.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c: In function 'InjectPayload':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:32:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   32 |             pInjector->status = inject_LoadLibraryA((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:37:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   37 |             pInjector->status = inject_LoadLibraryW((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:42:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   42 |             pInjector->status = inject_ManualMap((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:47:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   47 |             pInjector->status = InjectExecuteShellcode(data, pInjector);
      |                                 ^~~~~~~~~~~~~~~~~~~~~~

- [x] [ 42%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon.dir/src/manualmap.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c: In function 'LibraryLoader':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c:25:25: warning: conversion to 'int' from 'long unsigned int' may change the sign of the result [-Wsign-conversion]
   25 |             int count = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
      |                         ^

- [x] [  7%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon.dir/src/crt.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/crt.c: In function 'InjectExecuteShellcode':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/crt.c:67:27: warning: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to 'WINBOOL (*)(void *, void *, const void *, SIZE_T,  SIZE_T *)' {aka 'int (*)(void *, void *, const void *, long long unsigned int,  long long unsigned int *)'} [-Wcast-function-type]
   67 |     pWriteProcessMemory = (WriteProcessMemory_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory");
      |                           ^
- [x] [ 10%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon.dir/src/injector.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c: In function 'InjectPayload':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:32:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   32 |             pInjector->status = inject_LoadLibraryA((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:37:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   37 |             pInjector->status = inject_LoadLibraryW((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:42:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   42 |             pInjector->status = inject_ManualMap((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:47:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   47 |             pInjector->status = InjectExecuteShellcode(data, pInjector);
      |                                 ^~~~~~~~~~~~~~~~~~~~~~

- [x] [ 21%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon.dir/src/manualmap.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c: In function 'LibraryLoader':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c:25:25: warning: conversion from 'long long unsigned int' to 'int' may change value [-Wconversion]
   25 |             int count = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
      |                         ^
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c:33:29: warning: conversion from 'uintptr_t' {aka 'long long unsigned int'} to 'DWORD' {aka 'long unsigned int'} may change value [-Wconversion]
   33 |                     *ptr += delta;
      |                             ^~~~~

- [x] [ 32%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon.dir/src/util.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/util.c: In function 'GetArchitechture':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/util.c:83:24: warning: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to 'BOOL (*)(void *, USHORT *, USHORT *)' {aka 'int (*)(void *, short unsigned int *, short unsigned int *)'} [-Wcast-function-type]
   83 |     fIsWow64Process2 = (IsWow64Process2_t)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process2");
      |                        ^

- [x] [ 42%] Building C object src/pidjeon/CMakeFiles/vpr-pidjeon-wrapper.dir/src/util.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/util.c: In function 'GetArchitechture':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/util.c:83:24: warning: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to 'BOOL (*)(void *, USHORT *, USHORT *)' {aka 'int (*)(void *, short unsigned int *, short unsigned int *)'} [-Wcast-function-type]
   83 |     fIsWow64Process2 = (IsWow64Process2_t)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process2");
      |                        ^
- [x] [ 71%] Building C object test/CMakeFiles/run-tests.dir/__/src/pidjeon/src/crt.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/crt.c: In function 'InjectExecuteShellcode':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/crt.c:67:27: warning: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to 'WINBOOL (*)(void *, void *, const void *, SIZE_T,  SIZE_T *)' {aka 'int (*)(void *, void *, const void *, long long unsigned int,  long long unsigned int *)'} [-Wcast-function-type]
   67 |     pWriteProcessMemory = (WriteProcessMemory_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory");
      |                           ^
- [x] [ 75%] Building C object test/CMakeFiles/run-tests.dir/__/src/pidjeon/src/injector.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c: In function 'InjectPayload':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:32:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   32 |             pInjector->status = inject_LoadLibraryA((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:37:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   37 |             pInjector->status = inject_LoadLibraryW((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:42:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   42 |             pInjector->status = inject_ManualMap((DWORD)data->process_id, data->dll_rel_path);
      |                                 ^~~~~~~~~~~~~~~~
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/injector.c:47:33: warning: conversion from 'unsigned int' to 'unsigned char' may change value [-Wconversion]
   47 |             pInjector->status = InjectExecuteShellcode(data, pInjector);
      |                                 ^~~~~~~~~~~~~~~~~~~~~~

- [x] [ 85%] Building C object test/CMakeFiles/run-tests.dir/__/src/pidjeon/src/manualmap.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c: In function 'LibraryLoader':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c:25:25: warning: conversion from 'long long unsigned int' to 'int' may change value [-Wconversion]
   25 |             int count = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
      |                         ^
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/manualmap.c:33:29: warning: conversion from 'uintptr_t' {aka 'long long unsigned int'} to 'DWORD' {aka 'long unsigned int'} may change value [-Wconversion]
   33 |                     *ptr += delta;
      |                             ^~~~~

- [x] [ 96%] Building C object test/CMakeFiles/run-tests.dir/__/src/pidjeon/src/util.c.o
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/util.c: In function 'GetArchitechture':
/var/vpr-pidjeon-dev/vpr-pidjeon/src/pidjeon/src/util.c:83:24: warning: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to 'BOOL (*)(void *, USHORT *, USHORT *)' {aka 'int (*)(void *, short unsigned int *, short unsigned int *)'} [-Wcast-function-type]
   83 |     fIsWow64Process2 = (IsWow64Process2_t)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process2");
      |                        ^
