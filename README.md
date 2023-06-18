<h1 align="center">vpr-pidjeon</h1>
<p align="center">
  <img src="https://img.shields.io/badge/Windows--x86__64-supported-green?style=flat-square"/>
  <img src="https://img.shields.io/badge/Windows--x86-supported-green?style=flat-square"/>
  <img src="https://img.shields.io/badge/Linux-unsupported-red?style=flat-square"/>
  <a href="https://mit-license.org/"/>
    <img src="https://img.shields.io/github/license/0xvpr/vpr-pidjeon?style=flat-square"/>
  </a>
  <img src="https://img.shields.io/github/actions/workflow/status/0xvpr/vpr-pidjeon/docker_build.yml?style=flat-square"/>
  <img src="https://img.shields.io/github/actions/workflow/status/0xvpr/vpr-pidjeon/windows_unit_tests.yml?label=tests?style=flat-square"/>
</p>
<br>

**pidjeon**, a mash-up of the words PID (process ID) and injection, is a command  
line application for Windows that uses various techniques to inject a specified  
payload into a target process.  

## Demonstration
[ TODO ]

## Usage
```bash
Error message: Missing positional arguments.

Usage: vpr-pidjeon.exe <target_process> <path/to/payload> [ <optional arguments> ]

positional arguments:
  target_process.exe, path/to/payload

optional arguments:
  -d            add delay to the injection (milliseconds)
  -i            specify injection method
  -o,--output   specify output file
  -r            create remote thread
  -s,--silent   suppress output
  -S,--stealth  specify stealth level
  -V            specify verbosity level

example:
  vpr-pidjeon.exe calc.exe ./payload.dll -i LoadLibraryA -d 2000
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
