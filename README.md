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

### Contributing
We are using the CUnit Testing Suite in order to make sure  
that every commit doesn't suck ass. If you plan on contributing,  
we recommend that you install CUnit through the MSYS package  
manager and compile the program through MinGW.
```
make tests
```

### TODO
- [x] SWITCH: -o --output -> specify log file location
- [ ] SWITCH: -r --remote -> specify remote thread routine
- [ ] SWITCH: -s --silent -> disable logging
- [ ] SWITCH: -S --stealth -> specify stealth level
- [ ] SWITCH: -v -> specify verbosity level
