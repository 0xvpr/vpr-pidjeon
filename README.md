<h1 align="center">vpr-pidjeon</h1>
<p align="center">
  <img src="https://img.shields.io/badge/Windows--x86__64-supported-44CC11?style=flat-square"/>
  <img src="https://img.shields.io/badge/Windows--x86-supported-44CC11?style=flat-square"/>
  <img src="https://img.shields.io/badge/Linux-unsupported-red?style=flat-square"/>
  <a href="https://mit-license.org/"/>
    <img src="https://img.shields.io/github/license/0xvpr/vpr-pidjeon?style=flat-square&color=44CC11"/>
  </a>
  <img src="https://img.shields.io/github/actions/workflow/status/0xvpr/vpr-pidjeon/docker_build.yml?style=flat-square"/>
  <img src="https://img.shields.io/github/actions/workflow/status/0xvpr/vpr-pidjeon/windows_unit_tests.yml?label=tests"/>
</p>
<br>

**pidjeon**, a mash-up of the words PID (process ID) and injection, is a command  
line application for Windows that uses various techniques to inject a specified  
payload into a target process.  

## Demonstration
[ TODO ]

## Usage
```bash
Error message: Not enough arguments provided.

Usage:
  vpr-pidjeon-x64.exe [<optional_arguments>] <target_process> <path/to/payload>

Positional arguments (no specific order required):
  <target_process>  Specify either the executable name or process id
  <path/to/payload> Specify either the full or relative (POSIX compliant)
                    path to a payload.

Optional arguments:
  -i <method>       Specify injection one of the following injection methods:
                    LoadLibraryA|lla (default method),
                    LoadLibraryW|llw,
                    ManualMap|mm,
                    CreateRemoteThread|crt.
  -d <milliseconds> Add delay to the injection.
  -s,               Set stealth level 0-2.
  -ss,
  --stealth=<N>
  -v,               Set verbosity level 0-2.
  -vv,
  --verbosity=<N>
  -o <file>,        Specify output log file (verbose level 2).
  --output-file <file>

Examples:
  Injection using default injection method (LoadLibraryA):
    vpr-pidjeon-x64.exe calc.exe ./payload.dll
  Injection using Manual Map injection method (mm) with verbosity enabled:
    vpr-pidjeon-x64.exe -i mm ./payload.bin 1234 -v

Version:
  2.0.1

Author:
  VPR
```

## Building
### Docker Container Live Instance
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
make docker-container && make-build
```
