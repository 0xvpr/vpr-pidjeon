#!/usr/bin/env python3.exe

from .util import handle_command_line

from .types import (
    Resource,
    Injector
)

from .c_bindings import (
    vpr_pidjeon_dll,
    get_process_id_by_process_name,
    load_library_a,
)

from ctypes import (
    POINTER,
    byref,
    c_int32,
)

def _load_library_a(resource: POINTER(Resource), injector: POINTER(Injector)) -> c_int32:
    if not vpr_pidjeon_dll or not get_process_id_by_process_name or not load_library_a:
        Exception(ImportError)

    return load_library_a(
        resource,
        injector
    )

if __name__ == "__main__":
    resource, injector = handle_command_line()
    _load_library_a(
        byref(resource),
        byref(injector)
    )
