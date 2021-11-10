# CLI Payload Injector

**This payload injector is intended to be used in a WSL/MSYS environment.**

### Supports:
- [x] DLL injection
- [ ] Remote thread injection
- [ ] TBD

### Example use
```
./payload_injector_x86.exe explorer.exe test_payload.dll
```

### Building (MinGW)
```
make
```

### TODO
- [ ] SWITCH: -o --output -> specify log file location
- [ ] SWITCH: -r --remote -> specify remote thread routine
