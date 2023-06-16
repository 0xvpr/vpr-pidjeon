<h1 align="center">
  <img src="res/Banner.png"></img>
</h1>
<br>
<p align="center">
  <img src="https://img.shields.io/badge/Windows--x86__64-supported-green">
  <img src="https://img.shields.io/badge/Windows--x86-supported-green">
  <img src="https://img.shields.io/badge/Linux--x86__64-unsupported-red">
  <img src="https://img.shields.io/badge/Linux--x86-unsupported-red">
  <a href="https://mit-license.org/">
    <img src="https://img.shields.io/github/license/0xvpr/vpr-pidjeon?style=flat-square">
  </a>
  <br>
  <h3 align="center">vpr-pidjeon</h3>
  <br>
</p>

**Pidjeon**, a mash of the words PID (process ID) and injection, is a command  
line application for Windows that uses various techniques to inject a specified  
payload into a target process.  

**Pidjeon was originally intended to be used in a WSL/MSYS environment**

## Demonstration
[ embed a demo ]

## Load Environment (preferred build method)
### Docker Container Instance
```bash
# using WSL2
git clone https://github.com/0xvpr/vpr-pidjeon
cd vpr-pidjeon
make docker-container && make-instance
```
### Docker Container Build
```
# using WSL2
git clone https://github.com/0xvpr/vpr-pidjeon
cd vpr-pidjeon
make docker-container && make-instance
```

## Usage
### Primary Features
- [x] DLL injection
- [x] Remote thread injection & execution
- [x] Shellcode injection

### Switches/Usage
- SWITCH: -i [ LoadLibraryA | LoadLibraryW | ManualMap ] -\> specify the injection method
- SWITCH: -d -> specify delay (milliseconds)
- SWITCH: -o --output \<path/to/output\> -\> specify log file location
- SWITCH: -r --remote -> specify remote thread routine
- SWITCH: -v -> specify verbosity level
- SWITCH: -c -> create a remote thread using second positional argument


### Examples
#### Powershell
Standard LoadLibraryA injection:
```powershell
./vpr-pidjeon.exe explorer.exe test_payload.dll
```
Silent Manual Map injection after 850 millisecond delay, output to out.txt using Stealth level 3:
```powershell
./vpr-pidjeon.exe explorer.exe test_payload.dll -s -i ManualMap -d 850 -o out.txt -S 3
```
TODO Create remote thread using bytes from shellcode.txt at code cave address 0xDEADBEEF using Stealth level 3:
```powershell
./vpr-pidjeon.exe explorer.exe shellcode.txt -r --cave 0xDEADBEEF -S 3
```
#### WSL2/MSYS
```bash
vpr-pidjeon explorer.exe test_payload.dll
```
Silent Manual Map injection after 850 millisecond delay, output to out.txt using Stealth level 3:
```bash
vpr-pidjeon explorer.exe test_payload.dll -s -i ManualMap -d 850 -o out.txt -S 3
```
TODO Create remote thread using bytes from shellcode.txt at code cave address 0xDEADBEEF using Stealth level 3:
```bash
vpr-pidjeon explorer.exe shellcode.txt -r --cave 0xDEADBEEF -S 3
```

## Building
#### Linux/WSL2 (MinGW)
```bash
make # makes release binaries by default
```

#### Windows (MinGW)
```powershell
make.exe # makes release binaries by default
```

### [TODO](TODO.md)
