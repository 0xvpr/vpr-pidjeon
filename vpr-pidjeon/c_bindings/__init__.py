__all__ = [
    "vpr_pidjeon_dll",
    "get_process_id_by_process_name",
    "log_basic",
    "log_advanced",
    "create_remote_thread",
    "load_library_a",
    "load_library_a_i686"
]

import ctypes
import os

from ctypes import (
    CDLL,
    POINTER
)

from ..types import (
    Resource,
    Injector
)

_current_dir = os.path.dirname(os.path.abspath(__file__))
vpr_pidjeon_dll = CDLL( _current_dir + "\\..\\lib\\vpr-pidjeon.dll");

_get_process_id_by_process_name = vpr_pidjeon_dll["get_process_id_by_process_name"]
_get_process_id_by_process_name.restype = ctypes.c_uint32
_get_process_id_by_process_name.argtypes = [ctypes.c_char_p]
get_process_id_by_process_name = _get_process_id_by_process_name

_log_basic = vpr_pidjeon_dll["log_basic"]
_log_basic.restype = ctypes.c_uint32
_log_basic.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_uint32]
log_basic = _log_basic

_log_advanced = vpr_pidjeon_dll["log_advanced"]
_log_advanced.restype = ctypes.c_uint32
_log_advanced.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_uint32]
log_advanced = _log_advanced

_create_remote_thread = vpr_pidjeon_dll["create_remote_thread"]
_create_remote_thread.restype = ctypes.c_uint32
_create_remote_thread.argtypes = [POINTER(Resource), POINTER(Injector)]
create_remote_thread = _create_remote_thread

_load_library_a = vpr_pidjeon_dll["load_library_a"]
_load_library_a.restype = ctypes.c_uint32
_load_library_a.argtypes = [POINTER(Resource), POINTER(Injector)]
load_library_a = _load_library_a

_load_library_a_i686 = vpr_pidjeon_dll["load_library_a_i686"]
_load_library_a_i686.restype = ctypes.c_uint32
_load_library_a_i686.argtypes = [POINTER(Resource), POINTER(Injector)]
load_library_a_i686 = _load_library_a_i686
