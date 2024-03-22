# __init__.py for mypackage

# Import some necessary classes or functions from modules for easy access
#from .mymodule import MyClass, my_function

# Initialize some package-level data
#package_data = {"key": "value"}

# Define what gets imported with "from mypackage import *"
__all__ = [
    "Resource",
    "Injector"
]

import ctypes

class Resource(ctypes.Structure):
    _fields_ = [
        ("process_id",              ctypes.c_uint32),
        ("target_process",          ctypes.c_char * 260),
        ("relative_payload_path",   ctypes.c_char * 260)
    ]

    def __init__(
            self,
            process_id:            ctypes.c_uint32 = ctypes.c_uint32(0),
            target_process:        bytes           = b"",
            relative_payload_path: bytes           = b"" ):

        super().__init__()
        self.process_id = process_id
        self.target_process = target_process
        self.relative_payload_path = relative_payload_path

class Injector(ctypes.Structure):
    _logger_t = ctypes.CFUNCTYPE(ctypes.c_int32, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_uint32)
    _fields_ = [
        ("status",                  ctypes.c_uint32),
        ("operation",               ctypes.c_int32),
        ("delay_ms",                ctypes.c_uint32),
        ("remote",                  ctypes.c_int32),
        ("silent",                  ctypes.c_int32),
        ("stealth",                 ctypes.c_int32),
        ("verbosity",               ctypes.c_int32),
        ("logger",                  _logger_t),
        ("output_file",             ctypes.c_char * 260)
    ]

    def __init__(
            self,
            delay_ms:    ctypes.c_uint32 = ctypes.c_uint32(0),
            remote:      ctypes.c_int32  = ctypes.c_int32(0),
            silent:      ctypes.c_int32  = ctypes.c_int32(0),
            stealth:     ctypes.c_int32  = ctypes.c_int32(0),
            verbosity:   ctypes.c_int32  = ctypes.c_int32(0),
            logger:      _logger_t       = 0,
            output_file: bytes           = b"vpr-pidjeon.log" ):

        super().__init__()
        self.status = 0
        self.delay_ms = delay_ms
        self.remote = remote
        self.silent = silent
        self.stealth = stealth
        self.verbosity = verbosity
        self.logger = self._logger_t(logger)
        self.output_file = output_file
