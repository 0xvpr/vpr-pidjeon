# CLI Payload Injector

**This payload injector is intended to be used in a WSL/MSYS environment.**

### Features
- [x] DLL injection
- [x] Remote thread injection & execution

### Example uses
Standard LoadLibraryA injection:
```
./payload_injector_x86.exe explorer.exe test_payload.dll
```
Silent Manual Map injection after 850 millisecond delay, output to out.txt using Stealth level 3:
```
./payload_injector_x86.exe explorer.exe test_payload.dll -s -i ManualMap -d 850 -o out.txt -S 3
```
(TODO) Create remote thread using bytes from shellcode.txt at code cave address 0xDEADBEEF using Stealth level 3:
```
./payload_injector_x86.exe explorer.exe shellcode.txt -r --cave 0xDEADBEEF -S 3
```

### Building (MinGW)
```
make # makes release binaries
```

### TODO
- [x] SWITCH: -o --output \<path/to/output\> -\> specify log file location
- [x] SWITCH: -r --remote -> specify remote thread routine
  - [ ] SWITCH: --cave -> (enhanced remote thread routine) code cave address
- [ ] SWITCH: -s --silent -> disable logging
- [ ] SWITCH: -S --stealth -> specify stealth level
- [x] SWITCH: -v -> specify verbosity level
